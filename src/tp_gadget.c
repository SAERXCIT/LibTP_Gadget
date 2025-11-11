#include "tp_gadget.h"

WINBASEAPI HMODULE WINAPI KERNEL32$LoadLibraryA     (LPCSTR);
WINBASEAPI VOID NTAPI NTDLL$TpAllocWork   (PTP_WORK*, PTP_WORK_CALLBACK, PVOID, PTP_CALLBACK_ENVIRON);
WINBASEAPI VOID NTAPI NTDLL$TpPostWork    (PTP_WORK);
WINBASEAPI VOID NTAPI NTDLL$TpWaitForWork (PTP_WORK, BOOL);
WINBASEAPI VOID NTAPI NTDLL$TpReleaseWork (PTP_WORK);

typedef struct _TP_GADGET_PARAMS {
    ULONG_PTR pGadgetAddress;
    NTARGS NtArgs;
} TP_GADGET_PARAMS, *PTP_GADGET_PARAMS;

void __attribute__((naked)) WorkCallback()
{
    __asm__ __volatile__ (
        ".intel_syntax noprefix;"
        "sub rsp, 0x68;"            // Counteract `add rsp, 28h` in the gadget's epilogue; done in this function's "prologue"
        "mov rax, [rdx + 0x8];"     // Put pTargetFunctionAddress into rax; will be called by the gadget
        "mov rcx, [rdx + 0x10];"    // Put Param1 in rcx
        "mov r8,  [rdx + 0x20];"    // Put Param3 in r8
        "mov r9,  [rdx + 0x28];"    // Put Param4 in r9
        "mov r11, [rdx + 0x30];"    // Put Param5 in temp register
        "mov [rsp+0x20], r11;"      // Put temp register as 5th parameter -> We're jumping to a `call`, so there'll be another value pushed to the stack, hence rsp+0x20 for the 5th parameter instead of rsp+0x28
        "mov r11, [rdx + 0x38];"    // Put Param6 in temp register
        "mov [rsp+0x28], r11;"      // Put temp register as 6th parameter
        "mov r11, [rdx + 0x40];"    // Put Param7 in temp register
        "mov [rsp+0x30], r11;"      // Put temp register as 7th parameter
        "mov r11, [rdx + 0x48];"    // Put Param8 in temp register
        "mov [rsp+0x38], r11;"      // Put temp register as 8th parameter
        "mov r11, [rdx + 0x50];"    // Put Param9 in temp register
        "mov [rsp+0x40], r11;"      // Put temp register as 9th parameter
        "mov r11, [rdx + 0x58];"    // Put Param10 in temp register
        "mov [rsp+0x48], r11;"      // Put temp register as 10th parameter
        "mov r11, [rdx + 0x60];"    // Put Param11 in temp register
        "mov [rsp+0x50], r11;"      // Put temp register as 11th parameter

        "mov r11, [rdx];"           // Put pGadgetAddress into r11; will be jumped to
        "mov rdx, [rdx + 0x18];"    // Put Param2 in rdx
        "jmp r11;"                  // Jmp to the gadget, will not put this function's address in the call stack
        ".att_syntax prefix;"
    );
}

ULONG_PTR GetCallGadgetAddress(
    PBYTE pModule
) {

    DWORD i = { 0 };

    for (i = 0x1001; i < (0x1000 + 0xDEADBEEF); i++) {
        if (pModule[i]     == 0xFF       &&  // call rax
            pModule[i + 1] == 0xD0       &&  // call rax
            pModule[i + 8] == 0x68       &&  // add rsp,68
            pModule[i + 9] == 0xC3           // ret
            ) {
            return (ULONG_PTR)(&(pModule[i]));
        }
    }

    return 0;

}

VOID ProxyNtApi(NTARGS * args) {

    if (args == NULL) return;

    PTP_WORK WorkReturn = NULL;

    TP_GADGET_PARAMS Params = { 0 };
    Params.NtArgs = *args;

    HMODULE hGadget = KERNEL32$LoadLibraryA("module_with_gadget");
    Params.pGadgetAddress = GetCallGadgetAddress((PBYTE)hGadget);

    NTDLL$TpAllocWork(&WorkReturn, (PTP_WORK_CALLBACK)WorkCallback, &Params, NULL);
    NTDLL$TpPostWork(WorkReturn);
    NTDLL$TpWaitForWork(WorkReturn, FALSE);
    NTDLL$TpReleaseWork(WorkReturn);

    return;

}
