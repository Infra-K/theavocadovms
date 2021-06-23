for /f "tokens=3 usebackq" %%G in (include\config\confver.hpp) do set VE_VER=%%G
echo %VE_VER%
set VE_VER_PHASE=release

mkdir Avocadovms-%VE_VER%-win64-%VE_VER_PHASE%
REM copy dir from client and vecvr
xcopy /y /e client\prj_win32\x64\Release  Avocadovms-%VE_VER%-win64-%VE_VER_PHASE%
xcopy /y /e vecvr\prj_win32\x64\Release  Avocadovms-%VE_VER%-win64-%VE_VER_PHASE%

REM cooy service file
xcopy /y /e output\windows\nssm\win64\nssm.exe  Avocadovms-%VE_VER%-win64-%VE_VER_PHASE%
xcopy /y output\windows\gencertificate.bat  Avocadovms-%VE_VER%-win64-%VE_VER_PHASE%
xcopy /y output\windows\regservice64.bat  Avocadovms-%VE_VER%-win64-%VE_VER_PHASE%
xcopy /y output\windows\unregservice64.bat  Avocadovms-%VE_VER%-win64-%VE_VER_PHASE%

del  Avocadovms-%VE_VER%-win64-%VE_VER_PHASE%\*.pdb /f /s /q
del  Avocadovms-%VE_VER%-win64-%VE_VER_PHASE%\*.lik /f /s /q
del  Avocadovms-%VE_VER%-win64-%VE_VER_PHASE%\*.lib /f /s /q
del  Avocadovms-%VE_VER%-win64-%VE_VER_PHASE%\*.exp /f /s /q
