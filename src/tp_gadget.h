#pragma once
#include <windows.h>

typedef struct {
    ULONG_PTR functionPtr;
    ULONG_PTR argument1;
    ULONG_PTR argument2;
    ULONG_PTR argument3;
    ULONG_PTR argument4;
    ULONG_PTR argument5;
    ULONG_PTR argument6;
    ULONG_PTR argument7;
    ULONG_PTR argument8;
    ULONG_PTR argument9;
    ULONG_PTR argument10;
    ULONG_PTR argument11;
} NTARGS;

VOID ProxyNtApi(NTARGS * args);
