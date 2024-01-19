@REM File:    build_eep_and_app.bat
@REM Author:  Garrett Carter
@REM Purpose: Build and flash eep_data_write, followed by main app

@echo off

pushd "%~dp0"

echo.
echo Building eep_data_write...
echo.
call eep_data_write\build.bat || goto :error

:: Give time for eep flashing
echo.
echo Flashing EEPROM...
echo.
timeout /t 3 /nobreak > NUL

echo.
echo Building neo_driver_app...
echo.
call neo_driver_app\build.bat || goto :error

:error
popd