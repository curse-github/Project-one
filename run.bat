@echo off
make ./out/app.exe
if errorlevel 1 GOTO error
cd out
start /WAIT /B app.exe
if not errorlevel 0 GOTO error
GOTO noerror
:error
pause
:noerror
cd ..
make clean