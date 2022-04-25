#! bash -uvx
set -e
mingwx mk.mgw git-console
cp -p git-console-x86_64-static.exe ../cmd/git-console.exe
