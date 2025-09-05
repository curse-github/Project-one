@echo off
make ./out/app.exe
if errorlevel 1 GOTO error
robocopy Resources out/Resources /E /NFL /NDL /NJH /NJS /nc /ns /np
GOTO noerror
:error
pause
:noerror