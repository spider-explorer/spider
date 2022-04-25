@rem https://stackoverflow.com/questions/13716658/how-to-delete-all-commit-history-in-github
@rem https://stackoverflow.com/questions/19542301/delete-all-tags-from-a-git-repository
for /f %%i in ('git rev-parse --abbrev-ref HEAD') do set branch=%%i
git tag -l | xargs -n 1 git push --delete origin
git tag | xargs git tag -d
git checkout --orphan latest_branch
git add -A
git commit -am "Initial commit"
git branch -D %branch%
git branch -m %branch%
git push -f origin %branch%
git push --set-upstream origin %branch%
