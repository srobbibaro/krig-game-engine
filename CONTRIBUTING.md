# Contributing to Krig Game Engine

## Commit messages

All non-trivial commits must follow this format:

```
type: short, non-blank description of the change

Problem:

A clear statement of why the change is needed — what is wrong,
missing, or confusing without it.

Solution:

Concrete steps taken to address the problem. Use a bulleted list
for multi-part changes.
```

The summary line must be present and meaningful — do not leave it blank or use placeholder text.

Trivial commits (e.g. fixing a typo, renaming a variable in one place) may use a single summary line only.

All commits must be verified before merging.

## Doc comments

Use `/** */` comments selectively — only on methods where the behaviour is
non-obvious or a precondition would be missed by a reader. Do not add doc
comments to every method. Examples of what warrants a comment: a parameter
that must be a unit vector, a no-op guard on a zero input, a non-roundtrip
extraction, an unimplemented stub. Examples of what does not: a copy
constructor, an identity loader whose name is self-explanatory.

Multi-line doc comments use the following format:

```cpp
/**
 * Description.
 */
```

## Branch naming

Branches must follow this format:

```
<user-short-name>/short-description-of-changes
```

For example: `steve/add-quaternion-slerp`

## Atomic commits

Prefer atomic commits in almost every circumstance — one logical change per commit. A commit should do exactly one thing and do it completely. This makes history bisectable, reverts safe, and reviews straightforward.

Every commit must leave the project in a good working state: `make build` compiles cleanly and `make run-tests` passes. For commits that change engine implementation code (anything under `src/`), `make run-demo` must also start the demo game without crashing. Test-only and docs-only commits do not require a demo run. This is the baseline definition of atomic — a commit is not complete unless all applicable checks hold. This is what makes bisect reliable and individual commits reviewable in isolation.

A complete unit of work means code, tests, and documentation travel together. Any commit that adds or changes behaviour must include:
- The implementation change (`src/`)
- Tests that cover the new or changed behaviour (`test/`)
- Any documentation that needs updating — header contracts, `docs/MATH_AND_TESTING_CONVENTIONS.md`, `docs/TESTING.md`, or inline comments — so the commit is fully self-contained and reviewable in isolation

In practice this means:
- One new test file per commit, not several bundled together.
- Source changes and their corresponding test changes travel together in a single commit. This includes fixing any tests broken by a source removal.
- Documentation that captures non-obvious behaviour, design decisions, or API contracts belongs in the same commit as the code it describes. Standalone doc-only changes (e.g. fixing a stale comment or adding a section unrelated to a code change) get their own commit.

If you find yourself writing "and" in a commit summary, that is usually a sign the commit should be split.

Within a body of related work (e.g. a set of new documentation files or a batch of test additions), prefer splitting by logical unit even when all the changes are the same type. A reviewer should be able to read one commit in isolation and understand exactly what it introduces and why. Splitting by unit — one doc per commit, one test file per commit — keeps each commit independently reviewable and makes the history tell a coherent story. This does not override the rule that code, tests, and docs travel together; it applies within the same type of change where bundling adds noise rather than context.

## What belongs in source control

Before staging files for a commit, verify that everything included is appropriate for a public repository. Things that must not be committed:

- **Credentials and secrets** — API keys, tokens, passwords, private certificates, or any value that grants access to a system.
- **Local configuration** — IDE settings, editor state, OS metadata (`.DS_Store`, `.vscode/`, etc.). Add these to `.gitignore` instead.
- **Private or unreleased content** — game assets, levels, scripts, or tools that reference other unreleased work. Committing a doc or source file that points to private files creates dangling references for public readers.
- **Personal information** — email addresses, usernames, or other identifying details not already public to the project.

Git history is effectively permanent. Rewriting history (via `git rebase`, `git filter-branch`, or similar) does not reliably remove content that has already been pushed — it may persist in clones, forks, CI caches, or external mirrors. If sensitive content is accidentally committed and pushed, treat it as exposed and rotate or revoke it immediately rather than relying on a history rewrite to contain it.

## Pull request format

All pull requests must include the following sections:

