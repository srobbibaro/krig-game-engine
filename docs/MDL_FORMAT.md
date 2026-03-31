# `.mdl` Model Format (Krig Game Engine)

This documents the custom text-based `.mdl` format used by the Krig
Game Engine for static triangle meshes. It is derived from
`ModelStorage::load()` in `src/ModelGameObject.cpp`.

All values are written as plain ASCII numbers separated by whitespace
(`std::ifstream >>` semantics).

---

## Overview

File layout in order:

1. `initalScale` (float)
2. `numVertices` (int)
3. Per-vertex data (repeated `numVertices` times):
   - `x` (float)
   - `y` (float)
   - `z` (float)
   - `nx` (float)
   - `ny` (float)
   - `nz` (float)
4. `numTriangles` (int)
5. Per-triangle data (repeated `numTriangles` times):
   - `v0` (int) – index into vertex array
   - `v1` (int)
   - `v2` (int)
   - `c0_r` (float) – color for vertex `v0`
   - `c0_g` (float)
   - `c0_b` (float)
   - `c1_r` (float) – color for vertex `v1`
   - `c1_g` (float)
   - `c1_b` (float)
   - `c2_r` (float) – color for vertex `v2`
   - `c2_g` (float)
   - `c2_b` (float)

---

## Reference models

The following tracked models (`krig3/models/`) demonstrate common
patterns:

| Model | Vertices | Triangles | Pattern |
|-------|----------|-----------|---------|
| `CannonBall.mdl` | 86 | 168 | Spherical mesh — shared vertices, radial normals |
| `Powerup.mdl` | 14 | 20 | Inner glow / outline cage |
| `Powerup2.mdl` | 14 | 20 | Variant of Powerup inner-glow pattern |
| `blueshot.mdl` | 6 | 8 | Small projectile — uniform color, silhouette outline only |
| `GreenShot.mdl` | 6 | 8 | Same pattern as blueshot |
| `Building.mdl` | 376 | 412 | Composite box mesh |
| `Ship.mdl` | 50 | 88 | Composite box mesh |
| `Enemy.mdl` | 46 | 78 | Composite box mesh |
| `Pinetree.mdl` | 23 | 30 | Layered flat planes (vegetation) |
| `Snowpine.mdl` | 23 | 30 | Layered flat planes (vegetation) |

---

## Engine loading

Models are loaded at startup from the `./models/` directory (relative
to the current working directory when the engine is launched). Every
`.mdl` file in that directory is automatically indexed by filename in a
static `modelHash`. When a Lua script calls
`obj:set_model("CannonBall.mdl")`, the engine looks up
`"CannonBall.mdl"` in that hash.

**Key facts:**
- All models are loaded once at startup — not lazy-loaded.
- Mesh data (`baseVertex`, `normal`, `triangle`) is **shared** across
  all instances of the same model.
- Per-object transformed data (`updatedVertex`, `lightIntensity`) is
  maintained by each `ModelGameObject` instance separately.
- If the `./models/` directory is absent, no models load and a debug
  message is printed — no crash.

---

## Lua API

```lua
obj:set_model("ModelName.mdl")   -- attach a model; filename must match a file in ./models/
```

Object properties that affect model rendering (set after `obj:load()`,
applied with `obj:save()`):

| Property | Effect |
|----------|--------|
| `always_lit` | If true, bypasses the normal/light dot product and fixes light intensity at 0.5 (mid-bright, no shading variation). Use for objects that should look evenly lit regardless of light direction. |
| `rotation` | Quaternion `{x, y, z, w}`. Applied to every vertex and normal at update time. |
| `scale` | Vector `{x, y, z}`. Applied to every vertex at update time. |
| `position` | Vector `{x, y, z}`. Applied as a translation at draw time. |

---

## C++ Loader Reference

From `ModelGameObject.h`:

```cpp
struct ModelStorage {
  GLuint numVertices;   // number of vertices in the model
  GLuint numTriangles;  // number of triangles in the model
  GLfloat initalScale;  // stored but never reapplied at runtime (see below)
  GLfloat** baseVertex; // per-vertex positions [numVertices][3]
  Vector* normal;       // per-vertex normals [numVertices]
  Triangle* triangle;   // per-triangle data [numTriangles]

  void load(char[]);
};
```

From `src/Triangle.h`:

```cpp
struct Triangle {
  GLint vertices[3];        // indices into baseVertex
  GLfloat colors[3][3];     // per-vertex color: colors[vertex][r/g/b]
};
```

The loader (`ModelGameObject.cpp`):

```cpp
void ModelStorage::load(char fileName[]) {
  std::ifstream fin(fileName);
  fin >> initalScale;
  fin >> numVertices;

  // allocate and read vertices
  for (int i = 0; i < numVertices; ++i) {
    fin >> baseVertex[i][0] >> baseVertex[i][1] >> baseVertex[i][2];  // x, y, z
    fin >> normal[i].x >> normal[i].y >> normal[i].z;                 // nx, ny, nz
  }

  fin >> numTriangles;

  // read triangles
  for (int i = 0; i < numTriangles; ++i) {
    fin >> triangle[i].vertices[0] >> triangle[i].vertices[1] >> triangle[i].vertices[2];
    for (int j = 0; j < 3; ++j)
      fin >> triangle[i].colors[j][0] >> triangle[i].colors[j][1] >> triangle[i].colors[j][2];
  }
}
```

