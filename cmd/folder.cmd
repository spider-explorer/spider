@echo off
where notepad++.exe > nul 2>&1
if %ERRORLEVEL% neq 0 (
  call scoop bucket add extras
  call scoop install notepadplusplus
)
start %USERPROFILE%\scoop\apps\notepadplusplus\current\notepad++.exe -multiInst -openFoldersAsWorkspace %*
