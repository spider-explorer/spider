#! bash -uvx
set -e
cwd=`pwd`
lazbuild.exe project1.lpr
rm -rf build-spider2.tmp
if [ $# -eq 1 ] && [ "$1" == "--full" ]; then
  clang-format -i *.cpp *.h
fi
astyle --ascii --style=allman --suffix=none *.java,*.c,*.cpp,*.cxx,*.h,*.hpp,*.hxx | sed -e "/Unchanged  /d"
sleep 10
ts=`date "+%Y.%m.%d.%H.%M.%S"`
echo $ts > main/spider2.version.txt
UNAME=`uname`
#mingwx.cmd mk.mgw spider3
cd main
astyle --ascii --style=allman --suffix=none *.java,*.c,*.cpp,*.cxx,*.h,*.hpp,*.hxx | sed -e "/Unchanged  /d"
mingwx.cmd mk.mgw main
cd $cwd
cd boot
astyle --ascii --style=allman --suffix=none *.java,*.c,*.cpp,*.cxx,*.h,*.hpp,*.hxx | sed -e "/Unchanged  /d"
mingwx.cmd mk.mgw boot
cd $cwd
if [ $# -eq 1 ] && [ "$1" == "--full" ]; then
  cd git-console
  astyle --ascii --style=allman --suffix=none *.java,*.c,*.cpp,*.cxx,*.h,*.hpp,*.hxx | sed -e "/Unchanged  /d"
  ./pack.sh
  cd $cwd
  cd gitlab-console
  astyle --ascii --style=allman --suffix=none *.java,*.c,*.cpp,*.cxx,*.h,*.hpp,*.hxx | sed -e "/Unchanged  /d"
  ./pack.sh
  cd $cwd
  cd scoop-console
  astyle --ascii --style=allman --suffix=none *.java,*.c,*.cpp,*.cxx,*.h,*.hpp,*.hxx | sed -e "/Unchanged  /d"
  ./pack.sh
  cd $cwd
  cd spider-console
  astyle --ascii --style=allman --suffix=none *.java,*.c,*.cpp,*.cxx,*.h,*.hpp,*.hxx | sed -e "/Unchanged  /d"
  ./pack.sh
  cd $cwd
fi
mkdir -p upload.tmp/spider-release
7z a -t7z upload.tmp/spider-release/spider2-$ts.7z main boot project1.exe scoop-sw-list.ini scoop-bucket-map.json vscode_16x16 cmd temp -x!temp/*
sha256sum upload.tmp/spider-release/spider2-$ts.7z
sum1=`sha256sum upload.tmp/spider-release/spider2-$ts.7z | awk '{print $1}'`
echo $sum1
cat << EOS > spider2.json
{
    "version": "$ts",
    "description": "",
    "homepage": "",
    "license": "MIT",
    "depends": [
        "main/7zip",
        "main/git",
        "main/vim",
        "main/llvm",
        "main/mono",
        "main/gcc",
        "main/make",
        "main/cmake",
        "main/python",
        "main/jq",
        "main/unzip",
        "main/zip",
        "main/file",
        "main/astyle",
        "main/nyagos"
    ],
    url: [
        "https://gitlab.com/javacommons/spider-release/-/raw/main/spider2-$ts.7z"
    ],
    "hash": [
        "$sum1"
    ],
    "bin": [
        [
            "boot/boot-x86_64-static.exe",
            "spider2",
            "--dumy"
        ]
    ],
    "shortcuts": [
        [
            "boot/boot-x86_64-static.exe",
            "Spider2",
            "--dummy"
        ],
        [
            "project1.exe",
            "Project1",
            "--dummy"
        ]
    ],
    "persist": "temp"
}
EOS
cp spider2.json upload.tmp/spider-release/spider2-$ts.json
gitlab-console.exe --project javacommons/spider-release --action upload upload.tmp/spider-release/spider2-$ts.7z upload.tmp/spider-release/spider2-$ts.json
git add .
git commit -m"Spider2 v$ts"
git tag -a v$ts -mv$ts
git push origin v$ts
git push
