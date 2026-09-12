#!/bin/sh
set -eu

repo="$HOME/Projects/alphasmart/betawise-abi"
branch="sdk/abi-automation"
task="$repo/docs/OS3K_ANALYSIS_NEXT.md"
result="$repo/docs/OS3K_ANALYSIS_RESULT.md"

export PATH="$HOME/.local/bin:$PATH"

cd "$repo"

current_branch=$(git branch --show-current)
if [ "$current_branch" != "$branch" ]; then
    echo "ERROR: expected branch $branch, found $current_branch" >&2
    exit 1
fi

if [ -n "$(git status --porcelain)" ]; then
    echo "ERROR: worktree is not clean" >&2
    git status --short
    exit 1
fi

git fetch origin "$branch"

local_head=$(git rev-parse HEAD)
remote_head=$(git rev-parse "origin/$branch")

if [ "$local_head" != "$remote_head" ]; then
    echo "ERROR: local and origin/$branch differ; synchronize manually before running Codex." >&2
    git log --oneline --left-right "HEAD...origin/$branch" --max-count=20
    exit 1
fi

cat "$repo/AGENTS.md" "$task" | \
    script -qefc "codex exec --dangerously-bypass-approvals-and-sandbox -C \"$repo\" -" /dev/null

echo
echo "=== OS3K ANALYSIS RESULT ==="
cat "$result"
echo
echo "=== GIT STATUS ==="
git status --short
