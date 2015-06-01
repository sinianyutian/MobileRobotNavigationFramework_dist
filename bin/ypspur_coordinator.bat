@rem ypspur-coordinator.exe -p TEC-1.param -d \\.\COM21 --high-resolution
set "FILE=COM21"
@echo off
for /f "delims=" %%I in ('start /min /b /wait MSHTA.EXE vbscript:Execute("CreateObject(""Scripting.FileSystemObject"").GetStandardStream(1).Write InputBox( ""Input COM Number of T-Frog. You can find the number with Device Manager"", ""Input"" ,""%FILE%""):close"^)') do ypspur-coordinator.exe -p TEC-1.param -d \\.\%%I --high-resolution
