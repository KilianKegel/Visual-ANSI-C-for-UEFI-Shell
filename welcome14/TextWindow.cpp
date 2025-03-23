#include <stdio.h>
#include <stdint.h>
#include <stdarg.h>
#include <wchar.h>
#include <limits.h>
#include <time.h>
#include <uefi.h>
#include "UefiBase.hpp"
#include "TextWindow.hpp"
#include "DPRINTF.H"
#include "base_t.h"

extern "C" EFI_SYSTEM_TABLE * gSystemTable;
extern "C" EFI_HANDLE gImageHandle;

//
// destructors
//
CTextWindow::~CTextWindow()	// full screen
{
	DPRINTF(("DTOR default\n"));
	//
	// clear entire window area
	//
	if (false == fFullScreen) {

		gSystemTable->ConOut->SetAttribute(gSystemTable->ConOut, BgAtt);

		for (int i = 0; i < WinDim.Y; i++)
		{
			GotoXY({ WinPos.X, i + WinPos.Y });
			TextPrint(L"%s", pwcsWinClrLine);
		}

	}
	else {
		DPRINTF(("DTOR default exit\n"));
		ClrScr(EFI_BACKGROUND_BLACK + EFI_WHITE);
	}
	
	delete this->pwcsWinClrLine;
	delete this->pwcsBlockDrawBuf;
	delete this->pwcsBlockScrtchBuf;

	DPRINTF(("DTOR default exit\n"));
}

//
// constructors
//
CTextWindow::CTextWindow(CTextWindow* pParent)	// full screen
{
	DPRINTF(("CTOR default\n"));
	fFullScreen = true;

	this->WinPos = { 0,0 };
	this->WinDim = ScrDim;
	this->pParent = pParent;
}


//
// full screen + clear screen with attrib
//
CTextWindow::CTextWindow(CTextWindow* pParent,int32_t ScreenAttrib)
{
	DPRINTF(("CTOR ScreenAttrib %X\n", ScreenAttrib));
	fFullScreen = true;

	size_t size = (sizeof(wchar_t) * ScrDim.X + sizeof(L'\0'));

	this->WinPos = {0,0};			// remember position
	this->WinDim = { ScrDim.X /* - 1*/, ScrDim.Y};	// remember size
	this->WinAtt = ScreenAttrib;	// remember attribute
	pwcsWinClrLine = new wchar_t[size];
	pwcsBlockDrawBuf = new wchar_t[WinDim.X * WinDim.Y + sizeof(L'\0')];
	pwcsBlockScrtchBuf = new wchar_t[WinDim.X * WinDim.Y + sizeof(L'\0')];
	this->pParent = pParent;

	wmemset(static_cast<wchar_t*>(pwcsWinClrLine), 0x20/* ASCII space*/, WinDim.X);	// init pwcsWinClrLine with space char
	pwcsWinClrLine[WinDim.X] = '\0';													// terminate the string

	ClrScr(EFI_BACKGROUND_BLUE);

	for (int i = 1; i < ScrDim.Y - 1; i++)
		TextPrint({ 0,i }, ScreenAttrib, L"%s", pwcsWinClrLine);
}

//
// TextWindow + clear window with attrib
//
CTextWindow::CTextWindow(CTextWindow* pParent, ABSPOS WinPos, ABSDIM WinDim, int32_t WinAtt)
{
	EFI_STATUS Status;
	DPRINTF(("CTOR ScreenAttrib %X\n", WinAtt));
	size_t size = (sizeof(wchar_t) * WinDim.X + sizeof(L'\0'));

	this->WinPos = WinPos;	// remember position
	this->WinDim = WinDim;	// remember size
	this->WinAtt = WinAtt;	// remember attribute
	pwcsWinClrLine = new wchar_t[size];
	pwcsBlockDrawBuf = new wchar_t[WinDim.X * WinDim.Y + sizeof(L'\0')];
	pwcsBlockScrtchBuf = new wchar_t[WinDim.X * WinDim.Y + sizeof(L'\0')];
	this->pParent = pParent;

	wmemset(static_cast<wchar_t*>(pwcsWinClrLine), 0x20/* ASCII space*/, WinDim.X);	// init pwcsWinClrLine with space char
	pwcsWinClrLine[WinDim.X] = '\0';														// terminate the string

	Status = gSystemTable->ConOut->SetAttribute(gSystemTable->ConOut, WinAtt);

	for (int i = 0; i < WinDim.Y; i++)
	{
		GotoXY({ WinPos.X, i + WinPos.Y });
		TextPrint(L"%s", pwcsWinClrLine);
	}
}

