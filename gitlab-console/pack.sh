#! bash -uvx
set -e
mingwx mk.mgw gitlab-console
cp -p gitlab-console-x86_64-static.exe ../cmd/gitlab-console.exe
