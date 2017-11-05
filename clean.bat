@echo off
echo clean up...
if exist x64 rd /s /q x64
for /D %%d in (welcome*) do if exist %%d\x64 rd /s /q %%d\x64

