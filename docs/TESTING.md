# Unit testing

The C++ engine is exercised by **Catch** (v1) tests in `test/`. The Makefile builds a `unit-test` binary that links game objects from `obj/src/` with `obj/test/*.o` (see `Makefile` targets `build-test` and `run-tests`).

## Running tests

```bash
make run-tests
```

Equivalent: `make build-test && ./unit-test`

## Coverage

```bash
make run-coverage
```

Generates an HTML report at `coverage/index.html`. The CI `coverage` job uploads this as a downloadable artifact on every push.

**lcov compatibility warnings** — the `run-coverage` target passes `--ignore-errors` flags to suppress warnings from lcov 2.4's stricter validation of LLVM gcov's emulated output format. These are a known toolchain compatibility shim, not indicators of bad data. Line coverage numbers for our source files are trustworthy. The bulk of the noise comes from Catch v1's single-header inline functions; upgrading to Catch2/3 would reduce it, but the underlying lcov 2.4 / LLVM gcov gap would remain.

**Meaningful files** — these are directly exercised by the unit tests: `Vector`, `Matrix`, `Quaternion`, `Sphere`, `Frustum`, `Plane`, `GameLevel`, `Camera`, `GameTimer`, and their API bindings.

**Expected low coverage** — `Engine.cpp`, `ModelGameObject.cpp`, `Terrain.cpp`, audio, and GLUT callbacks require a live display and audio device. Low numbers in these files are expected, not a gap in test coverage.

## What the suite is for

- **Regression guard** for math, spatial structures, list behaviour, and plain C++ game-object state.
- **Documentation by example** for APIs that are easy to misread (see below).

It is **not** a full integration harness: there is no automated GL context, OpenAL device, or end-to-end game loop.

### Noise during `make run-tests`

Constructing **`GameLevel`** can trigger **`Music`** / OpenAL buffer setup; in headless or CI environments you may see **AL errors** in the log even when **all assertions pass**. That is a known limitation of running tests against the full object graph.

## Engine-specific notes (for tests and tooling)

These points come from `Engine` construction/teardown and from running tests that link the full engine:

1. **`Engine::shutdown()`** only sets **`isRunning_`** to false ([`Engine.cpp`](../src/Engine.cpp)). It does **not** close Lua, OpenAL, or delete levels. Full cleanup happens in **`~Engine()`** via `unloadGame()` / `unload()` (AL context/device, `delete currentLevel_`, etc.).
2. **Constructor** initializes **`GameTimer`**; if **`timer_.init()`** fails, the constructor logs and **`exit(1)`** — there is no recoverable error path for embedders or unit tests on that path.
3. **`getKeyState()`** and **`getSpecialKeyState()`** always return addresses of **`Engine`** member structs. They stay valid for the lifetime of the **`Engine`** instance (including after **`shutdown()`**), until the object is destroyed.
4. Methods such as **`initGL`**, **`initAL`**, **`loadGame`**, **`gameCycle`**, and **`renderText`** assume a real GLUT/GL and audio environment; the current **`test/engine.cpp`** scenarios intentionally avoid them.

## Counterintuitive behaviour the suite locks in

Some assertions match the **current engine**, not necessarily what readers assume. The main ones are spelled out in **`docs/MATH_AND_TESTING_CONVENTIONS.md`** (matrix layout, rotations, quadtree sphere Z, quaternion `slerp`, etc.). Additional **game-object / lifecycle** quirks:

1. **`Object::state` vs `Object::active`** — **`setState(DEAD)`** (and related) updates the **`state_`** field only. It does **not** clear **`active_`**. You can be **`DEAD`** and still **`getActive() == true`** until **`setActive(false)`** is used. See `test/object.cpp`.
2. **Two `setState` overloads** — **`setState(const unsigned char&)`** and **`setState(const int&)`** both write **`state_`**. Callers should treat them as the same destination with different argument types (legacy API surface).
3. **`Object::animate` — explicit base call in tests** — concrete subclasses override `animate(float, Object*)`, which has a different signature from the base `animate(const float&, Object*)`. Virtual dispatch will resolve to the subclass (empty no-op in `BasicObject`). To test the base-class frame-loop logic (velocity accumulation, rotation velocity, scale rate), call `o.Object::animate(dt, nullptr)` explicitly.
4. **`GameLevel` minimal buffer load** — **`loadLevelFromBuffer(“test=1”)`** is a harness-friendly path that depends on engine bootstrap behaviour; **`getObjectFromId(0)`** is the camera after **`setCamera`**, not a general guarantee for arbitrary buffers.
5. **Lua `krig.vector`** — **`scalar`** and **`dot_product`** wrap the same underlying vector math in the C API; overlapping behaviour is intentional for script compatibility, not a bug to “fix” in tests alone.

For **math-specific** surprises, always check **`MATH_AND_TESTING_CONVENTIONS.md`** first.

## Scope conventions (when adding tests)

Prefer tests that need **only stack allocation and existing public APIs**:

- **In scope:** `Vector`, `Matrix`, `Quaternion`, `Plane`, `Sphere`, `Frustum`, `QuadTree` / `QuadTreeNode`, `DisplayList`, `ObjectList`, `Object` / `Camera` state setters, `GameLevel` metadata accessors, `KeyState`, `Engine` smoke checks.
- **Use care:** Anything that opens **Lua scripts** (`Object::setScript`, `loadScript`, level load with real script paths), **OpenGL** (`gl*` in `prepareGLView`, terrain draw), or **`setParticleSystem`** (allocates particle implementations) unless you accept heavier coupling.
- **Lua API surface:** `test/api.cpp` covers the **`krig.vector`** bindings; expanding other `api_*` tests is optional.

Do **not** require new **fixtures** (terrain files, golden assets) or **engine refactors** unless the task explicitly allows it.

## Test files (by area)

| File | Focus |
|------|--------|
| `catch.cpp` | Catch main |
| `vector.cpp`, `matrix.cpp`, `quaternion.cpp`, `plane.cpp` | Core math |
| `sphere.cpp`, `frustum.cpp` | Collision / view primitives |
| `object.cpp` | `Object` construction and state |
| `objectlist.cpp`, `displaylist.cpp` | Lists |
| `quadtree.cpp` | `QuadTree::buildTree`, `buildLeafList`, node defaults |
| `camera.cpp` | `Camera` without GL (e.g. `setCamera`, `update(0)`, `getRotationMatrix`) |
| `gamelevel.cpp` | Level load failures/success, metadata, light, ids |
| `engine.cpp` | `Engine` running flag, shutdown, key-state pointers |
| `keystate.cpp` | Key state tracking |
| `gametimer.cpp` | Frame timing |
| `api.cpp` | Lua vector API |

## Related documentation

- **`docs/MATH_AND_TESTING_CONVENTIONS.md`** — Matrix layout, rotation conventions, quadtree XZ / sphere Z sign, quaternion `slerp` guard, matrix multiply composition.
- **`README.md`** — Build and one-line test invocation.

When you add behaviour that is **surprising but measurable**, add a **short test** and a **sentence in `MATH_AND_TESTING_CONVENTIONS.md`** (for math/spatial) or here (for process/coverage).
