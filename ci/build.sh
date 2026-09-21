#!/bin/sh
# Build every week in one group, report ALL failures rather than stopping at the
# first, and write the result to the run summary — which is readable without
# signing in, unlike the raw log.
set -u
group="$1"
dirs=$(make -s print-"$group")
summary="${GITHUB_STEP_SUMMARY:-/dev/stdout}"
label="${RUNNER_OS:-local} / ${CC:-cc}"
failed=""

printf '\n## %s — %s\n\n| Week | Result |\n|---|---|\n' "$group" "$label" >> "$summary"

for d in $dirs; do
    if out=$(make -C "$d" 2>&1); then
        printf '| `%s` | ok |\n' "$d" >> "$summary"
    else
        failed="$failed $d"
        printf '| `%s` | **FAILED** |\n' "$d" >> "$summary"
    fi
done

if [ -n "$failed" ]; then
    printf '\n### Errors\n' >> "$summary"
    for d in $failed; do
        printf '\n<details><summary>%s</summary>\n\n```\n' "$d" >> "$summary"
        make -C "$d" 2>&1 | head -40 >> "$summary"
        printf '```\n\n</details>\n' >> "$summary"
    done
    printf '\nFailed:%s\n' "$failed"
    exit 1
fi

printf '\nAll of %s built.\n' "$group"