---

## Details

### `initalScale`

A float value stored in the file but never reapplied by the runtime.
Vertices in the file should already be at their final scale. The field
exists for historical reasons. Set it to any value; `1.0` is
conventional.

Note: the spelling `initalScale` (missing an 'i') is a typo in the
original C++ source (`ModelGameObject.h`) that has been preserved here
intentionally to match the loader. It is not a documentation error.

### Vertex Block

For each vertex `i` from `0` to `numVertices-1`:

```text
x y z nx ny nz
```

- `x, y, z`: Position in model space. These are the base (unrotated,
  unscaled) positions.
- `nx, ny, nz`: Surface normal. Should point outward from the surface.
  The engine normalizes normals at update time, so any length works, but
  unit-length normals in the file are conventional. Normals are used for
  lighting only — they do not affect culling.

### Triangle Block

After vertices:

```text
numTriangles
```

Then for each triangle `t` from `0` to `numTriangles-1`:

```text
v0 v1 v2
c0_r c0_g c0_b
c1_r c1_g c1_b
c2_r c2_g c2_b
```

- `v0, v1, v2` are **integer indices** into the vertex array
  `[0..numVertices-1]`.
- `cN_*` are per-vertex colors for that triangle, in the range `[0, 1]`.

---

## Runtime transform pipeline

Understanding when each transform is applied is important for getting
triangle winding right.

### Update time (every frame, `ModelGameObject::update()`)

1. Build a combined rotation + scale matrix from the object's quaternion
   and scale vector.
2. Apply that matrix to every `baseVertex[i]` → store in
   `updatedVertex[i]`. This is the vertex position used for both drawing
   and collision.
3. Apply the same rotation matrix to every `normal[i]`, then normalize
   it.
4. Compute per-vertex **light intensity**:
   `dot(rotated_normal, light_direction)`, clamped to `[0, 1]`. Special
   case: if the dot product is exactly `1.0`, clamp to `0.5` instead.
   - If `always_lit` is true, light intensity is fixed at `0.5`
     regardless of normals or light direction.

### Draw time (`ModelGameObject::draw()`)

1. `glTranslatef(position_)` — translates to world position.
2. Iterate triangles; for each vertex of each triangle:
   - Set the vertex color: `glColor3fv(triangle[i].colors[j])`.
   - Set the 1D texture coordinate:
     `glTexCoord1f(lightIntensity[v])`. This samples the cel-shading
     lookup texture (a 16-step luminance ramp) to produce the posterized
     shading effect.
   - Emit the transformed vertex: `glVertex3fv(updatedVertex[v])`.

**Consequence for winding:** Triangle winding order is fixed in the file
and must be correct for the model's **default orientation** (before any
runtime rotation). Rotation and scale are applied to the already-loaded
`baseVertex` data every frame — the file is the source of truth for the
untransformed mesh.

---

## Rendering and triangle winding

The engine renders models under the following OpenGL state (set in
`Engine::initGL()`):

- **`glFrontFace(GL_CCW)`** — Triangles wound counter-clockwise in
  window space are front-facing.
- **`glCullFace(GL_BACK)`** — Back-facing triangles are culled (not
  drawn).

**Winding rule:** For a face to be visible from a given side, its
triangles must be wound **CCW when viewed from the outside**. If a face
is backwards-wound, it is culled and appears as a hole. Reverse the
winding (swap two vertex indices in each triangle of that face) to fix
it.

**Convention for "front" vs "back":**

- **+Z** = front of the model (visible when the camera is on the +Z
  side).
- **-Z** = back of the model (visible when the camera is on the -Z
  side).

**Box layout:** Order each face's vertices so that 0→1→2→3 is CCW when
viewed from outside, then use `(0,1,2)`, `(0,2,3)` for every face. Back
face at z=zb: 0=BR, 1=BL, 2=TL, 3=TR (CCW from -Z). Front face at
z=zf: 4=BL, 5=BR, 6=TR, 7=TL (CCW from +Z). Then:

- Back face: `(0,1,2)`, `(0,2,3)`.
- Front face: `(4,5,6)`, `(4,6,7)`.
- Left (-X): `(1,4,7)`, `(1,7,2)`.
- Right (+X): `(0,3,6)`, `(0,6,5)`.
- Top: `(2,3,6)`, `(2,6,7)`.
- Bottom: `(1,0,5)`, `(1,5,4)`.

**Mirrored boxes:** If you compute `x0 = +...` / `x1 = -...` per side,
you can accidentally reverse left/right ordering and silently invert
winding on one side. Prefer helpers that take min/max bounds before
emitting vertices, or build the part once in a local +X layout and
mirror positions only.

**Top caps of boxes:** Top faces must be CCW in the XZ plane when
viewed from +Y. If a top face is see-through from above, flip the top
triangles (swap the last two indices of each top triangle).

