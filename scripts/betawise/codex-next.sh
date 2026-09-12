#!/bin/sh
set -eu

repo="$HOME/Projects/alphasmart/betawise-abi"
branch="sdk/abi-automation"
task="$repo/docs/CODEX_NEXT.md"
result="$repo/docs/CODEX_RESULT.md"

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

cat "$repo/AGENTS.md" "$task" |     codex exec --approve-for-me -C "$repo" -

echo
echo "=== CODEX RESULT ==="
cat "$result"
echo
echo "=== GIT STATUS ==="
git status --short
