# Git flow
- [Git flow](#git-flow)
  - [ref:](#ref)
  - [The main branches](#the-main-branches)
  - [Supporting branches](#supporting-branches)
    - [Feature branches](#feature-branches)
      - [Creating a feature branch](#creating-a-feature-branch)
      - [Incorporating a finished feature on develop](#incorporating-a-finished-feature-on-develop)
    - [Release branches](#release-branches)
      - [Creating a release branch](#creating-a-release-branch)

## ref: 
- https://www.jianshu.com/p/104fa8b15d1e
- [a successful git branching model](https://nvie.com/posts/a-successful-git-branching-model/)
- [git_flow](https://github.com/nvie/gitflow)

![Overview](../Pic/git/Overview.png)
## The main branches
![main_branches](../Pic/git/main-branches.png)


the central repo holds two main branches with an infinite lifetime:
- `Master`: we consider origin/master to be the main branch where the souce code of `HEAD` always reflects a production-ready state.
- `Develop`: we consider origin/develop to be the main branch where the source code of `HEAD` always reflects a state with the latest delivered development changes for teh next release.

Therefore, each time when changes are merged back into `master`, this is a new production release by `definition`. 
Then we could use a Git hook script to automatically build and roll-out our software to our production servers everytime there was a commit on `master`.

## Supporting branches
Development model uses a variety of supporting branches to aid parallel development between team members, ease tracking of features, prepare for production releases and to assist in quickly fixing live production problems.
Unlike the main branches, these branches always have a limited life time, since they will be removed eventually.
- Feature branches
- Release branches
- Hotfix branches

### Feature branches
![feature_branches](../Pic/git/feature_branches.png)

May branch off from `develop`.
must merge back into `develop`,

Feature branches are used to develop new features for the upcoming or a distant future release.
The essence of a feature branch is that it exists as long as the feature is in development,
but will be merged back into `develop`( to definitely add the new feature to the upcoming release) or discareded (in case of a disappointing experiment).

Feature branches typically exist in developer repos only, not in `origin`.

#### Creating a feature branch
branch off from the `develop` branch
```git
git checkout -b myfeature develop
```
#### Incorporating a finished feature on develop
Finished features may be merged into the `develop` branch to definitely add them to the upcoming release:
```git
git checkout develop # Switched to branch 'develop'
git merge --no-ff myfeature # Updating ea1b82a..05e9557
(Summary of changes)
git branch -d myfeature # Deleted branch myfeature (was 05e9557)
git push origin develop
```

The `--no-ff` flag causes the merge to alwyas create a new commit object, even if the merge could be performed with a fast-forward.
This avoids losing information about the historical existence of a feature branch and groups together all commits that together added the feature.
![](../Pic/git/merge-without-ff.png)

In the latter case, it is impossible to see from the Git history which of the commit objects together have implemented a feature - you would have to manually read all the log messages.
Reverting a while feature (i.e. a group of commits), is a true headache in the latter situation,
whereas it is easily done if the `--on-ff` flag was used.

### Release branches
branch off from `develop`, must merge back into `develop` and `master`, branch naming convention: `release-*`.

Release branches support preparation of a new production release.
They allow for last-minute dotting of i's and crossing t's.
Furthermore, they allow for minor bug fixes and preparing meta-data for a release (version number, build dates, etc.).
By doing all of this work on a release branch, the develop branch is cleared to receive features for the next big release.

It is exactly  at the start of a release branch that the upcomming release gets assigned a version number - not any earlier.
Up until that moment, the `develop` branch reflected changes for the "next release",
but it is unclear whether that "next release" will eventually become 0.3 or 1.0,
until the release branch is started.
That decision is made on the start of the release branch and is carried out by the project's rules on version number bumping.

#### Creating a release branch
