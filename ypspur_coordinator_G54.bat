@rem ypspur-coordinator.exe -p robot_G54.param -d \\.\COM1 --high-resolution
set "FILE=COM16"
@echo off
for /f "delims=" %%I in ('start /min /b /wait MSHTA.EXE vbscript:Execute("CreateObject(""Scripting.FileSystemObject"").GetStandardStream(1).Write InputBox( ""Input COM Number of T-Frog. You can find the number with Device Manager"", ""Input"" ,""%FILE%""):close"^)') do bin\ypspur-coordinator.exe -p bin\robot_G54A.param -d \\.\%%I --high-resolution