```
### Summary / Overview

This is a summary of what the pull request does and why it exists, including the changes being made to the system.

### Verification

Any verification or testing steps necessary to make sure the changes are sound. This could include things like building, running the game, and verifying a passing test suite. Additionally, if there are any steps to test manually, those steps should be outlined. Say for example, we're updating the demo game to add new items in the first level. We should also outline any exploratory testing or edge cases that should be explored prior to merging.

### Dependencies

This section can be skipped if there aren't any. Since we're typically going to be practicing atomic commits, dependencies should be minimal.

### Compatibility

List out any breaking changes, deprecations, or notes that could impact existing games written in Lua or other general compatibility concerns.
```

## Test coverage approach

When adding or reviewing tests, go beyond happy-path coverage. For each class or method under test, consider:

- **Critical paths first** — identify how the class is actually used in the engine (callers in `src/`, Lua API in `api_*.cpp`) and make sure the real usage patterns are exercised, not just isolated unit behaviour.
- **Analytically grounded assertions** — where possible, compute expected values by hand and assert specific results rather than just checking that output falls within a valid range. A test that asserts `result == 1` is stronger than one that asserts `result >= -1 && result <= 1`.
- **Lock in non-obvious behaviour** — if a method has a guard, a fallback, or a conservative trade-off (e.g. an early return that can over-render but never under-render), write a test that explicitly captures it. Future readers should not have to re-derive why the behaviour is correct.
- **Self-contained scenarios** — each `GIVEN` block should be independently readable. Do not rely on a reader connecting it to another scenario to understand what is being asserted.
- **Parameter names and header contracts** — when a declaration uses bare positional parameters and the argument order or preconditions are not obvious, add parameter names to the declaration. This is a non-functional change but it makes the API self-documenting without requiring a doc comment.
- **Semantic clarity** — small non-functional changes (renaming variables to domain terms, correcting stale comments, fixing comment/code mismatches) are worth making alongside tests. They reduce the cognitive load for anyone reading the code later and belong in the same commit as the tests they accompany.

## Line length

Aim for a maximum of **120 characters per line** in source files, headers, tests, and documentation prose. This is a strong guideline, not a hard rule — prefer exceeding it over breaking readability (e.g. a long string literal, a URL, or a deeply nested expression that reads clearly on one line).

## Self-documenting code

Code should communicate its intent through naming, not just comments. Prefer names that make the purpose clear without requiring explanation:

- Variable and parameter names should reflect domain concepts — `tNear`, `nearFaceFound`, `hitPoint` over `s`, `sComputed`, `tv`.
- Declaration parameter names should be present whenever the argument order or meaning is not obvious from the type alone — `setSphere(float x, float y, float z, float radius)` over four anonymous `const float&` parameters.
- Stale names and comments that no longer match the implementation should be corrected promptly — a mismatch between a comment and the code it describes is actively misleading.

Self-documenting naming is a first-class goal. A `/** */` doc comment that explains what a variable name already makes obvious is a sign the name should be improved, not that the comment should stay.

When working with existing code, apply these improvements with judgement. A rename that touches many callers or crosses a stable public API boundary may not be worth the churn — note it for a dedicated cleanup commit instead. The right time to improve a name is when you are already changing the surrounding code, not as a reflexive edit on every file you open.

## Running tests

```bash
make run-tests
```

See `docs/TESTING.md` for full test scope, conventions, and known behaviour the suite locks in.

## Documentation index

A full list of documentation files and their purpose is maintained in the [Documentation section of README.md](README.md#documentation).

When adding or updating documentation, verify that every cross-reference — links to other docs, source file paths, section anchors, and test file names — exists and is reachable. Do not reference files that are not yet in source control.

Any commit that adds, removes, or renames a function in an `api_*.cpp` registration table (`luaL_Reg`) must update `docs/LUA_API.md` to match.

## Architecture rules

A few constraints that must be respected to keep the engine consistent:

- **Rotation always goes through `Quaternion`** — use `buildFromAxis`, `buildFromEuler`, and `buildRotationMatrix`. Do not introduce parallel matrix-level rotation paths.
- **`Matrix::data[16]` is column-major** (OpenGL convention). The 16-float constructor takes columns, not rows.

For non-obvious engine behaviour that affects how you read or test existing code (object state, camera update, quaternion guards, frustum classification, etc.), see `docs/MATH_AND_TESTING_CONVENTIONS.md`.
