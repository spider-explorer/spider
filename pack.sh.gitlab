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
7z a -t7z upload.tmp/spider-release/spider-$ts.7z *-static.exe *-static.dll scoop-sw-list.ini scoop-bucket-map.json cmd spiderbrowser temp -x!temp/*
sha256sum upload.tmp/spider-release/spider-$ts.7z
#sum1=`./sha256sum upload.tmp/spider-release/spider-$ts.7z | awk '{print $1}'`
sum1=`./sha256-x86_64-static.exe upload.tmp/spider-release/spider-$ts.7z`
echo $sum1
cat << EOS > spider.json
{
    "version": "$ts",
    "description": "",
    "homepage": "",
    "license": "MIT",
    "depends": [
    ],
    "url": [
        "https://gitlab.com/javacommons/spider-release/-/raw/main/spider-$ts.7z"
    ],
    "hash": [
        "$sum1"
    ],
    "bin": [
        [
            "boot-x86_64-static.exe",
            "spider",
            "--dumy"
        ]
    ],
    "shortcuts": [
        [
            "boot-x86_64-static.exe",
            "Spider",
            "--dummy"
        ]
    ],
    "persist": "temp"
}
EOS
cp spider.json upload.tmp/spider-release/spider-$ts.json
./gitlab-console-x86_64-static.exe --project javacommons/spider-release --action upload spider.json upload.tmp/spider-release/spider-$ts.7z upload.tmp/spider-release/spider-$ts.json
git add .
git commit -m"Spider Explorer v$ts"
git tag -a v$ts -mv$ts
git push origin v$ts
git push
