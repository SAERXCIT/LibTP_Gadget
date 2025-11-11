#include "hooks.h"

WINBASEAPI HMODULE WINAPI KERNEL32$LoadLibraryA (LPCSTR);
WINBASEAPI HMODULE WINAPI KERNEL32$GetModuleHandleA (LPCSTR);
WINBASEAPI VOID NTAPI NTDLL$NtOpenProcess (PHANDLE, ACCESS_MASK, PCOBJECT_ATTRIBUTES, PCLIENT_ID);
WINBASEAPI VOID NTAPI NTDLL$NtAllocateVirtualMemory (HANDLE, PVOID, ULONG_PTR, PSIZE_T, ULONG, ULONG);
WINBASEAPI VOID NTAPI NTDLL$NtWriteVirtualMemory (HANDLE, PVOID, PVOID, SIZE_T, PSIZE_T);
WINBASEAPI VOID NTAPI NTDLL$NtProtectVirtualMemory  (HANDLE, PVOID, PSIZE_T, ULONG, PULONG);
WINBASEAPI VOID NTAPI NTDLL$NtFreeVirtualMemory  (HANDLE, PVOID *, PSIZE_T, ULONG);
WINBASEAPI VOID NTAPI NTDLL$NtClose  (HANDLE);
WINBASEAPI VOID NTAPI NTDLL$NtCreateThreadEx  (PHANDLE, ACCESS_MASK, PCOBJECT_ATTRIBUTES, HANDLE, PUSER_THREAD_START_ROUTINE, PVOID, ULONG, SIZE_T, SIZE_T, SIZE_T, PPS_ATTRIBUTE_LIST);
WINBASEAPI VOID NTAPI NTDLL$NtOpenThread (PHANDLE, ACCESS_MASK, PCOBJECT_ATTRIBUTES, PCLIENT_ID);
WINBASEAPI VOID NTAPI NTDLL$NtSuspendThread (HANDLE, PULONG);
WINBASEAPI VOID NTAPI NTDLL$NtResumeThread (HANDLE, PULONG);
WINBASEAPI VOID NTAPI NTDLL$NtGetContextThread(HANDLE, PCONTEXT);
WINBASEAPI VOID NTAPI NTDLL$NtSetContextThread(HANDLE, PCONTEXT);
WINBASEAPI VOID NTAPI NTDLL$NtWaitForSingleObject (HANDLE, BOOLEAN, PLARGE_INTEGER);

HMODULE WINAPI H_LoadLibraryA(
    LPCSTR lpLibFileName
    ) {

    NTARGS Args = { 0 };
    Args.functionPtr = (ULONG_PTR)KERNEL32$LoadLibraryA;
    Args.argument1   = (ULONG_PTR)lpLibFileName;

    ProxyNtApi(&Args);

    return KERNEL32$GetModuleHandleA(lpLibFileName);

}

NTSTATUS NTAPI H_NtOpenProcess(
    _Out_ PHANDLE ProcessHandle,
    _In_ ACCESS_MASK DesiredAccess,
    _In_ PCOBJECT_ATTRIBUTES ObjectAttributes,
    _In_opt_ PCLIENT_ID ClientId
    ) {

    NTARGS Args = { 0 };
    Args.functionPtr = (ULONG_PTR)NTDLL$NtOpenProcess;
    Args.argument1   = (ULONG_PTR)ProcessHandle;
    Args.argument2   = (ULONG_PTR)DesiredAccess;
    Args.argument3   = (ULONG_PTR)ObjectAttributes;
    Args.argument4   = (ULONG_PTR)ClientId;

    ProxyNtApi(&Args);
    return (*ProcessHandle == NULL || *ProcessHandle == INVALID_HANDLE_VALUE) ? STATUS_INVALID_PARAMETER : STATUS_SUCCESS;

}

NTSTATUS NTAPI H_NtAllocateVirtualMemory(
    _In_ HANDLE ProcessHandle,
    _Inout_ _At_(*BaseAddress, _Readable_bytes_(*RegionSize) _Writable_bytes_(*RegionSize) _Post_readable_byte_size_(*RegionSize)) PVOID *BaseAddress,
    _In_ ULONG_PTR ZeroBits,
    _Inout_ PSIZE_T RegionSize,
    _In_ ULONG AllocationType,
    _In_ ULONG PageProtection
    ) {

    NTARGS Args = { 0 };
    Args.functionPtr = (ULONG_PTR)NTDLL$NtAllocateVirtualMemory;
    Args.argument1   = (ULONG_PTR)ProcessHandle;
    Args.argument2   = (ULONG_PTR)BaseAddress;
    Args.argument3   = (ULONG_PTR)ZeroBits;
    Args.argument4   = (ULONG_PTR)RegionSize;
    Args.argument5   = (ULONG_PTR)AllocationType;
    Args.argument6   = (ULONG_PTR)PageProtection;

    ProxyNtApi(&Args);

    return *BaseAddress == NULL ? STATUS_INVALID_PARAMETER : STATUS_SUCCESS;

}