int CTextWindow::TextBorder(RELPOS TxtPos, ABSDIM WinDim, wchar_t upleft, wchar_t upright, wchar_t lowleft, wchar_t lowright, wchar_t horiz, wchar_t verti, wchar_t* pwcsTitle)
{
	size_t size = (sizeof(wchar_t) * WinDim.X + sizeof(L'\0'));
	wchar_t wcsCornerUL[2] = { upleft,'\0' };	// must be '\0'-terminated string
	wchar_t wcsCornerUR[2] = { upright,'\0' };	// must be '\0'-terminated string
	wchar_t wcsCornerLL[2] = { lowleft,'\0' };	// must be '\0'-terminated string
	wchar_t wcsCornerLR[2] = { lowright,'\0' };	// must be '\0'-terminated string
	wchar_t wcsCornerVER[2] = { verti,'\0' };	// must be '\0'-terminated string

	if (nullptr == pwcsWinHorizBorder)
	{
		pwcsWinHorizBorder = new wchar_t[size/sizeof(wchar_t)];
		wmemset(static_cast<wchar_t*>(pwcsWinHorizBorder), horiz, WinDim.X - 2/* left+right corner */);
		pwcsWinHorizBorder[WinDim.X - 2/* left+right corner */] = '\0';
	}

	TextPrint(TxtPos, L"%s%s%s", wcsCornerUL, pwcsWinHorizBorder, wcsCornerUR);

	TextPrint({ TxtPos.X ,TxtPos.Y + WinDim.Y - 1}, L"%s%s%s", wcsCornerLL, pwcsWinHorizBorder, wcsCornerLR);

	for (int i = 1; i <= WinDim.Y - 2; i++)
	{
		TextPrint({ TxtPos.X , TxtPos.Y + i}, L"%s", wcsCornerVER);
		TextPrint({ TxtPos.X + WinDim.X - 1, TxtPos.Y + i}, L"%s", wcsCornerVER);
	}

	if (nullptr != pwcsTitle)
	{
		//
		// NOTE: allow "-| TITLE |-" style titles with first and last charcter "|" in default window background
		//
		TextPrint({ WinDim.X / 2 - (int32_t)wcslen(pwcsTitle) / 2		,1 }, WinAtt, L"%s", pwcsTitle);
		TextPrint({ WinDim.X / 2 - (int32_t)wcslen(pwcsTitle) / 2 + 1	,1 }, EFI_BACKGROUND_BLUE | EFI_WHITE, L"%s", &pwcsTitle[1]);
		TextPrint({ WinDim.X / 2 - (int32_t)wcslen(pwcsTitle) / 2 - 1 + (int32_t)wcslen(pwcsTitle)		,1 }, WinAtt, L"%s", &pwcsTitle[-1 + wcslen(pwcsTitle)]);
	}
	return 0;
}


//
// TextPrint() - it uses printf()
//
int CTextWindow::TextPrint(const char* strFmt, ...)
{
	va_list ap;
	int nRet = 0;

	va_start(ap, strFmt);

	nRet = vfprintf(stdout, (const char* const)strFmt, ap);

	va_end(ap);

	return nRet;
}

int CTextWindow::TextVPrint(const wchar_t* wcsFmt, va_list ap)
{
	EFI_STATUS Status;
	int len = _vsnwprintf(0, 0, (const wchar_t*)wcsFmt, ap);	// get num of chars, don't write any char
	wchar_t* pwcs = new wchar_t[len + sizeof((char)'\0')];		// allocate buffer

	//
	// NOTE: wprintf() family doesn't print wchar_t characters above max. ASCII value 0xFF
	//		 Instead swprintf() deal perfectly with all other values!!!
	//
	_vsnwprintf(pwcs, UINT_MAX, (const wchar_t*)wcsFmt, ap);	// create the formatted wcs-/wchar_t-string

	Status = gSystemTable->ConOut->OutputString(				// write to the console
		gSystemTable->ConOut,
		(CHAR16*)pwcs
	);

	//todo: consider Status
	//printf("--> %ls\n", pwcs);
	delete pwcs;												// free buffer
	return (int)len;
}

int CTextWindow::TextVPrint(const char* strFmt, va_list ap)
{
	int nRet = 0;

	nRet = vfprintf(stdout, (const char* const)strFmt, ap);

	return nRet;
}

