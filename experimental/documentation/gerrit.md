Using Gerrit without git-cl
===========================

setup
-----

The following must be executed within the Skia source repository.

This command sets up a Git commit-message hook to add a unique Change-Id to
each commit.  Gerrit only accepts changes with a Change-Id and uses it to
identify which review a change applies to.

    curl -Lo "$(git rev-parse --git-dir)/hooks/commit-msg"
      'https://gerrit-review.googlesource.com/tools/hooks/commit-msg'
    chmod +x "$(git rev-parse --git-dir)/hooks/commit-msg"

If you aquired Skia from a mirror (such as github), you need to change the
`origin` remote to point to point to googlesource.  Advanvced uses will note
that there is nothing special about the string `origin` and that you could call
this remote anything you want, as long as you use that name for `get push`.

    git remote set-url origin 'https://skia.googlesource.com/skia.git'


creating a change
-----------------

1.  Create a topic branch

        git checkout -b TOPIC

    You may want to set a tracking branch at this time with:

        git checkout -b TOPIC -t origin/master

2.  Make a commit.

        echo FOO >> whitespace.txt
        git commit --all --message 'Change Foo'
        git log -1

    `git log` should show that a Change-Id line has been added you your commit
    message.


3.  If You have multiple commits in your branch, Gerrit will think you want
    multiple changes that depend on each other.  If this is not what you want,
    you need to squash the commits.

4.  Push to Gerrit

        git push origin @:refs/for/master

    `@` is shorthand for `HEAD`, introduced in git v1.8.5.

    If you want to target a branch other than `master`, that can be specified
    here, too.  For example:

        git push origin @:refs/for/chrome/m57

    [Gerrit Upload Documentation](https://gerrit-review.googlesource.com/Documentation/user-upload.html)


updating a change
-----------------


1.  Edit your commits more.

        echo BAR >> whitespace.txt
        git commit --all --amend

    Changes to the commit message will be sent with the push as well.


2.  Re-squash if needed.  (Not needed if you only amended your original commit.)


3.  Push to Gerrit.

        git push origin @:refs/for/master

    If you want to set a comment message for this patch set, do this instead:

        git push origin @:refs/for/master%m=this_is_the_patch_set_comment_message

    The title of this patch set will be "this is the patch set comment message".


scripting
---------

You may want to make git aliases for common tasks:

    git config alias.gerrit-push 'push origin @:refs/for/master'

The following alias amends the head without editing the commit message:

    git config alias.amend-head 'commit --all --amend --reuse-message=@'

The following shell script will squash all commits on the current branch,
assuming that the branch has an upstream topic branch.

    squash_git_branch() {
        local MESSAGE="$(git log --format=%B ^@{upstream} @)"
        git reset --soft $(git merge-base @ @{upstream})
        git commit -m "$MESSAGE" -e
    }

This shell script pushes to gerrit and adds a message to a patchset:

    gerrit_push_with_message() {
        local REMOTE='origin'
        local REMOTE_BRANCH='master'
        local MESSAGE="$(echo $*|sed 's/[^A-Za-z0-9]/_/g')"
        git push "$REMOTE" "@:refs/for/${REMOTE_BRANCH}%m=${MESSAGE}"
    }

These shell scripts can be turned into Git aliases with a little hack:

    git config alias.squash-branch '!M="$(git log --format=%B ^@{u} @)";git reset --soft $(git merge-base @ @{u});git commit -m "$M" -e'

    git config alias.gerrit-push-message '!f(){ git push origin @:refs/for/master%m=$(echo $*|sed "s/[^A-Za-z0-9]/_/g");};f'


