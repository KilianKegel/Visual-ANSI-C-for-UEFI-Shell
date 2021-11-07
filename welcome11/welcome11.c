#include <uefi.h>
#include <stdio.h>

extern char* _strefierror(EFI_STATUS errcode);                           // Torito C extention according to strerror()

int main(int argc, char **argv) {
    EFI_SYSTEM_TABLE* SystemTable = (EFI_SYSTEM_TABLE*)(argv[-1]);      //SystemTable is passed in argv[-1]
    EFI_HANDLE ImageHandle = (void*)(argv[-2]);                         //ImageHandle is passed in argv[-2]
    EFI_STATUS Status = EFI_SUCCESS;

    //"https://uefi.org/sites/default/files/resources/UEFI_Spec_2_7.pdf#page=598"
    Status = SystemTable->ConOut->OutputString(SystemTable->ConOut, L"Welcome, to the jungle of \"the UEFI native API\"\r\n");    // "hello, world" on UEFI API

    puts(_strefierror(Status));
}
