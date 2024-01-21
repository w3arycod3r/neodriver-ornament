@REM File:    get_size_info.bat
@REM Author:  Garrett Carter
@REM Purpose: Generate size listing and disassembly of the .elf executable

@echo off

set in_elf_file=build\ATTinyCore.avr.attinyx5\neo_driver_app.ino.elf
set out_txt_file=build\size_info.txt

@REM rm -f %out_txt_file%
del /Q %out_txt_file%

>>%out_txt_file% (

    echo Last compiled:   %date%   %time%
    echo.

    echo ********************************
    echo SYMBOL SIZE LISTING
    echo Size ^(HEX^) ^| Sym Type ^| Sym Name
    echo ********************************
    echo.
    avr-nm --size-sort -C -r %in_elf_file%
    echo.

    echo ********************************
    echo SYMBOL ADDRESS LISTING
    echo Address ^(HEX^) ^| Sym Type ^| Sym Name
    echo ********************************
    echo.
    avr-nm -C -n %in_elf_file%
    echo.
    
    echo.
    echo ********************************
    echo CODE DISASSEMBLY
    echo ********************************
    avr-objdump -C -d %in_elf_file%

)

