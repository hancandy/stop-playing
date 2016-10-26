# stop-playing

## Fetching this project
```
cd /your/folder
git clone https://github.com/mrzapp/stop-playing.git
cd ./stop-playing
```

## Committing work
```
git add --all .
git commit -m "your push message"
git pull
git push
```

## When a merge error happens
Check which files are conflicted
```
CONFLICT (content): Merge conflict in path/to/your.file
Automatic merge failed; fix conflicts and then commit the result.
```
If you want to overwrite your own conflicted file with the remote file
```
git checkout path/to/your.file --theirs
```

If you want to keep your own file
```
git checkout path/to/your.file --ours
```

Always remember to commit after you fixed a merge error
