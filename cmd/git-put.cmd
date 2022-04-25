rem @echo off
for /f %%i in ('git rev-parse --abbrev-ref HEAD') do set branch=%%i
git commit -a -m.
git push origin HEAD:%branch%
git status