NTSTATUS NTAPI H_NtWriteVirtualMemory(
    _In_ HANDLE ProcessHandle,
    _In_opt_ PVOID BaseAddress,
    _In_reads_bytes_(NumberOfBytesToWrite) PVOID Buffer,
    _In_ SIZE_T NumberOfBytesToWrite,
    _Out_opt_ PSIZE_T NumberOfBytesWritten
    ) {

    NTARGS Args = { 0 };
    Args.functionPtr = (ULONG_PTR)NTDLL$NtWriteVirtualMemory;
    Args.argument1   = (ULONG_PTR)ProcessHandle;
    Args.argument2   = (ULONG_PTR)BaseAddress;
    Args.argument3   = (ULONG_PTR)Buffer;
    Args.argument4   = (ULONG_PTR)NumberOfBytesToWrite;
    Args.argument5   = (ULONG_PTR)NumberOfBytesWritten;

    ProxyNtApi(&Args);

    return STATUS_SUCCESS;

}

NTSTATUS NTAPI H_NtProtectVirtualMemory(
    _In_ HANDLE ProcessHandle,
    _Inout_ PVOID *BaseAddress,
    _Inout_ PSIZE_T RegionSize,
    _In_ ULONG NewProtection,
    _Out_ PULONG OldProtection
    ) {

    NTARGS Args = { 0 };
    Args.functionPtr = (ULONG_PTR)NTDLL$NtProtectVirtualMemory;
    Args.argument1   = (ULONG_PTR)ProcessHandle;
    Args.argument2   = (ULONG_PTR)BaseAddress;
    Args.argument3   = (ULONG_PTR)RegionSize;
    Args.argument4   = (ULONG_PTR)NewProtection;
    Args.argument5   = (ULONG_PTR)OldProtection;

    ProxyNtApi(&Args);

    return STATUS_SUCCESS;

}

NTSTATUS NTAPI H_NtFreeVirtualMemory(
    _In_ HANDLE ProcessHandle,
    _Inout_ __drv_freesMem(Mem) PVOID *BaseAddress,
    _Inout_ PSIZE_T RegionSize,
    _In_ ULONG FreeType
    ) {

    NTARGS Args = { 0 };
    Args.functionPtr = (ULONG_PTR)NTDLL$NtFreeVirtualMemory;
    Args.argument1   = (ULONG_PTR)ProcessHandle;
    Args.argument2   = (ULONG_PTR)BaseAddress;
    Args.argument3   = (ULONG_PTR)RegionSize;
    Args.argument4   = (ULONG_PTR)FreeType;

    ProxyNtApi(&Args);

    return STATUS_SUCCESS;

}

NTSTATUS NTAPI H_NtClose(
    _In_ _Post_ptr_invalid_ HANDLE Handle
    ) {

    NTARGS Args = { 0 };
    Args.functionPtr = (ULONG_PTR)NTDLL$NtClose;
    Args.argument1   = (ULONG_PTR)Handle;

    ProxyNtApi(&Args);

    return STATUS_SUCCESS;

}

NTSTATUS NTAPI H_NtCreateThreadEx(
    _Out_ PHANDLE ThreadHandle,
    _In_ ACCESS_MASK DesiredAccess,
    _In_opt_ PCOBJECT_ATTRIBUTES ObjectAttributes,
    _In_ HANDLE ProcessHandle,
    _In_ PUSER_THREAD_START_ROUTINE StartRoutine,
    _In_opt_ PVOID Argument,
    _In_ ULONG CreateFlags, // THREAD_CREATE_FLAGS_*
    _In_ SIZE_T ZeroBits,
    _In_ SIZE_T StackSize,
    _In_ SIZE_T MaximumStackSize,
    _In_opt_ PPS_ATTRIBUTE_LIST AttributeList
    ) {

    NTARGS Args = { 0 };
    Args.functionPtr = (ULONG_PTR)NTDLL$NtCreateThreadEx;
    Args.argument1   = (ULONG_PTR)ThreadHandle;
    Args.argument2   = (ULONG_PTR)DesiredAccess;
    Args.argument3   = (ULONG_PTR)ObjectAttributes;
    Args.argument4   = (ULONG_PTR)ProcessHandle;
    Args.argument5   = (ULONG_PTR)StartRoutine;
    Args.argument6   = (ULONG_PTR)Argument;
    Args.argument7   = (ULONG_PTR)CreateFlags;
    Args.argument8   = (ULONG_PTR)ZeroBits;
    Args.argument9   = (ULONG_PTR)StackSize;
    Args.argument10  = (ULONG_PTR)MaximumStackSize;
    Args.argument11  = (ULONG_PTR)AttributeList;

    ProxyNtApi(&Args);

    return (*ThreadHandle == NULL || *ThreadHandle == INVALID_HANDLE_VALUE) ? STATUS_INVALID_PARAMETER : STATUS_SUCCESS;

}

