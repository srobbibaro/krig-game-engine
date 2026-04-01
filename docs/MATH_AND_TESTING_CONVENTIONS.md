# Math layout and testing conventions

This document records **behavior that is easy to misread** when assuming
textbook OpenGL, "standard" ray casts, or row-major matrix habits. It
complements comments in `src/Vector.h`, `src/Matrix.h`, and the Catch
tests under `test/`.

**Where comments live:** **`Matrix.h`** / **`Matrix.cpp`** keep
**self-contained** API notes so commits that touch them do not depend on
this file. The **`test/`** files **`vector.cpp`**, **`matrix.cpp`**, and
**`frustum.cpp`** use **self-contained comments** at the scenarios that
lock in odd behavior (so test-only commits do not refer to paths that do
not exist yet in history).

## Matrix storage and the 16-float constructor

- **`Matrix::data[16]`** is **column-major**, consistent with typical
  OpenGL fixed-function expectations: column 0 occupies indices
  `0,4,8,12`, column 1 occupies `1,5,9,13`, and so on.
- **`Matrix( float × 16 )`** expects arguments in **column order**:
  first the four entries of column 0 (top to bottom in math notation),
  then column 1, then column 2, then column 3. It does **not** match
  "write four rows left-to-right on paper" unless you transpose
  mentally.

**Tests:** `test/matrix.cpp` (explicit constructor scenario).

## `Matrix::operator*` composition

For `A = S * T` with `S = setScale(…)` and `T = setTranslation(…)`,
`transformVertex` on a column vector follows the engine's multiplication
order (see `Matrix::operator *`). A naïve expectation "scale first, then
translate" can disagree with the numeric result: the **Scale times
translation** scenario in `test/matrix.cpp` locks in the current
behavior (e.g. translating then scaling on X for that product).

## QuadTree XZ partitioning

- **`QuadTree::buildTree(QuadTreeNode*&, xMin, xMax, zMin, zMax,
  scaleFactor)`** stores **`min[0]`/`max[0]` as X** and
  **`min[1]`/`max[1]` as Z** (2D partition in the horizontal plane).
- **`boundingSphere`** uses **`y = 0`** and **`z = -centerZ`** for the
  sphere origin (see `src/QuadTree.cpp`), which matches the engine's
  ground-plane convention but differs from "max corner as positive Z
  only" mental models.

**Tests:** `test/quadtree.cpp` (`buildLeafList`, splits for square vs
elongated regions). The tests call a local **`freeQuadTreeNode`**
because **`QuadTree`'s destructor does not delete the tree.**

## `Quaternion::operator*` and `krig.rotation.add`

`Quaternion::operator*` computes quaternion composition using the
engine's multiplication convention (see `test/quaternion.cpp` for the
analytic result of 90°X × 90°Y). Composition is **non-commutative**:
`q1 * q2 ≠ q2 * q1` in general.

The Lua API function `krig.rotation.add(q1, q2)` calls `q1 * q2` — it
is **rotation composition**, not component-wise addition. The name "add"
reflects how Lua game scripts accumulate rotations, but it maps to
`operator*`, not `operator+`. Do not confuse it with `operator+`, which
does literal per-component addition and is not exposed to Lua.

## `Quaternion::slerp`

The guard is `if (t >= 0.0f && t <= 1.0f)`. Values outside `[0, 1]`
leave `*this` unchanged. Both engine callers (`Object.cpp`,
`Camera.cpp`) clamp `t` before calling `slerp`, so out-of-range values
should not occur in normal use. See `test/quaternion.cpp` for endpoint,
out-of-range, negative dot product, and linear fallback coverage.

## Frustum helper tests

`test/frustum.cpp` uses a **deliberately simple** matrix (identity with
`data[10]` adjusted) to exercise `Frustum::extractFromProjectionMatrix`
without requiring a full `gluPerspective`-style matrix. With that matrix
the six planes bound the unit cube `[-1, 1]^3` and all plane distances
from the origin equal 1, so `testSphere` results are analytically
predictable. Tests assert **specific classification codes** (inside = 1,
outside = −1, intersecting = 0) rather than just valid-code membership.
`getPlaneDefinition` with an out-of-range index silently leaves output
parameters unchanged — locked in as a no-op guard.

**`testSphere` is conservative**: it returns `0` on the first plane
where `|distance| < radius` without checking remaining planes. A sphere
that intersects one plane but is outside another returns `0` (render)
instead of `-1` (cull). Both engine callers
(`ModelGameObject::animate`, `QuadTree::buildDisplayList`) use
`r != -1` as the visibility threshold, so this conservative behavior
causes over-rendering at most — objects are never incorrectly hidden.

