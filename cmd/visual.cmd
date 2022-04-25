@echo off
where vim.exe > nul 2>&1
if %ERRORLEVEL% neq 0 (
  call scoop bucket add main
  call scoop install vim
)
vim.exe %*