NTSTATUS NTAPI H_NtOpenThread(
    _Out_ PHANDLE ThreadHandle,
    _In_ ACCESS_MASK DesiredAccess,
    _In_ PCOBJECT_ATTRIBUTES ObjectAttributes,
    _In_opt_ PCLIENT_ID ClientId
    ) {

    NTARGS Args = { 0 };
    Args.functionPtr = (ULONG_PTR)NTDLL$NtOpenThread;
    Args.argument1   = (ULONG_PTR)ThreadHandle;
    Args.argument2   = (ULONG_PTR)DesiredAccess;
    Args.argument3   = (ULONG_PTR)ObjectAttributes;
    Args.argument4   = (ULONG_PTR)ClientId;

    ProxyNtApi(&Args);

    return (*ThreadHandle == NULL || *ThreadHandle == INVALID_HANDLE_VALUE) ? STATUS_INVALID_PARAMETER : STATUS_SUCCESS;

}

NTSTATUS NTAPI H_NtSuspendThread(
    _In_ HANDLE ThreadHandle,
    _Out_opt_ PULONG PreviousSuspendCount
    ) {

    NTARGS Args = { 0 };
    Args.functionPtr = (ULONG_PTR)NTDLL$NtSuspendThread;
    Args.argument1   = (ULONG_PTR)ThreadHandle;
    Args.argument2   = (ULONG_PTR)PreviousSuspendCount;

    ProxyNtApi(&Args);

    return STATUS_SUCCESS;

}

NTSTATUS NTAPI H_NtResumeThread(
    _In_ HANDLE ThreadHandle,
    _Out_opt_ PULONG PreviousSuspendCount
    ) {

    NTARGS Args = { 0 };
    Args.functionPtr = (ULONG_PTR)NTDLL$NtResumeThread;
    Args.argument1   = (ULONG_PTR)ThreadHandle;
    Args.argument2   = (ULONG_PTR)PreviousSuspendCount;

    ProxyNtApi(&Args);

    return STATUS_SUCCESS;

}

NTSTATUS NTAPI H_NtGetContextThread(
    _In_ HANDLE ThreadHandle,
    _Inout_ PCONTEXT ThreadContext
    ) {

    NTARGS Args = { 0 };
    Args.functionPtr = (ULONG_PTR)NTDLL$NtGetContextThread;
    Args.argument1   = (ULONG_PTR)ThreadHandle;
    Args.argument2   = (ULONG_PTR)ThreadContext;

    ProxyNtApi(&Args);

    return STATUS_SUCCESS;

}

NTSTATUS NTAPI H_NtSetContextThread(
    _In_ HANDLE ThreadHandle,
    _In_ PCONTEXT ThreadContext
    ) {

    NTARGS Args = { 0 };
    Args.functionPtr = (ULONG_PTR)NTDLL$NtSetContextThread;
    Args.argument1   = (ULONG_PTR)ThreadHandle;
    Args.argument2   = (ULONG_PTR)ThreadContext;

    ProxyNtApi(&Args);

    return STATUS_SUCCESS;

}



NTSTATUS NTAPI H_NtWaitForSingleObject(
    _In_ HANDLE Handle,
    _In_ BOOLEAN Alertable,
    _In_opt_ PLARGE_INTEGER Timeout
    ) {

    NTARGS Args = { 0 };
    Args.functionPtr = (ULONG_PTR)NTDLL$NtWaitForSingleObject;
    Args.argument1   = (ULONG_PTR)Handle;
    Args.argument2   = (ULONG_PTR)Alertable;
    Args.argument3   = (ULONG_PTR)Timeout;

    ProxyNtApi(&Args);

    return STATUS_SUCCESS;

}
