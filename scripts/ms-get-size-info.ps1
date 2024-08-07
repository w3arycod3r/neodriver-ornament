# File:    ms-get-size-info.ps1
# Author:  Garrett Carter
# Purpose: Generate symbol size info for elf file passed in via params
param (
    [Parameter(Mandatory)]
    [string]
    $elf_to_parse,
    [Parameter(Mandatory)]
    [string]
    $output_text_file
)

$ErrorActionPreference = 'Stop'
if (-Not (Get-Command atprogram.exe -ErrorAction SilentlyContinue)) {
    Write-Host "Adding Atmel Studio dirs to process PATH variable..."
    $Env:PATH = "C:\Program Files (x86)\Atmel\Studio\7.0\;C:\Program Files (x86)\Atmel\Studio\7.0\shellutils\;C:\Program Files (x86)\Atmel\Studio\7.0\atbackend\;C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\;$Env:PATH"
}
$elf_to_parse = [System.IO.Path]::GetFullPath($elf_to_parse)
$output_text_file = [System.IO.Path]::GetFullPath($output_text_file)

Write-Host "Beginning to parse $elf_to_parse ..."

if (Test-Path $output_text_file) {
    Write-Host "Deleting $output_text_file ..."
    Remove-Item -Force $output_text_file
}

Write-Host "Writing code size analysis info to $output_text_file ..."

&{
    Write-Host "Last generated: $(Get-Date)"
    Write-Host
    Write-Host "********************************"
    Write-Host "SYMBOL SIZE LISTING"
    Write-Host "Size (Decimal) | Sym Type | Sym Name"
    Write-Host "********************************"
    Write-Host
    avr-nm --size-sort -Crtd $elf_to_parse
    Write-Host
    Write-Host "********************************"
    Write-Host "SYMBOL ADDRESS LISTING"
    Write-Host "Address (HEX) | Sym Type | Sym Name"
    Write-Host "********************************"
    Write-Host
    avr-nm -C -n $elf_to_parse
    Write-Host
    Write-Host
    Write-Host "********************************"
    Write-Host "CODE DISASSEMBLY"
    Write-Host "********************************"
    avr-objdump -C -d $elf_to_parse
} *> $output_text_file

Exit 0