int CTextWindow::TextPrint(const wchar_t* wcsFmt, ...)
{
	va_list ap;
	int nRet = 0;

	va_start(ap, wcsFmt);

	nRet = TextVPrint((const wchar_t*)wcsFmt, ap);

	va_end(ap);

	return nRet;
}

int CTextWindow::TextPrint(RELPOS TxtPos, const char* strFmt, ...)//https://uefi.org/sites/default/files/resources/UEFI_Spec_2_8_final.pdf#page=525
{
	int nRet = 0;
	va_list ap;
	//EFI_STATUS Status = gSystemTable->ConOut->SetCursorPosition(gSystemTable->ConOut, static_cast<UINTN>(TxtPos.Col), static_cast<UINTN>(TxtPos.Row));
	EFI_STATUS Status = GotoXY({ WinPos.X + TxtPos.X, WinPos.Y + TxtPos.Y });

	if (EFI_SUCCESS == Status)
	{
		va_start(ap, strFmt);

		nRet = vfprintf(stdout, (const char* const)strFmt, ap);

		va_end(ap);
	}

	return nRet;
}

int CTextWindow::TextPrint(RELPOS TxtPos, WINATT TxtAtt, const char* strFmt, ...)//https://uefi.org/sites/default/files/resources/UEFI_Spec_2_8_final.pdf#page=525
{
	int nRet = 0;
	va_list ap;
	EFI_STATUS Status = GotoXY({ WinPos.X + TxtPos.X, WinPos.Y + TxtPos.Y });

	//gSystemTable->ConOut->Mode->Attribute = Attrib;

	Status = gSystemTable->ConOut->SetAttribute(gSystemTable->ConOut, TxtAtt);

	if (EFI_SUCCESS == Status)
	{
		va_start(ap, strFmt);

		nRet = vfprintf(stdout, (const char* const)strFmt, ap);

		va_end(ap);
	}

	return nRet;
}

int CTextWindow::TextPrint(RELPOS TxtPos, const wchar_t* wcsFmt, ...)//https://uefi.org/sites/default/files/resources/UEFI_Spec_2_8_final.pdf#page=525
{
	int nRet = 0;
	va_list ap;
	EFI_STATUS Status = GotoXY({WinPos.X + TxtPos.X, WinPos.Y + TxtPos.Y });

	if (EFI_SUCCESS == Status)
	{
		va_start(ap, wcsFmt);

		nRet = TextVPrint((const wchar_t*)wcsFmt, ap);

		va_end(ap);
	}

	return nRet;
}

int CTextWindow::TextPrint(RELPOS TxtPos, WINATT TxtAtt, const wchar_t* wcsFmt, ...)//https://uefi.org/sites/default/files/resources/UEFI_Spec_2_8_final.pdf#page=525
{
	int nRet = 0;
	va_list ap;
	EFI_STATUS Status = GotoXY({ WinPos.X + TxtPos.X, WinPos.Y + TxtPos.Y });

	Status = gSystemTable->ConOut->SetAttribute(gSystemTable->ConOut, TxtAtt);

	if (EFI_SUCCESS == Status)
	{
		va_start(ap, wcsFmt);

		nRet = TextVPrint((const wchar_t*)wcsFmt, ap);

		va_end(ap);
	}

	return nRet;
}

void CTextWindow::TextBlockDraw(RELPOS BlkPos)
{
	wchar_t wcseol[] = { L"\n" };
	size_t len = wcslen(this->pwcsBlockDrawBuf);
	wchar_t* pcontext, *pwcs;
	int32_t i = 0;

	this->BlkPos = BlkPos;	// save block position

	wmemcpy(this->pwcsBlockScrtchBuf, this->pwcsBlockDrawBuf, 1 + len);
	pwcs = wcstok(this->pwcsBlockScrtchBuf, &wcseol[0], &pcontext);
	do {
		if (nullptr == pwcs)
			break;
		CTextWindow::TextPrint({ BlkPos.X,BlkPos.Y + i++ }, pwcs);
		pwcs = wcstok(nullptr, &wcseol[0], &pcontext);
	} while (1);
}

