rem @echo off
for /f %%i in ('git rev-parse --abbrev-ref HEAD') do set branch=%%i
git pull -f origin %branch%
git submodule update --init --recursive
