# Week 44 — Collaborative Development and Code Review

📖 **[Lecture notes](https://sercankulcu.github.io/files/c/Week_44_Collaborative_Development_and_Code_Review.html)** — read these first; the code here is the worked example from that page.

## Build and run

```bash
make
make run
```

## Experiments from the notes

These are the commands the lecture notes ask you to run — including the ones that
are supposed to fail.

```bash
git switch -c fix-config-leak      # branch and switch (modern spelling)
# .. work, commit ..
git push -u origin fix-config-leak
```

```bash
git switch fix-config-leak
git fetch origin
git merge origin/main             # preserves history; adds a merge commit
# or
git rebase origin/main            # replays your commits; linear history
```

```bash
git rebase -i origin/main         # squash "fix typo" into its parent
git log --oneline origin/main..   # review what you are about to submit
```
