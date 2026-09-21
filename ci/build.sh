#!/bin/sh
# Build every week in one group and report ALL failures, not just the first.
#
# The raw log and the step summary both need a GitHub sign-in to read, so the
# failures are also emitted as a workflow annotation, which is visible to
# anyone looking at the run. One annotation per group, carrying every error.
set -u
group="$1"
dirs=$(make -s print-"$group")
summary="${GITHUB_STEP_SUMMARY:-/dev/stdout}"
label="${RUNNER_OS:-local}/${CC:-cc}"
tmp=$(mktemp -d)
failed=""

printf '\n## %s — %s\n\n| Week | Result |\n|---|---|\n' "$group" "$label" >> "$summary"

for d in $dirs; do
    if make -C "$d" > "$tmp/$d.log" 2>&1; then
        printf '| `%s` | ok |\n' "$d" >> "$summary"
    else
        failed="$failed $d"
        printf '| `%s` | **FAILED** |\n' "$d" >> "$summary"
    fi
done

[ -z "$failed" ] && { printf '\nAll of %s built on %s.\n' "$group" "$label"; exit 0; }

# full detail into the summary
printf '\n### Errors\n' >> "$summary"
for d in $failed; do
    printf '\n<details><summary>%s</summary>\n\n```\n' "$d" >> "$summary"
    head -40 "$tmp/$d.log" >> "$summary"
    printf '```\n\n</details>\n' >> "$summary"
done

# condensed detail into one annotation (newlines encoded as %0A)
msg=$(for d in $failed; do
        printf '%s:\n' "$d"
        grep -E 'error:|Error [0-9]|No rule to make|undefined reference|fatal' "$tmp/$d.log" \
            | head -3 | sed 's/^/  /'
      done | sed 's/%/%25/g' | awk '{printf "%s%%0A", $0}')
printf '::error title=%s failed on %s::%s\n' "$group" "$label" "$msg"

printf '\nFailed:%s\n' "$failed"
exit 1
