#! bash -uvx
function processApp() {
  cwd=$1
  app=$2
  path_spec=$3
  cd $cwd
  scoop install $app
  scoop update $app
  version=`scoop-console-x86_64-static.exe --latest $app | awk '{print $1}'`
  echo $version
  path=`scoop-console-x86_64-static.exe --latest $app | awk '{print $2}'`
  echo $path
  exists=`$cwd/gitlab-console-x86_64-static.exe --action exists --project spider-explorer/spider-software .software/$app/$app-$version.7z`
  url=https://gitlab.com/spider-explorer/spider-software/-/raw/main/.software/$app/$app-$version.7z
  json=`echo "$json" | jq -r --arg app "$app" --arg version "$version" --arg path_spec "$path_spec" --arg url "$url" '.software[$app] = {"version":$version, "ext":"7z", "path":$path_spec, "url":$url}'`
  if [ "$exists" == "true" ]; then
    return 0
  fi
  if [ -e "$path" ]; then
    echo exist
    cd $cwd
    mkdir -p .software/$app
    cd .software/$app
    cd $path
    7z.exe a -r $cwd/.software/$app/$app-$version.7z * -x!"User Data"
    $cwd/gitlab-console-x86_64-static.exe --action upload --project spider-explorer/spider-software $cwd/.software/$app/$app-$version.7z --path .software/$app/
  fi
}
cwd=`pwd`
scoop install git
scoop bucket add main
scoop bucket add extras
json=`cat ./investigate.json`
processApp $cwd gh "/bin"
processApp $cwd googlechrome "."
processApp $cwd wixtoolset "."
processApp $cwd uncrustify "/bin"
processApp $cwd geany "/bin"
processApp $cwd deno "."
processApp $cwd vscode "/bin;."
processApp $cwd cmake "/bin"
processApp $cwd premake "."
processApp $cwd make "/bin"
processApp $cwd file "."
processApp $cwd astyle "/bin"
processApp $cwd 7zip "."
processApp $cwd git "/cmd"
processApp $cwd nyagos "."
processApp $cwd jq "."
processApp $cwd windows-terminal "."
processApp $cwd sqlitestudio "."
processApp $cwd rapidee "."
processApp $cwd notepad3 "."
processApp $cwd smartgit "/bin"
processApp $cwd wget "."
processApp $cwd curl "/bin"
processApp $cwd qownnotes "."
processApp $cwd sed "."
echo "$json" > spider-software.json
dos2unix spider-software.json
$cwd/gitlab-console-x86_64-static.exe --action upload --project spider-explorer/spider-software spider-software.json
