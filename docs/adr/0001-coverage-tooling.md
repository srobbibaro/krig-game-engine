# ADR 0001: Coverage tooling — gcov + lcov

## Status

Accepted

## Context

The project needed a way to measure unit test coverage to identify untested areas
and track progress as the test suite grows. The tool needed to be free, runnable
both locally (macOS) and on CI, and require no external service dependency.

## Decision

Use **gcov** (built into GCC/Clang) for instrumentation and **lcov** for report
generation. The Makefile `run-coverage` target builds an instrumented binary
(`unit-test-cov`) in a separate `obj-cov/` directory, runs it, collects `.gcda`
data, filters out system headers and the Catch test framework, and generates an
HTML report via `genhtml`.

On macOS (Apple Clang), `llvm-cov gcov` is used as the gcov tool via a thin
wrapper script (`build-scripts/llvm-cov-gcov.sh`). The Darwin `ifeq` block in
the Makefile sets `GCOV` to the wrapper automatically, so no override is needed:

```bash
make run-coverage
```

CI runs the coverage job on `macos-latest` to match the existing build and test
jobs, with `lcov` installed via `brew install lcov`.

Coverage is **informational only** — it is not a gate on PRs and does not affect
the production build (`-O3`). The coverage binary uses `-O0`.

## Alternatives considered

**Codecov / Coveralls** — both are free for open-source and provide PR-level
coverage diffs and badge tracking. Rejected because they require an external
service and account setup. The project currently has no external service
dependencies and the overhead is not justified at this stage.

**Linux CI job for coverage** — gcov and lcov work with less friction on Linux
(no `llvm-cov gcov` wrapper needed). Rejected because the project is not
targeting Linux CI yet; adding a Linux job solely for coverage would be
inconsistent with the rest of the CI setup. Can be revisited when Linux CI is
added.

**llvm-cov show / llvm-cov report** — Clang's native coverage format
(`-fprofile-instr-generate` / `-fcoverage-mapping`). More accurate with Clang
but requires a different toolchain path and produces different output. Rejected
in favour of gcov compatibility, which works with both GCC and Clang via
`--coverage`.

## Consequences

- Coverage reports are generated locally and uploaded as CI artifacts (retained
  14 days). No persistent coverage history or trend tracking without an external
  service.
- Numbers will be naturally low for rendering, audio, and GLUT callback code —
  these require a live display and audio device. This is expected and documented
  in `docs/TESTING.md`.
- If a Linux CI job is added in the future, the `GCOV` override can be dropped
  and the default `gcov` will work without modification.
