# LibTP_Gadget

This is a generalisation of https://github.com/AlmondOffSec/LibTPLoadLib to proxy APIs with an arbitrary number (11 max actually, but good enough) of arguments. Provided as a [Crystal Palace](https://tradecraftgarden.org/crystalpalace.html) shared library. API made compatible with @rasta-mouse's [LibTP](https://github.com/rasta-mouse/LibTP) for easy switch using spec files. Hooks are provided to show off the [newest Crystal Palace features](https://aff-wg.org/2025/11/10/tradecraft-engineering-with-aspect-oriented-programming/).

**WARNING ⚠️** : This project is not usable as-is. I am not providing a module in which to find a `call` gadget. You'll have to find your own. [Read the blogpost for more info](https://offsec.almond.consulting/evading-elastic-callstack-signatures.html).

**Note** : Due to the ability to pass 11 arguments (7 of which will be put on the stack), the call gadget must reside within a function with a large enough stack frame. Look for an `add rsp` of at least `0x68`.

**WARNING ⚠️** : Due to the inner workings of this technique, it is not possible to retrieve the return value of the proxied function. Proxying NTAPIs is generally more usable, since their return value is "only" the `NTSTATUS`. It does make it difficult to know whether the function actually succeeded 🙃.

## How

1. Find a working gadget in a module available on the target. In `src/tp_gadget.c`, patch its name in the `LoadLibraryA` call and the function `GetCallGadgetAddress` to make it retrieve its address. Patch the assembly stub if necessary (`sub rsp` value, register that the gadget will call).

2. Compile the project: `make`. The output is the Crystal Palace shared library `libtp_gadget.x64.zip`.

3. Compile the example COFF (that will just load `wininet.dll` then locally alloc and execute shellcode): `make -C example_print`. The output will be a COFF file `example_print.x64.o`.

4. Create a Crystal Palace spec file instructing the linker to use our hooks on DFR: Use the Tradecraft Garden's Simple PIC spec file, and add the following lines between `mergelib "../libtcg.x64.zip"` and `export`:

```
        mergelib "../libtp_gadget.x64.zip"

        attach "KERNEL32$LoadLibraryA"          "H_LoadLibraryA"
        optout "ProxyNtApi"                     "H_LoadLibraryA"
        attach "NTDLL$NtOpenProcess"            "H_NtOpenProcess"
        attach "NTDLL$NtAllocateVirtualMemory"  "H_NtAllocateVirtualMemory"
        attach "NTDLL$NtWriteVirtualMemory"     "H_NtWriteVirtualMemory"
        attach "NTDLL$NtProtectVirtualMemory"   "H_NtProtectVirtualMemory"
        attach "NTDLL$NtFreeVirtualMemory"      "H_NtFreeVirtualMemory"
        attach "NTDLL$NtClose"                  "H_NtClose"
        attach "NTDLL$NtCreateThreadEx"         "H_NtCreateThreadEx"
        attach "NTDLL$NtOpenThread"             "H_NtOpenThread"
        attach "NTDLL$NtSuspendThread"          "H_NtSuspendThread"
        attach "NTDLL$NtResumeThread"           "H_NtResumeThread"
        attach "NTDLL$NtGetContextThread"       "H_NtGetContextThread"
        attach "NTDLL$NtSetContextThread"       "H_NtSetContextThread"
        attach "NTDLL$NtWaitForSingleObject"    "H_NtWaitForSingleObject"
```

5. Link the whole thing using Crystal Palace to make PIC shellcode: `path/to/crystalpalace/link simplepic_modified/loader.spec example_print/example_print.x64.o out.bin`.

6. Run the shellcode using any loader.

## Credits

- Raphael Mudge, [Adversary Fan Fiction Writers Guild](https://aff-wg.org/)
- Chetan Nayak, [Dark Vortex](https://0xdarkvortex.dev/hiding-in-plainsight/)
- Daniel Duggan, [Rasta Mouse](https://rastamouse.me/)
