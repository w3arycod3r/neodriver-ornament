@REM File:    build.bat
@REM Author:  Garrett Carter
@REM Purpose: arduino-cli build script for ATtiny85 and Atmel-ICE programmer

@echo off

set board_name=ATTinyCore:avr:attinyx5:LTO=enable,TimerClockSource=default,chip=85,clock=8internal,eesave=aenable,bod=disable,millis=enabled
set prog_name=atmel_ice

arduino-cli compile --libraries ../libraries --fqbn %board_name% -e && ^
get_size_info.bat && ^
arduino-cli upload --fqbn %board_name% -v -P %prog_name%