**Caps / fans (e.g. front and back caps on a gem):** The boundary
vertices are CCW from +Z. Front cap (normal +Z): `(center, i, j)`. Back
cap (normal -Z) needs the opposite winding since the same boundary is CW
from -Z: `(center, j, i)`. Both caps are then solid from their
respective sides.

**Annular band (two concentric rings):** Use an outer ring and an inner
ring.
- **Back face** (normal -Z): `(i, i-1, inner_i-1, inner_i)` →
  `(i, i-1, inner_i-1)`, `(i, inner_i-1, inner_i)`.
- **Front face** (normal +Z):
  `(outer_i, outer_i+1, inner_i+1, inner_i)` →
  `(outer_i, outer_i+1, inner_i+1)`, `(outer_i, inner_i+1, inner_i)`.
- **Outer cylindrical side:**
  `(back_i, back_i+1, front_i+1, front_i)`.
- **Inner cylindrical side** must also be drawn or the band will be
  see-through from inside.

**Layered glass / inner glow (e.g. `krig3/models/Powerup.mdl`):**
Combine an inner solid core (bright warm colors, fully closed) with a
thin outer frame or cage that leaves large openings. The outermost
surface must remain closed and CCW-wound so the outline pass draws a
clear silhouette. The inner core should be inset enough to avoid
z-fighting with the frame.

**Spherical meshes (e.g. `krig3/models/CannonBall.mdl`):** Use
latitude/longitude rings with shared vertices and a single pole at each
end. Set the normal at each vertex equal to the normalized position
offset from the sphere centre. Avoid stacking multiple shells at the
same radius — reuse the same vertices wherever possible so there is only
one closed surface.

---

## Cel-shading and outline pass

The engine achieves its cartoon look in three rendering passes (defined
as OpenGL display lists in `Engine::initGL()`):

### Pass 1 — Cel (shaded) pass

Normal render with `GL_TEXTURE_1D` enabled. The cel texture is a
16-step luminance ramp. The per-vertex light intensity (the dot product
of the rotated normal with the light direction) is passed as a 1D
texture coordinate (`glTexCoord1f`), which samples that ramp to produce
discrete shading bands.

### Pass 2 — Outline pass

```
glCullFace(GL_FRONT)           — cull front-facing triangles
glPolygonMode(GL_BACK, GL_LINE) — draw back-facing triangles as wireframe
glLineWidth(3.0f)
glDepthFunc(GL_LESS)
```

Back-facing triangles are slightly behind front-facing geometry in the
depth buffer, so the outline appears only at silhouette edges. This is
why **winding matters for outlines**: a backwards-wound face treated as
"front" is culled in the outline pass, placing a dark line where there
should be none, or missing a line at a real silhouette.

### Pass 3 — Reset

Restore fill, `GL_BACK` culling, and depth function for subsequent
objects.

Every extra edge is a potential outline. Use large, clean shapes for
primary silhouettes and add detail deliberately so outlines help
readability instead of turning into noise.

---

## Terrain-resting models

When designing models that will sit on terrain (trees, walls, statues):

- Call `obj:set_height_from_terrain(offset)` followed by
  `obj:orient_on_terrain(euler)` once, inside `on_load`. The engine
  samples terrain height at three control points derived from the
  model's collision box and tilts the object to align with that
  triangle's normal.
- Avoid calling `set_height_from_terrain()` again after additional
  rotations — repeated height sampling after rotation can lift or sink
  the model's base unexpectedly.
- Apply small visual lifts as a simple additive Y offset after the
  initial height/orientation pass rather than introducing additional
  terrain queries.

---

## Minimal Example

A minimal `.mdl` file for a single colored triangle:

```text
1.0          # initalScale (stored, not reapplied)
3            # numVertices
0.0 0.0 0.0  0.0 0.0 1.0    # v0: pos (0,0,0), normal (0,0,1)
1.0 0.0 0.0  0.0 0.0 1.0    # v1: pos (1,0,0), normal (0,0,1)
0.0 1.0 0.0  0.0 0.0 1.0    # v2: pos (0,1,0), normal (0,0,1)
1            # numTriangles
0 1 2        # triangle uses vertices 0, 1, 2
1.0 0.0 0.0  # color at v0 (red)
0.0 1.0 0.0  # color at v1 (green)
0.0 0.0 1.0  # color at v2 (blue)
```

---

## Creating models

1. Decide on vertices (`x,y,z`) and normals (`nx,ny,nz`) for the shape.
   Normals should point outward from the surface.
2. Define triangles with indices (`v0,v1,v2`) so each face is **CCW
   when viewed from the outside** (see **Rendering and triangle
   winding** above). Winding is defined for the model's default
   orientation before any runtime rotation.
3. Assign per-vertex colors for each triangle corner in `[0, 1]`.
4. Write the file in order: scale, numVertices, vertex block,
   numTriangles, triangle block.
5. Place the file in `./models/` so the engine picks it up at startup.

When a face is backwards-wound, it is culled and appears as a hole. The
fix is almost always to **swap two vertex indices** in each triangle of
that face — not to add additional geometry.
