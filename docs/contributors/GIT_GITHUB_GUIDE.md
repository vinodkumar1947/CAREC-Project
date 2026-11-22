\# Git \& GitHub Guide for First-Time Contributors



Welcome! This guide walks you through the full contribution workflow: clone → branch → commit → push → pull request → review updates → resolving conflicts → squash merge. Steps are given both via the \*\*command line\*\* and \*\*GitHub Desktop\*\*.



\---



\## 1. Clone the repository



Cloning downloads a full copy of the repo, including its history, to your machine.



\*\*Command line\*\*

```bash

git clone https://github.com/<your-github-name>/CAREC-Project.git

cd CAREC-Project

```



\*\*GitHub Desktop\*\*

1\. Click \*\*File → Clone Repository\*\*.

2\. Select the repo from the list (or paste the URL under the "URL" tab).

3\. Choose a local folder and click \*\*Clone\*\*.



\---



\## 2. Create a branch



Never commit directly to `main`. Create a new branch for each piece of work.



\### Branch naming convention



This project (CAREC) uses the pattern:



```

docs/<issue-number>-<your-github-name>

```



For example, this guide's own branch is named `docs/19-Sanjanabarge` (issue #19, contributor `Sanjanabarge`). Use the same pattern for other workstreams — swap the `docs/` prefix for the relevant area if your issue calls for it (e.g. `scenario/`, `tooling/`), but always keep `<issue-number>-<your-github-name>` after it.



\*\*Command line\*\*

```bash

git checkout -b docs/<issue-number>-<your-github-name>

```



\*\*GitHub Desktop\*\*

1\. Click the \*\*Current Branch\*\* dropdown.

2\. Click \*\*New Branch\*\*.

3\. Enter the branch name as `docs/<issue-number>-<your-github-name>` and click \*\*Create Branch\*\*.



\---



\## 3. Make changes and commit



Commit small, logical chunks of work with clear messages.



\*\*Command line\*\*

```bash

git add <file-or-folder>       # or: git add .

git commit -m "Short, clear summary of the change"

```



\*\*GitHub Desktop\*\*

1\. Your edited files appear automatically in the \*\*Changes\*\* tab.

2\. Check the boxes next to the files you want to include.

3\. Enter a summary (and optional description) at the bottom left.

4\. Click \*\*Commit to <branch-name>\*\*.



\*\*Good commit message tips\*\*

\- Use the imperative mood: "Add guide" not "Added guide."

\- Keep the summary under \~50 characters; use the description field for details.

\- One logical change per commit — don't mix unrelated edits.



\---



\## 4. Push your branch



Pushing uploads your local commits to GitHub.



\*\*Command line\*\*

```bash

git push -u origin <branch-name>

```

(After the first push, `git push` alone is enough.)



\*\*GitHub Desktop\*\*

1\. Click \*\*Push origin\*\* in the top toolbar.

2\. The first push will prompt to also \*\*publish the branch\*\* — confirm.



\---



\## 5. Open a pull request (PR)



\*\*Command line / browser\*\*

1\. Go to the repo on GitHub — you'll usually see a banner: \*\*"Compare \& pull request."\*\* Click it.

2\. If not, go to \*\*Pull requests → New pull request\*\*, choose `main` as base and your branch as compare.

3\. Fill in a title and description (what changed and why; link the issue, e.g. `Closes #19`).

4\. Click \*\*Create pull request\*\*.



\*\*GitHub Desktop\*\*

1\. Click \*\*Branch → Create Pull Request\*\*, or the \*\*Create Pull Request\*\* button after pushing.

2\. This opens your browser with the PR form pre-filled — complete and submit it there.



\---



\## 6. Update your PR after a review



Reviewers often ask for changes. You keep working on the \*\*same branch\*\* — new commits automatically appear on the open PR.



\*\*Command line\*\*

```bash

\# make the requested edits, then:

git add <changed-files>

git commit -m "Address review feedback: <what changed>"

git push

```



\*\*GitHub Desktop\*\*

1\. Make your edits.

2\. Commit them (Step 3) and push (Step 4) as before — no new PR needed.



Reply to review comments on GitHub, and click \*\*Resolve conversation\*\* once addressed.



\---



\## 7. Resolving conflicts (safely, without destructive commands)



A conflict happens when `main` and your branch changed the same lines. Here's how to fix it without losing work.



\*\*Command line\*\*

```bash

git checkout main

git pull                     # get the latest main

git checkout <branch-name>

git merge main                # bring main's changes into your branch

```
Git marks conflicting sections in a file with special conflict markers. For example, the opening marker identifies your current version as `HEAD`, the separator divides the two versions, and the closing marker identifies the incoming branch.

The section between these markers contains your version and the incoming version. Edit the file to keep the correct final content, and remove all conflict markers before committing.

1\. Open each conflicted file and edit it to the correct final content (remove the `<<<<<<<`, `=======`, `>>>>>>>` markers).

2\. Save the file, then:

```bash

git add <resolved-file>

git commit               # completes the merge

git push

```



\*\*GitHub Desktop\*\*

1\. GitHub Desktop will flag conflicted files after a \*\*Branch → Update from main\*\*.

2\. Click \*\*Open in \[your editor]\*\* next to each conflicted file, resolve it the same way as above, and save.

3\. Back in GitHub Desktop, mark the file as resolved and click \*\*Continue Merge\*\*, then \*\*Push origin\*\*.



\*\*If you get stuck:\*\* you can always abort a merge that's gone wrong and start over — this is non-destructive to your existing commits:

```bash

git merge --abort

```

Ask a maintainer for help if a conflict looks confusing. Avoid `git reset --hard` or force-pushing unless a maintainer specifically tells you to — those can discard work.



\---



\## 8. Squash merge (done by a maintainer)



Once your PR is approved, a maintainer will usually \*\*squash and merge\*\* it — combining all your commits into a single, clean commit on `main`.



\- On the PR page, they'll click the dropdown next to \*\*Merge pull request\*\* and choose \*\*Squash and merge\*\*.

\- You don't need to do anything extra — just make sure your branch is up to date and all conversations are resolved.

\- After merging, you can delete your branch (GitHub will offer a \*\*Delete branch\*\* button, or delete it locally with `git branch -d <branch-name>`).



\---



\## Quick reference



| Step | Command line | GitHub Desktop |

|---|---|---|

| Clone | `git clone <url>` | File → Clone Repository |

| Branch | `git checkout -b docs/<issue-number>-<github-name>` | Current Branch → New Branch |

| Commit | `git add . \&\& git commit -m "..."` | Check files → write summary → Commit |

| Push | `git push -u origin <name>` | Push origin |

| PR | Open via GitHub banner | Branch → Create Pull Request |

| Update PR | commit + push again | commit + push again |

| Conflicts | `git merge main`, edit, `git add`, `git commit` | Update from main → resolve in editor → Continue Merge |

| Merge | (maintainer) Squash and merge | (maintainer) Squash and merge |



\---



\## Acceptance criteria checklist (for this issue)



\- \[x] Includes command-line steps

\- \[x] Includes GitHub Desktop alternative

\- \[x] Uses the CAREC branch naming convention (`docs/<issue-number>-<github-name>`)

\- \[x] Explains how to recover from common mistakes without destructive commands

\- \[ ] Tested by one contributor unfamiliar with Git — \*\*hand this to a newcomer and get their feedback\*\*
