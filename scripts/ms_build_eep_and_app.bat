@REM File:    ms_build_eep_and_app.bat
@REM Author:  Garrett Carter
@REM Purpose: Build and flash eep_data_write, followed by neo_driver_app

@echo off

pushd "%~dp0"

echo **********************************************************
echo Building eep_data_write...
echo **********************************************************
call ms_build_flash_config.bat eep_data_write eep_data_write.elf clean || goto :error

echo **********************************************************
echo Waiting for eep_data_write to execute...
echo **********************************************************
timeout /t 3 /nobreak > NUL

echo **********************************************************
echo Building neo_driver_app...
echo **********************************************************
call ms_build_flash_config.bat Release neo_driver_app.elf clean || goto :error

:error
popd