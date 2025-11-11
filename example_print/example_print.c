
#include "../src/internals.h"

WINBASEAPI int WINAPI MSVCRT$printf (const char *, ...);
WINBASEAPI int WINAPI MSVCRT$getchar (void);
WINBASEAPI HMODULE WINAPI KERNEL32$LoadLibraryA (LPCSTR);
WINBASEAPI VOID NTAPI NTDLL$NtAllocateVirtualMemory (HANDLE, PVOID, ULONG_PTR, PSIZE_T, ULONG, ULONG);
WINBASEAPI VOID NTAPI NTDLL$NtProtectVirtualMemory  (HANDLE, PVOID, PSIZE_T, ULONG, PULONG);
WINBASEAPI VOID NTAPI NTDLL$NtFreeVirtualMemory  (HANDLE, PVOID *, PSIZE_T, ULONG);

typedef void ( * ShellcodeMain )();

void go() {

    char* libName = "wininet";
    MSVCRT$printf("%s: 0x%p\n", libName, KERNEL32$LoadLibraryA(libName));


    PVOID pAllocatedLocalAddress = NULL;
    SIZE_T RegionSize = 0x1000 * 0x1000;

    NTDLL$NtAllocateVirtualMemory((HANDLE)-1, &pAllocatedLocalAddress, 0, &RegionSize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    MSVCRT$printf("Local region at: 0x%p\n", pAllocatedLocalAddress);

    if ( pAllocatedLocalAddress == NULL ) {
        return;
    }

    __stosb(pAllocatedLocalAddress, '\x90', RegionSize);    // All nops
    ((PBYTE)pAllocatedLocalAddress)[RegionSize-1] = '\xc3'; // And ret at the end


    DWORD dwOldProtect = 0;

    NTDLL$NtProtectVirtualMemory((HANDLE)-1, &pAllocatedLocalAddress, &RegionSize, PAGE_EXECUTE_READ, &dwOldProtect);
    MSVCRT$printf("Protection changed!\n");


    ((ShellcodeMain)pAllocatedLocalAddress)();

    MSVCRT$printf("Execution completed.\n");

    NTDLL$NtFreeVirtualMemory((HANDLE)-1, pAllocatedLocalAddress, &RegionSize, MEM_RELEASE);

    MSVCRT$printf("Press Enter to exit...\n");
    MSVCRT$getchar();

}
