#ifndef _TEXT_WINDOWS_H_
#define _TEXT_WINDOWS_H_

#include <stdio.h>
#include <uefi.h>
#include "UefiBase.hpp"
#include "TextWindow.hpp"
#include "DPRINTF.h"
#include "base_t.h"

enum TEXT_KEY {
	NO_KEY, KEY_ALT, KEY_ESC, KEY_RIGHT, KEY_LEFT, KEY_UP, KEY_DOWN, KEY_ENTER,
	KEY_F1, KEY_F2, KEY_F3, KEY_F4, KEY_F5, KEY_F6, KEY_F7, KEY_F8, KEY_F9, KEY_F10, KEY_F11, KEY_F12
};

class CTextWindow : public CUefiBase {
public:
	CTextWindow* pParent;
	ABSPOS WinPos;										// Window position	absolute on the screen
	ABSDIM WinDim;										// Window dimension	absolute on the screen
	RELPOS TxtPos;										// Text position	relative in the window
	//ABSDIM TxtDim;										// Text dimension	relative in the window
	RELPOS BlkPos;										// Block position	relative in the window
	WINATT WinAtt;										// Window attribute
	WINATT BgAtt = EFI_BACKGROUND_BLACK + EFI_WHITE;	// Background attribute
	wchar_t* pwcsWinClrLine = nullptr;			// clear line wide string
	wchar_t* pwcsWinHorizBorder = nullptr;
	wchar_t* pwcsBlockDrawBuf = nullptr;			// text buffer size of X*Y
	wchar_t* pwcsBlockScrtchBuf = nullptr;		// clear buffer size of X*Y

	bool fFullScreen = false;
	int  nProgress = 0;									// counter for progress indicator

	CTextWindow(CTextWindow* pParent);
	CTextWindow(CTextWindow* pParent, int32_t ScreenAttrib);
	CTextWindow(CTextWindow* pParent, ABSPOS WinPos, ABSDIM WinDim, int32_t WindowAttrib);
	~CTextWindow();

	CTextWindow* TextWindowGetRoot(void);				// return instance without NULL == pParent
	void TextWindowUpdateProgress(void);				// update the progress indicator at Root Window

	int TextBorder(RELPOS TxtPos, ABSDIM WinDim, wchar_t upleft, wchar_t upright, wchar_t lowleft, wchar_t lowright, wchar_t horiz, wchar_t verti, wchar_t* pwcsTitle);

	int TextPrint(const char* strFmt, ...);										// narrow string
	int TextPrint(const wchar_t* wcsFmt, ...);									// wide string
	int TextPrint(RELPOS TxtPos, const char* strFmt, ...);						// narrow string
	int TextPrint(RELPOS TxtPos, const wchar_t* wcsFmt, ...);					// wide string
	int TextPrint(RELPOS TxtPos, WINATT TxtAtt, const char* strFmt, ...);		// narrow string
	int TextPrint(RELPOS TxtPos, WINATT TxtAtt, const wchar_t* wcsFmt, ...);	// wide string

	int TextVPrint(const char* strFmt, va_list ap);
	int TextVPrint(const wchar_t* wcsFmt, va_list ap);

	void TextBlockDraw(RELPOS BlkPos);											// place at XY a textblock, one or more '\n' separated lines
	//void TextBlockRfrsh(void);													// place at XY a textblock, one or more '\n' separated lines
	void TextBlockRfrsh(void);													// refresh last textblock
	void TextBlockClear(void);													// clear   last textblock

	void TextClearWindow(void);
	void TextClearWindow(int BgAtt);

	TEXT_KEY TextGetKey(void);
};

#endif//_TEXT_WINDOWS_H_
