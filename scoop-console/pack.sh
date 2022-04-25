#! bash -uvx
set -e
mingwx mk.mgw scoop-console
cp -p scoop-console-x86_64-static.exe ../cmd/scoop-console.exe
