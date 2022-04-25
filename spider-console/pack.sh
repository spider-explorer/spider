#! bash -uvx
set -e
mingwx mk.mgw spider-console
cp -p spider-console-x86_64-static.exe ../cmd/spider-console.exe
