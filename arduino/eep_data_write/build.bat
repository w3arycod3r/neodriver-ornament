@REM File:    build.bat
@REM Author:  Garrett Carter
@REM Purpose: arduino-cli build script for ATtiny85 and Atmel-ICE programmer

@echo off

pushd "%~dp0"

set board_name=ATTinyCore:avr:attinyx5:LTO=enable,TimerClockSource=default,chip=85,clock=8internal,eesave=aenable,bod=disable,millis=enabled
set prog_name=atmel_ice

call arduino-cli compile --libraries ../libraries --fqbn %board_name% -e || goto :error
call arduino-cli upload --fqbn %board_name% -v -P %prog_name% || goto :error

:error
popd
if %errorlevel% neq 0 exit /b %errorlevel%