void CTextWindow::TextBlockRfrsh(void)
{
	wchar_t wcseol[] = { L"\n" };
	size_t len = wcslen(this->pwcsBlockDrawBuf);
	wchar_t* pcontext, * pwcs;
	int32_t i = 0;

	wmemcpy(this->pwcsBlockScrtchBuf, this->pwcsBlockDrawBuf, 1 + len);
	pwcs = wcstok(this->pwcsBlockScrtchBuf, &wcseol[0], &pcontext);
	do {
		if (nullptr == pwcs)
			break;
		CTextWindow::TextPrint({ this->BlkPos.X,this->BlkPos.Y + i++ }, pwcs);
		pwcs = wcstok(nullptr, &wcseol[0], &pcontext);
	} while (1);
}

void CTextWindow::TextBlockClear(void)
{
	wchar_t wcseol[] = { L"\n" };
	size_t len = wcslen(this->pwcsBlockDrawBuf);
	wchar_t* pcontext, *pwcs;
	int32_t i = 0;

	wmemcpy(this->pwcsBlockScrtchBuf, this->pwcsBlockDrawBuf, 1 + len);
	pwcs = wcstok(this->pwcsBlockScrtchBuf, &wcseol[0], &pcontext);
	do {
		if (nullptr == pwcs)
			break;
		wmemset(pwcs, L'\x20', wcslen(pwcs));
		CTextWindow::TextPrint({ this->BlkPos.X,this->BlkPos.Y + i++ }, pwcs);
		pwcs = wcstok(nullptr, &wcseol[0], &pcontext);
	} while (1);

}

void CTextWindow::TextClearWindow(int BgAtt)
{
	gSystemTable->ConOut->SetAttribute(gSystemTable->ConOut, BgAtt);

	for (int i = 0; i < WinDim.Y; i++)
	{
		GotoXY({ WinPos.X, i + WinPos.Y });
		TextPrint(L"%s", pwcsWinClrLine);
	}
}

void CTextWindow::TextClearWindow(void)
{
	for (int i = 0; i < WinDim.Y; i++)
	{
		GotoXY({ WinPos.X, i + WinPos.Y });
		TextPrint(L"%s", pwcsWinClrLine);
	}
}

CTextWindow* CTextWindow::TextWindowGetRoot(void) {				// return instance without NULL == pParent
	
	CTextWindow* pRoot = this;

	while (nullptr != pRoot->pParent)
		pRoot = pRoot->pParent;
	
	return pRoot;
}

void CTextWindow::TextWindowUpdateProgress(void) {				// update the progress indicator at Root Window
	
	static const char* rgstrProgress[] = { "|", "/", "-", "\\" };
	CTextWindow* pRoot = TextWindowGetRoot();
	clock_t endclk = CLOCKS_PER_SEC / 16 + clock();
	#define PRGRSS  pRoot->nProgress

	PRGRSS = (PRGRSS + 1) % (sizeof(rgstrProgress) / sizeof(rgstrProgress[0]));

	pRoot->TextPrint({ pRoot->WinDim.X - 3,0 }, EFI_BACKGROUND_LIGHTGRAY + EFI_BLACK, "[%s]", rgstrProgress[PRGRSS]);

	while (endclk > clock())
		;

}

TEXT_KEY CTextWindow::TextGetKey(void) {
	
	TEXT_KEY key = NO_KEY;
	//
	// identify the key hits
	//

	EFI_KEY_DATA KeyData = this->ReadKeyStrokeEx();

	if (KeyData.Key.ScanCode == 0
		&& KeyData.Key.UnicodeChar == 0
		&& ((KeyData.KeyState.KeyShiftState == (EFI_SHIFT_STATE_VALID | EFI_LEFT_ALT_PRESSED)) /*left alt*/
			|| (KeyData.KeyState.KeyShiftState == (EFI_SHIFT_STATE_VALID | EFI_RIGHT_ALT_PRESSED)) /*ALT-GR*/))
		key = KEY_ALT;
	if (SCAN_F10 == KeyData.Key.ScanCode)
		key = KEY_F10;
	if (SCAN_ESC == KeyData.Key.ScanCode)
		key = KEY_ESC;
	if (SCAN_RIGHT == KeyData.Key.ScanCode)
		key = KEY_RIGHT;
	if (SCAN_LEFT == KeyData.Key.ScanCode)
		key = KEY_LEFT;
	if (SCAN_UP == KeyData.Key.ScanCode)
		key = KEY_UP;
	if (SCAN_DOWN == KeyData.Key.ScanCode)
		key = KEY_DOWN;
	if (CHAR_CARRIAGE_RETURN == KeyData.Key.UnicodeChar)
		key = KEY_ENTER;
	
	memset(&KeyData, 0xFF, sizeof(KeyData));

	return key;
}
