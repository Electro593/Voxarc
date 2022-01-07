/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
**                                                                         **
**  Author: Aria Seiler                                                    **
**                                                                         **
**  This program is in the public domain. There is no implied warranty,    **
**  so use it at your own risk.                                            **
**                                                                         **
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <shared.h>

internal void
LoadWin32Funcs(void)
{
    win32_teb *TEB = (win32_teb*)Asm_ReadGSQWord(48);
    win32_list_entry *Entry = TEB->PEB->Ldr->MemoryOrderList.Next;
    Entry = Entry->Next->Next;
    win32_module Kernel32 = ((win32_ldr_data_table_entry*)Entry)->DllBase;
    
    win32_image_dos_header *DOSHeader = (win32_image_dos_header*)Kernel32;
    win32_image_nt_headers *NTHeaders = (win32_image_nt_headers*)((u08*)DOSHeader + DOSHeader->e_lfanew);
    win32_image_export_directory *ExportDirectory = (win32_image_export_directory*)((u08*)DOSHeader + NTHeaders->OptionalHeader.ExportTable.VirtualAddress);
    u32 *ExportNameTable    = (u32*)((u08*)DOSHeader + ExportDirectory->AddressOfNames);
    u16 *ExportOrdinalTable = (u16*)((u08*)DOSHeader + ExportDirectory->AddressOfNameOrdinals);
    u32 *ExportAddressTable = (u32*)((u08*)DOSHeader + ExportDirectory->AddressOfFunctions);
    
    u32 Index;
    u32 Low = 0;
    u32 High = ExportDirectory->NumberOfNames - 1;
    while(TRUE) {
        Index = (High + Low) / 2;
        c08 *Goal = "GetProcAddress";
        c08 *Curr = (c08*)((u08*)DOSHeader + ExportNameTable[Index]);
        
        while(*Goal && *Curr && *Goal == *Curr) {
            Goal++;
            Curr++;
        }
        
        if(*Goal == *Curr) break;
        if(*Goal > *Curr) Low = Index;
        else High = Index;
    }
    
    u16 GetProcAddressOrdinal = ExportOrdinalTable[Index];
    u32 GetProcAddressRVA = ExportAddressTable[GetProcAddressOrdinal];
    Win32_GetProcAddress = (func_Win32_GetProcAddress*)((u08*)DOSHeader + GetProcAddressRVA);
    
    Win32_LoadLibraryA = (func_Win32_LoadLibraryA*)Win32_GetProcAddress(Kernel32, "LoadLibraryA");
    
    #define PROC(Module, ReturnType, Name, ...) \
        Win32_##Name = (func_Win32_##Name*)Win32_GetProcAddress(Module, #Name);
    WIN32_FUNCS
    #undef PROC
}

external void _API_ENTRY
Entry(void)
{
    
    
    
    
    
    // ExitProcess(0);
}