#! bash
if [ ! -e ".git" ]; then
  echo "[Not git dir]"
  exit 0
fi
if [ "$1" == "--full" ]; then
  git fetch &> /dev/null
fi
if [ $(git --no-optional-locks status --short | wc -c) -ne 0 ]; then
  echo "[Local change]"
  git status --short
  #exit 0
fi
if [ $(git --no-optional-locks diff --summary origin/HEAD | wc -c) -ne 0 ]; then
  echo "[Remote Change]"
  git diff --summary origin/HEAD
  #exit 0
fi
#echo "no-change"

