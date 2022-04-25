@echo off
:loop
start notepad3.exe %1
shift
if not "%~1"=="" goto loop
