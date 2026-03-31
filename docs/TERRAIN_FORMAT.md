# Krig Terrain Format

Terrain in Krig is a uniform heightfield grid. Each vertex carries a height, an RGB colour, and a type code. The grid is stored in a plain-text `.txt` file and loaded at level start via `krig.level.set_terrain()`.

---

## File format

```
<xSize>
<zSize>
<scaleFactor>
<vertex data ...>
```

### Header fields

| Field | Type | Description |
|-------|------|-------------|
| `xSize` | integer | Number of vertices along the X axis |
| `zSize` | integer | Number of vertices along the Z axis |
| `scaleFactor` | float | World-space distance between adjacent vertices (metres) |

### Vertex data

After the header, vertices are written in Z-major, X-minor order:

```
for z in 0 .. zSize-1:
  for x in 0 .. xSize-1:
    write: height  r  g  b  type
```

Each vertex is five values:

| Field | Type | Range | Description |
|-------|------|-------|-------------|
| `height` | float | any | Y-axis elevation in world units |
| `r` | float | 0.0–1.0 | Red colour component |
| `g` | float | 0.0–1.0 | Green colour component |
| `b` | float | 0.0–1.0 | Blue colour component |
| `type` | integer | 0 or 1 | Terrain type code (see below) |

The parser uses whitespace-separated scanning (`>>` extraction), so values may be separated by spaces, tabs, or newlines. Both formats are valid:

**One value per line:**
```
256
32
5
0
0.6
0.4
0
0
...
```

**All vertex values on one line:**
```
54
44
10.0
96.7 0.52 0.50 0.48 0
52.3 0.50 0.48 0.47 0
...
```

---

## Coordinate system

After loading, vertex world-space coordinates are computed as:

| Axis | Formula |
|------|---------|
| X | `x * scaleFactor` |
| Y | loaded `height` value |
| Z | `-z * scaleFactor` (**negated**) |

The Z-axis negation means array index 0 maps to the back of the terrain in world space (positive Z) and increasing array indices move toward the camera (negative Z). All Lua terrain API calls use world-space X and Z coordinates, not array indices.

---

## Terrain type codes

| Type | Code | Behaviour |
|------|------|-----------|
| Land | 0 | Static; height and colour are stable after load |
| Water | 1 | Animated; height oscillates between 0 and 2 world units each frame |

### Water animation detail

When the file specifies type 1, the loader immediately randomises the vertex state:

- Height is set to a random value in [0, 2].
- The type is randomly assigned to 1, 2, or 3 (internal animation states): 60 % chance of 1, 20 % chance of 2, 20 % chance of 3.

At runtime, `Terrain::animate()` steps each water vertex:

- **Type 1** (at rest): random chance to start rising (transition to type 2).
- **Type 2** (rising): height increases by `elapsedTime` per frame until it reaches 2.0, then resets to type 1.
- **Type 3** (falling): height decreases by `elapsedTime` per frame until it reaches 0.0, then resets to type 1.

When saving, the engine normalises internal states: types 2 and 3 are written back as type 1, so the saved file only ever contains 0 or 1.

---

## Existing terrain files

| File | Dimensions | Scale | Description |
|------|-----------|-------|-------------|
| `krig3/terrains/level1.txt` | 256 × 32 | 5 | Flying game — open valley |
| `krig3/terrains/level2.txt` | 512 × 32 | 5 | Flying game — extended valley |
| `krig3/terrains/level3.txt` | 128 × 32 | 5 | Flying game — compact level |
| `krig3/terrains/level4.txt` | 512 × 32 | 5 | Flying game — extended level |

---

## Lua API

### Loading terrain

```lua
-- In level on_load(terrain):
krig.level.set_terrain(terrain, "./terrains/level1.txt")
```

The path is relative to the game directory (the argument passed to the `krig` executable).

### Querying the terrain

These functions take **world-space** coordinates:

```lua
krig.terrain.get_height(x, z) → number        -- bilinear-interpolated height at (x, z)
```

These functions take **grid** coordinates (integer vertex indices):

```lua
krig.terrain.get_vertex_height(x, z) → number
krig.terrain.get_vertex_color(x, z) → number, number, number   -- r, g, b
krig.terrain.get_vertex_type(x, z) → number
```

### Modifying the terrain

Modifications affect in-memory state immediately but do **not** invalidate cached lighting. Call `krig.level.set_terrain()` again or rely on the next light-direction update to refresh normals.

```lua
krig.terrain.set_vertex_height(x, z, height)
krig.terrain.set_vertex_color(x, z, r, g, b)
krig.terrain.set_vertex_type(x, z, type)
```

Out-of-bounds indices silently no-op — no error is raised.

### Placing objects relative to terrain

```lua
obj:set_height_from_terrain(offset)    -- set Y = terrain height at obj.position + offset
obj:orient_on_terrain(euler)           -- tilt obj to match terrain surface normal
```

Call `obj:set_height_from_terrain()` before `obj:orient_on_terrain()` so the normal is read at the correct position.

---

## Height queries

`krig.terrain.get_height(x, z)` bilinearly interpolates across the four vertices that surround `(x, z)`:

1. Convert world coordinates to terrain-array space: `col = x / scaleFactor`, `row = -z / scaleFactor`.
2. Identify bounding vertices: `col1 = floor(col)`, `col2 = col1 + 1`, `row1 = floor(row)`, `row2 = row1 + 1`.
3. Compute fractional offsets `perX = col - col1`, `perZ = row - row1`.
4. Interpolate:

```
th1 = lerp(height[col1][row1], height[col1][row2], perZ)
th2 = lerp(height[col2][row1], height[col2][row2], perZ)
result = lerp(th1, th2, perX)
```

---

## Curvature effect

When `isCurveEnabled_` is true, vertices far from the camera are pulled downward during rendering to simulate a curved horizon. This is a visual-only effect — it does not modify stored vertex data and does not affect height queries.

---

## Edit mode

Build with `make build-edit` (or set `EDIT=1` in `src/constants.h`) to enable the terrain editor. Edit mode opens a palette window alongside the game window.

### Palette controls

| UI element | Action |
|-----------|--------|
| R/G/B sliders | Set the colour that will be written to the selected vertex |
| Height sliders (0–10, 10–20, 20–30) | Set the height value for the next write |
| Land / Water buttons | Set the type code (0 or 1) |

The palette displays the current vertex's position, height, colour, and type live as you navigate the scene. Click controls to set values; the engine writes them to the terrain immediately.

Edit-mode changes are in-memory only. Saving requires a file-write step (not yet exposed to Lua).

---

## Generating terrain

Terrain files can be generated by any script that writes the header followed by per-vertex `height r g b type` records in Z-major, X-minor order (whitespace-delimited). The engine's file scanner is format-agnostic — values may be one per line or space-separated.

---

## QuadTree and rendering

The engine builds a `QuadTree` over the terrain's XZ extent. Each frame, `QuadTree::buildDisplayList()` frustum-tests each node's bounding sphere and populates a `DisplayList` of visible leaf nodes. `Terrain::draw()` then renders only those nodes.

Game scripts do not interact with the QuadTree directly. The terrain renders correctly without any setup beyond `krig.level.set_terrain()`.
