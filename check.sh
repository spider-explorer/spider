#! bash
set -e
#clear
cmd /c cls
astyle --style=allman --recursive  *.java,*.c,*.cpp,*.cxx,*.h,*.hpp,*.hxx >/dev/null 2>&1
for f in *.cpp; 
do
  #echo "Processing $f file..";
  if output=$( { cppcheck --enable=all --error-exitcode=1 --inline-suppr --suppressions-list=check-suppressions.txt $f; } 2>&1 ) ; then
  #if output=$( { cppcheck --enable=all --error-exitcode=1 --inline-suppr --suppress=*:3rdparty/* $f; } 2>&1 ) ; then
    #echo "Command succeeded"
    output=""
  else
    #echo "Command failed"
    echo "$output"
    exit $?
  fi
done
