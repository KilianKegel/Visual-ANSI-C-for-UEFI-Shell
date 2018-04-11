@echo off
echo clean up...
if exist x64 del /s x64\*.pdb
for /D %%d in (argcv welcome*) do if exist %%d\x64 rd /s /q %%d\x64

