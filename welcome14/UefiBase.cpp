#define _UEFI_BASE_CPP_

#include <stdio.h>
#include <string.h>
#include <uefi.h>
#include "UefiBase.hpp"
#include "TextWindow.hpp"
#include "DPRINTF.H"
#include "base_t.h"

extern "C" EFI_HANDLE gImageHandle = 0;
extern "C" EFI_SYSTEM_TABLE * gSystemTable = 0;

CUefiBase::CUefiBase()
{
    EFI_STATUS Status = EFI_SUCCESS;
    //
    // get text screen  dimension
    //
    VideoModeCurrent = gSystemTable->ConOut->Mode->Mode;
    VideoModeMax = gSystemTable->ConOut->Mode->MaxMode;
    ScrDim = QueryMode(VideoModeCurrent);

    Status = gSystemTable->BootServices->HandleProtocol(
        gSystemTable->ConsoleInHandle,
        &SimpleTextInputExProtocolGuid,
        (void**)&pSimpleTextInputExProtocol);

    DPRINTF(("Rows %d, Cols %d\n", ScrDim.X, ScrDim.Y));
}

//
// EFI_STATUS EFIAPI QueryMode(int32_t ModeNumber)
//
ABSDIM CUefiBase::QueryMode(
    IN int32_t ModeNumber
) {
    UINTN Columns = 0;
    UINTN  Rows = 0;
    ABSDIM TextDim = { -1,-1 };

    EFI_STATUS Status = EFI_SUCCESS;
    Status = gSystemTable->ConOut->QueryMode(gSystemTable->ConOut, (UINTN)VideoModeCurrent, (UINTN*)&Columns, (UINTN*)&Rows);

    if (EFI_SUCCESS == Status)
    {
        TextDim.X = static_cast<uint32_t>(Columns);
        TextDim.Y = static_cast<uint32_t>(Rows);
    }
    return TextDim;
}

//
// EFI_STATUS EFIAPI ClrScr(void)
//
EFI_STATUS CUefiBase::ClrScr(void)
{
    gSystemTable->ConOut->EnableCursor(gSystemTable->ConOut, 0);
    return gSystemTable->ConOut->ClearScreen(gSystemTable->ConOut);
    
    return EFI_SUCCESS;
}
EFI_STATUS CUefiBase::ClrScr(int32_t attr/*console color*/)
{
    gSystemTable->ConOut->Mode->CursorVisible = 0;
    gSystemTable->ConOut->SetMode(gSystemTable->ConOut, gSystemTable->ConOut->Mode->Mode);
    gSystemTable->ConOut->SetAttribute(gSystemTable->ConOut, static_cast<UINTN>(attr));

    ClrScr();

    return EFI_SUCCESS;
}
//
// GotoXY
//
EFI_STATUS CUefiBase::GotoXY(int32_t x, int32_t y) 
{
    EFI_STATUS Status = EFI_SUCCESS;

    Status = gSystemTable->ConOut->SetCursorPosition(gSystemTable->ConOut, static_cast<UINTN>(x), static_cast<UINTN>(y));

    return Status;
}

EFI_STATUS CUefiBase::GotoXY(ABSPOS WinPos)
{
    EFI_STATUS Status = EFI_SUCCESS;
        Status = gSystemTable->ConOut->SetCursorPosition(gSystemTable->ConOut, static_cast<UINTN>(WinPos.X), static_cast<UINTN>(WinPos.Y));

    return Status;
}

bool gfKbdDbg = false;

EFI_KEY_DATA CUefiBase::ReadKeyStrokeEx(void)
{
    EFI_STATUS  Status = EFI_SUCCESS;
    EFI_KEY_DATA KeyData;
    /* init */
    KeyData.Key.ScanCode = 0x0000;
    KeyData.Key.UnicodeChar = 0x0000;
    KeyData.KeyState.KeyShiftState = 0x00000000;
    KeyData.KeyState.KeyToggleState = 0x00;

    memset(&KeyData, 0, sizeof(KeyData));

    if (EFI_SUCCESS == Status)
        Status = pSimpleTextInputExProtocol->ReadKeyStrokeEx(pSimpleTextInputExProtocol, &KeyData);

    if (EFI_SUCCESS != Status)
        memset(&KeyData, 0xFF, sizeof(KeyData));

    if(true == gfKbdDbg)
        GotoXY({ 2,ScrDim.Y - 3 }),
        printf("DBG: SCode %04X UniChar %04X KShiftState %X KToggleState %04X", KeyData.Key.ScanCode, KeyData.Key.UnicodeChar, KeyData.KeyState.KeyShiftState, KeyData.KeyState.KeyToggleState);

    return KeyData;
}
