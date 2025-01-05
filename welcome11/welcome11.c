#include <uefi.h>
#include <stdio.h>
#include <stdlib.h>

#define TICKS_PER_SECOND  10000000U

EFI_SYSTEM_TABLE* SystemTable;
EFI_HANDLE ImageHandle;
EFI_EVENT                        Timer;

//Available functions: https://github.com/KilianKegel/toro-C-Library#implementation-status

extern char* _strefierror(EFI_STATUS errcode);                           // Torito C extention according to strerror()

VOID EFIAPI cbTimer(EFI_EVENT Event, VOID* Context)
{
    printf(".");
}

int main(int argc, char** argv) {
    SystemTable = (EFI_SYSTEM_TABLE*)(argv[-1]);            //SystemTable is passed in argv[-1]
    ImageHandle = (void*)(argv[-2]);                        //ImageHandle is passed in argv[-2]
    EFI_STATUS Status = EFI_SUCCESS;

    //"https://uefi.org/sites/default/files/resources/UEFI_Spec_2_7.pdf#page=598"
    Status = SystemTable->ConOut->OutputString(SystemTable->ConOut, L"Welcome, to the jungle of \"the UEFI native API\"\r\n");    // "hello, world" on UEFI API

    printf("Status OutputString(): \"%s\"\n", _strefierror(Status));

    if (1)
    {

        Status = SystemTable->BootServices->CreateEvent(EVT_TIMER | EVT_NOTIFY_SIGNAL, TPL_CALLBACK, &cbTimer, NULL, &Timer);
        printf("Status CreateEvent(): \"%s\"\n", _strefierror(Status));


        Status = SystemTable->BootServices->SetTimer(Timer, TimerPeriodic, TICKS_PER_SECOND);
        printf("Status SetTimer(): \"%s\"\n", _strefierror(Status));

        if (1)
        {
            UINTN Index;
            EFI_INPUT_KEY Key;
            EFI_STATUS Status;
			
            printf("Press any key to terminate the program, NOT CTRL-C\n");
            do {                                            // read real kbhit(), since STDIN is redirected
                SystemTable->BootServices->WaitForEvent(1, SystemTable->ConIn->WaitForKey, &Index);
                Status = SystemTable->ConIn->ReadKeyStroke(SystemTable->ConIn, &Key);
            } while (EFI_SUCCESS != Status);
        }
		printf("\n  ---uninstalling event handler---\n");
		Status = SystemTable->BootServices->CloseEvent(Timer);
        printf("Status CloseEvent(): \"%s\"\n", _strefierror(Status));
    }
}
