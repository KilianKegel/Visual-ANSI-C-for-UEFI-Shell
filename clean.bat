@echo off
echo clean up...
if exist x64 del /s x64\*.pdb
if exist x64 del /s x64\*.map
if exist x64 del /s x64\*.ilk
for /D %%d in (argcv wargcv welcome*) do if exist %%d\x64 rd /s /q %%d\x64

