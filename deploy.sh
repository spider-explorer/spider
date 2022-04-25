mkdynamic.sh spider
if [ $? -ne 0 ]; then
  exit $?
fi
rm -rf vc_redist.x64.exe
exe=`ls *.exe`
echo $exe
rm -rf translations iconengines imageformats platforms styles
windeployqt $exe
if [ -f "vc_redist.x64.exe" ]; then
  scoop bucket add main
  scoop install wixtoolset
  dark -x vc_redist.x64.tmp vc_redist.x64.exe
  7z x -y vc_redist.x64.tmp/AttachedContainer/packages/vcRuntimeMinimum_amd64/cab1.cab
  #7z x -y vc_redist.x64.tmp/AttachedContainer/packages/vcRuntimeAdditional_amd64/cab1.cab
  rm -rf vc_redist.x64.exe
fi
rm -rf *.tmp
rm -rf *.user
