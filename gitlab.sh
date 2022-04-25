#! bash -uvx
#! bash -uvx
set -e
cwd=`pwd`
killall boot-x86_64-static || true
sleep 3
cp -p boot-x86_64-static.exe ~/../
#lazbuild.exe project1.lpr
rm -rf *.tmp
rm -rf common
cp -rp ~/qt/common .
find .  -maxdepth 2 -iname *.h -o -iname *.cpp | xargs uncrustify -l CPP --replace --no-backup
#astyle --recursive --ascii --style=allman --suffix=none *.java,*.c,*.cpp,*.cxx,*.h,*.hpp,*.hxx | sed -e "/Unchanged  /d"
ts=`date "+%Y.%m.%d.%H.%M.%S"`
echo $ts > main/spider2.version.txt
UNAME=`uname`
mkdir -p upload.tmp/spider-release
7z a -tzip upload.tmp/spider-release/spider-$ts.zip *-static.exe *-static.dll scoop-sw-list.ini scoop-bucket-map.json cmd spiderbrowser temp -x!temp/*
sha256sum upload.tmp/spider-release/spider-$ts.zip
#sum1=`./sha256sum upload.tmp/spider-release/spider-$ts.zip | awk '{print $1}'`
sum1=`./sha256-x86_64-static.exe upload.tmp/spider-release/spider-$ts.zip`
echo $sum1

echo $GITLAB_READ_WRITE | glab auth login --stdin
glab release create v$ts upload.tmp/spider-release/spider-$ts.zip --name "v$ts" --notes "v$ts"