@echo off
echo clean up...
for /F %%d in ('dir /s /b *.vcxproj') do (
    if exist %%~pd\x64 rd /s /q %%~pd\x64
)
del /s *.ilk *.map *.pdb
if "%1" == "/all" if exist x64 rd /s /q x64
