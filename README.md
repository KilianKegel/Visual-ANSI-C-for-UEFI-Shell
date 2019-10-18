# Visual ANSI C for UEFI Shell
Create and debug Standard C UEFI Shell EFI_APPLICATION using Visual Studio 2019.

# Revision history
### 20191017/R141
* add CTRL-C support<br>
  NOTE: CTRL-C is implemented on Signal handling `<signal.h>` interface and can be catched and supressed
        by the application. https://github.com/KilianKegel/Visual-ANSI-C-for-UEFI-Shell/blob/master/welcome9/welcome9.c
* fixed system() library function does not workaround completely the EfiShellProtocol->Execute() bug
  to display the drive mappings, when multipartition ("BLK10") mass storage device is attached
* implement UEFI Shell compatibility mode for STDOUT and STDERR to allow I/O redirection `>a` for ASCII and `>` for UCS-2/UTF16
* add build switch `char _gSTDOUTMode;   /* 0 == UEFI Shell default, 1 == ASCII only */` to force TORITO C backward compatibility ( ASCII mode only )
### 20190922/R138
* Update to torito-C 20190621/R138 that autodetects I/O redirection UTF16 vs. ASCII
* add support for VS2019 ver 16.2.5
### 20190918/R137
* update to VS2019 Version 16.2.5 (Spectre Support)
### 20190621/R133
* Update to torito - C 20190621 / R133 that adds VS2019 support, remove library dependancy from compiler version
### 20190410/R126
* Update to torito - C 20190410 / R126 that provides various improvements
### 20190109/R123
* Update to torito-C 20190108/R123 that provides various improvements
### 20181216/R119
* Update to torito-C 20181216/R119 that provides various improvements
* add "WinNTx86-64 (Microsoft C Library)" solution configuration. 
  Previously only "UEFIx86-64 (Torito C Library)" and "WinNTx86-64 (Torito C Library)"
  For varification projects can be linked against the original Microsoft C library and the Torito C Library
* Update WindowsTargetPlatformVersion to  10.0.17134.0 from 10.0.16299.0
### 20181129/R116
* Update to torito-C 20181129/R116 that provides various improvements
### 20181112/R110
* Update to torito-C 20181112/R110 that provides scanf-family improvements
### 20180921/R107
* Update to torito-C 20180920/R106 that provides VS2017/15.8 support
* cleanup project settings, remove default C library from solution/project view
### 20180830/R102
* Update to torito-C 20180830/R102
### 20180817
* add UDK2018 MdePkg includes to enable acces to UEFI API
* fixed structure alignment to default / 8 byte alignment
### 20180717/R101
* Update to torito-C 20180717/R101
### 2018041/R95
* Update to torito-C 20180411/R95
### 20180130/R86
* Update to torito-C 20180130/R86
### 20180108/R85
* Update to torito-C 20180108/R85
### 20171105/R78
* initial revision

