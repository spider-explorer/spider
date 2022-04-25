#! bash -uvx
set -e
cwd=`pwd`
killall boot-x86_64-static || true
sleep 3
cp -p boot-x86_64-static.exe ~/../
rm -rf *.tmp
rm -rf common
cp -rp ~/qt/common .
find .  -maxdepth 2 -iname *.h -o -iname *.cpp | xargs uncrustify -l CPP --replace --no-backup
#astyle --recursive --ascii --style=allman --suffix=none *.java,*.c,*.cpp,*.cxx,*.h,*.hpp,*.hxx | sed -e "/Unchanged  /d"
ts=`date "+%Y.%m.%d.%H.%M.%S"`
echo $ts > main/spider2.version.txt
UNAME=`uname`
mkdir -p upload.tmp/spider-release
7z a -tzip upload.tmp/spider-release/spider-v$ts.zip *-static.exe *-static.dll scoop-sw-list.ini scoop-bucket-map.json cmd temp -x!temp/*
sha256sum upload.tmp/spider-release/spider-v$ts.zip
sum1=`sha256sum upload.tmp/spider-release/spider-v$ts.zip | awk '{print $1}'`
#sum1=`sha256-x86_64-static.exe upload.tmp/spider-release/spider-v$ts.zip`
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
        "https://github.com/spider-explorer/spider/releases/download/v$ts/spider-v$ts.zip"
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
echo $GITHUB_ALL | gh auth login --with-token
cp spider.json upload.tmp/spider-release/spider-v$ts.json
git add .
git commit -m"Spider Explorer v$ts"
git tag -a v$ts -mv$ts
git push origin v$ts
git push
gh release list | sed 's/|/ /' | awk '{print $1, $8}' | while read -r line; do gh release delete -y "$line"; done
gh release create v$ts "upload.tmp/spider-release/spider-v$ts.zip" --generate-notes --target main
