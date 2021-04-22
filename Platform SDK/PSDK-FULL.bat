@echo off
IF "%OS%" == "Windows_NT" setlocal

REM We need a targetdir
if "x%1"=="x" goto echousage
set _targetdir=%1

REM call extract.exe to extract CAB chain starting with PSDK-FULL.1.cab
call extract.exe /A /E /Y PSDK-FULL.1.cab /L %_targetdir%

goto end


:echousage
Echo.
Echo Usage: %0 ^<TargetDir^>
Echo.
Echo Example: %0 c:\temp
Echo.
goto end


:end
IF "%OS%" == "Windows_NT" endlocal