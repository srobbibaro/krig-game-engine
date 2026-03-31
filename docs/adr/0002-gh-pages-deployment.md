# ADR 0002: GitHub Pages deployment — GITHUB_TOKEN + manual git push

## Status

Accepted

## Context

The project previously deployed Doxygen documentation to GitHub Pages via a
Travis CI script (`build-scripts/update-documentation.sh`). Travis was
decommissioned and the `gh-pages` branch has been frozen since 2017. A
replacement was needed to auto-deploy Doxygen docs and the new lcov coverage
report on every merge to master, without external service dependencies.

## Decision

Add a `pages` CI job to `.github/workflows/c-cpp.yml` that:

- Runs on push to master and on `workflow_dispatch` (manual trigger).
- Depends on `build` and `test` jobs passing — Pages only updates when CI is green.
- Builds Doxygen docs (`make doc`) and the coverage report (`make run-coverage`).
- Deploys to the `gh-pages` branch using a manual git push authenticated with
  the built-in `secrets.GITHUB_TOKEN` (scoped to this repo, short-lived,
  auto-rotated by GitHub).
- Fully replaces `gh-pages` branch content on each deploy (`rm -rf /tmp/gh-pages/*`)
  so the published site always exactly matches the latest build.
- Skips the commit if nothing changed (`git diff --cached --quiet`).

Doxygen output is placed at the `gh-pages` root (preserving existing URLs at
`srobbibaro.github.io/krig-game-engine/`). Coverage report is placed under
`coverage/`.

## Alternatives considered

**`peaceiris/actions-gh-pages`** — the most common community action for this
pattern. Rejected because it is a third-party dependency; the same behavior
is achievable with `actions/checkout` and a short shell script using only
built-in git commands.

**`actions/upload-pages-artifact` + `actions/deploy-pages`** — official
GitHub-owned actions for Pages deployment. Would require switching the Pages
source from `gh-pages branch` to `GitHub Actions` in repo settings, changing
the deployment model. Rejected to keep the existing `gh-pages` branch
source setting and avoid a one-time manual settings change.

**Personal access token (PAT)** — the approach used by the original Travis
script (`$GH_TOKEN`). Rejected in favour of `GITHUB_TOKEN`, which requires
no manual token creation or rotation and is scoped to this repo only.

## Consequences

- Doxygen and coverage reports are published automatically after every green
  master merge. No manual steps required.
- `build-scripts/update-documentation.sh` (Travis-only) is removed.
- `secrets.GITHUB_TOKEN` write permission is granted to the `pages` job only,
  scoped via the `permissions: contents: write` key.
- If the Pages job fails, the site is not updated but master CI (build/test)
  is unaffected — the `pages` job is independent.
- The `pages` job does not depend on the `coverage` CI job. Both rebuild
  coverage independently. This is intentional — a coverage toolchain flake
  should not block the site from updating.
- `workflow_dispatch` deploys whatever ref is selected at trigger time, not
  necessarily master. This is intentional (allows a manual refresh from master
  without a dummy commit) but means dispatching from a topic branch will
  publish that branch's docs. Use master unless intentionally publishing
  from another ref.