## `Object::calcTriangleCenter`

`calcTriangleCenter(p1, p2, p3)` returns **half of the midpoint of p1
and p2, averaged with p3** — i.e. successive midpoints — **not**
`(p1 + p2 + p3) / 3`. Locked in `test/object.cpp`.

## `Object::setRotation` vs `Object::setRotationQuaternion`

These two methods both assign `rotation_` but have different side
effects:

- **`setRotation(const Quaternion&)`** (Object.h) sets
  `rotation_ = q` **and** `rotationChanged_ = true`.
- **`setRotationQuaternion(const Quaternion&)`** (Object.cpp) sets
  `rotation_ = q` **only** — it does **not** set `rotationChanged_`.

Callers that need the rotation matrix rebuilt on the next frame (e.g.
the render path) must use **`setRotation`**. Using
`setRotationQuaternion` silently skips the rebuild. Locked in
`test/object.cpp`.

## `GameLevel` metadata (non-script)

`setComplete`, `setElapsedTime`, `setId`, `setLightDirection`
(normalized), `setMusicPath`, `getObjects`, and `getObjectFromId` are
covered in `test/gamelevel.cpp` without custom fixtures. Behaviour of
**`loadLevelFromBuffer`** still depends on the bundled level bootstrap
logic.

## Camera tests without GL

`Camera::update`, `setCamera`, and `getRotationMatrix` paths used in
`test/camera.cpp` do not call **OpenGL or Lua** when the object has **no
script** (`L_ == NULL`). Avoid `prepareGLView`, `setProjectionMatrix`,
and `draw` in such tests.

**`Camera::update` unconditionally rebuilds `rotationMatrix`** on every
call via `rotation_.buildRotationMatrix(rotationMatrix)`. Unlike
`Object`, which gates the rebuild on `rotationChanged_`, `Camera` always
rebuilds — even with zero rotation velocity and no change in
orientation. Do not assume a no-op `update(0)` call leaves
`rotationMatrix` untouched; it rewrites it from the current quaternion.
Locked in `test/camera.cpp`.

After building **`direction_`** from **`rotateVector`**,
**`Camera::update`** applies **`direction_.setVector(x, y, -z)`**
(negates Z). So the **forward basis** is not a raw rotation of
**`baseDirection_`**; do not assume **`getDirection()`** matches a
textbook "camera look" vector without accounting for that flip
(`src/Camera.cpp`). The **`test/camera.cpp`** matrix check does not
assert **`direction_`**.

## `Plane::normalize` with zero normal

If the **(a, b, c)** components have length **0**, **`normalize()`**
leaves **all four** plane coefficients **unchanged** (no division). That
can leave a non-unit "normal" and is locked in **`test/plane.cpp`**
(degenerate case).

## `GameTimer` — blocking spin and FPS validity

On non-Windows platforms, **`GameTimer::getElapsedSeconds()`** spins in
a busy loop until at least `1/MAX_FPS` seconds have elapsed since the
last call (~33 ms at 30 FPS). The first call after `init()` can block
for that full interval. **`getFPS()`** is only meaningful after at least
one `getElapsedSeconds()` call — it returns `0` before any
elapsed-time measurement has been made. Both behaviors are locked in
`test/gametimer.cpp` (the `getFPS == 0` assertion is guarded by
`#ifndef _WIN32` because Windows is not currently supported).

## Where to look next

| Topic | Code | Tests |
|--------|------|--------|
| Column-major / ctor / `*` | `Matrix.h`, `Matrix.cpp` | `matrix.cpp` |
| Frustum planes / sphere | `Frustum.cpp` | `frustum.cpp` |
| Quadtree | `QuadTree.cpp`, `QuadTreeNode.cpp` | `quadtree.cpp` |
| Camera (no GL) | `Camera.cpp` | `camera.cpp` |
| Planes (incl. degenerate normalize), quaternions, lists | `Plane.cpp`, `Quaternion.cpp`, `ObjectList.cpp` | `plane.cpp`, `quaternion.cpp`, `objectlist.cpp` |
| Object helpers / state | `Object.cpp` | `object.cpp` |
| GameLevel metadata | `GameLevel.cpp` | `gamelevel.cpp` |
| GameTimer spin / FPS | `src/gametimer.h`, `src/gametimer.cpp` | `test/gametimer.cpp` |

When you add behavior that trades obvious "textbook" semantics for
historical engine behavior, **document it here** and add or adjust a
targeted unit test.

For **how to run tests, suite policy, and Engine lifecycle vs
shutdown**, see **`docs/TESTING.md`**.
