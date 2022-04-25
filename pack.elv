rm -rf build-spider.tmp
astyle --style=allman --recursive  "*.java,*.c,*.cpp,*.cxx,*.h,*.hpp,*.hxx"
sleep 3
var ts = (date "+%Y.%m.%d.%H.%M.%S")
echo $ts > spider.version.txt
mingwx.cmd mk.mgw spider
rm -rf "spider-*.7z"
cp -rp rapidee.* cmd/
7z a -t7z spider-$ts.7z spider-x86_64-static.exe wt-* sqlitestudio-* FolderSizePortable-* scoop-sw-list.ini cmd temp "-x!temp/*"
sha256sum spider-$ts.7z
var sum1 = (sha256sum spider-$ts.7z | awk '{print $1}')
echo $sum1
sed -e "s/<ts>/"$ts"/g" -e "s/<sum1>/"$sum1"/g" pack.json.template > spider.json
git add .
git commit -m.
git push
