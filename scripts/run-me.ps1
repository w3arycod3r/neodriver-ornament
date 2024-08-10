# File:    run-me.ps1
# Author:  Garrett Carter
# Purpose: Alias for ms-release-sequence.ps1

$ErrorActionPreference = 'Stop'

# Allow the other scripts (downloaded from web, unsigned) to run
# Running this script initially will likely trigger a security pop-up.
Set-ExecutionPolicy -ExecutionPolicy Bypass -Scope Process

& $PSScriptRoot\ms-release-sequence.ps1

Exit 0