# Krig Lua API Reference

Games are authored entirely in Lua. The engine exposes its API under the `krig` namespace. Objects and terrain instances are passed as tables; vectors as `{x, y, z}` tables; rotations as `{x, y, z, w}` quaternion tables.

## Conventions

**Vectors** are `{x, y, z}` tables. Pass and receive them in that form.

**Rotations** are quaternions: `{x, y, z, w}` tables. Build them with `krig.rotation.*`; do not construct by hand.

**Object references** are tables with an `id` field and methods attached. Obtain them from `krig.get_player()`, `krig.get_camera()`, `krig.level.add_object()`, or via hook arguments.

**`obj:load()` / `obj:save()`** — before reading object properties in `on_update`, call `obj:load()` to sync C++ state into the Lua table. After writing properties, call `obj:save()` to push them back to C++.

---

## `krig` — engine

| Function | Description |
|----------|-------------|
| `krig.get_player() → object` | Returns object reference to the player. |
| `krig.get_camera() → object` | Returns object reference to the active camera. |
| `krig.get_fps() → number` | Returns current frames per second. |
| `krig.get_mouse_coordinates() → number, number` | Returns `x, y` mouse position. |
| `krig.test_key_pressed(key: number) → bool` | True if `key` (ASCII code) is currently pressed. |
| `krig.test_key_released(key: number) → bool` | True if `key` was released this frame. |
| `krig.test_special_key_pressed(key: number) → bool` | True if a special key (arrow, F-key) is pressed. |
| `krig.test_special_key_released(key: number) → bool` | True if a special key was released this frame. |
| `krig.test_debug_enabled() → bool` | True if the engine was built with `DEBUG=1`. |
| `krig.play_sound(obj: object, filename: string)` | Play a sound effect at `obj`'s position. `filename` is relative to `krig3/sounds/`. |
| `krig.display_text(text: string, x: number, y: number, z: number, sx: number, sy: number)` | Add a screen-space text object at world position `(x, y, z)` with scale `(sx, sy)`. |
| `krig.render_text(text: string, x: number, y: number)` | Immediately render text at normalised screen coordinates `(x, y)`. |
| `krig.get_script_value(obj: object, name: string) → value` | Read a global variable named `name` from `obj`'s Lua script. |
| `krig.set_script_value(obj: object, name: string, value)` | Set a global variable named `name` in `obj`'s Lua script. |
| `krig.shutdown()` | Signal the engine to exit after the current frame. |

---

## `krig.level` — level management

| Function | Description |
|----------|-------------|
| `krig.level.load(path: string)` | Load a level script from `path` (relative to game directory). |
| `krig.level.load_from_buffer(lua: string)` | Load a level from a Lua string. Primarily used internally. |
| `krig.level.swap()` | Swap `currentLevel_` and `storedLevel_` (for level transitions). |
| `krig.level.pause()` | Pause the level update loop. |
| `krig.level.get_complete() → bool` | True if `set_complete(true)` has been called. |
| `krig.level.set_complete(complete: bool)` | Mark the level complete; `main.lua` typically advances to the next level. |
| `krig.level.get_id() → number` | Returns the level's numeric ID. |
| `krig.level.set_id(id: number)` | Set the level's numeric ID. |
| `krig.level.add_object(script: string, options: table) → object` | Spawn an object from `script` path. `options` is an optional table passed to the object's `on_load`. |
| `krig.level.add_text(script: string, text: string) → object` | Spawn a `TextGameObject` with initial string `text`. |
| `krig.level.add_sprite(script: string) → object` | Spawn a `SpriteGameObject`. |
| `krig.level.remove_object(obj: object)` | Move `obj` to the dead/free pool. **Known bugs — avoid in game scripts until fixed** (see caution below). |
| `krig.level.find_object_of_type(type_id: number) → object \| nil` | Return first active object whose `type_id` matches, or nil. |
| `krig.level.get_camera_id() → number` | Return the object ID of the active camera (always 0 after level load). |
| `krig.level.set_light_direction(vec: vector)` | Set global directional light. `vec` is normalised automatically. |
| `krig.level.get_light_direction() → vector` | Returns current light direction. |
| `krig.level.set_sky_box(r0: number, g0: number, b0: number, r1: number, g1: number, b1: number, r2: number, g2: number, b2: number)` | Set sky gradient as top, middle, and bottom RGB colours (0–1 each). |
| `krig.level.set_terrain(terrain: object, path: string)` | Load terrain data from `path` into `terrain` object. Call in `on_load`. |
| `krig.level.play_music(path: string, repeat: number)` | Stream OGG file at `path`. `repeat`: 1 = loop, 0 = once. |
| `krig.level.pause_music()` | Pause the current music track. |
| `krig.level.stop_music()` | Stop and unload the current music track. |

> **`remove_object` caution:** The remove/add flow has three known bugs: `remove_object` does not NULL-check the object reference (crash on a stale Lua ref); `idToObjectMap_` is not cleared when an object goes DEAD (stale refs can still resolve); reused objects are not assigned a new ID (same ID refers to different logical instances over time). Until these are fixed, avoid `remove_object` in game scripts. To "remove" an object, deactivate it instead: call `obj:load()`, set `active = false`, then `obj:save()`. These bugs are tracked as open issues.

---

## `krig.terrain` — terrain queries

Terrain functions operate on the currently loaded terrain. `x` and `z` are world-space coordinates.

| Function | Description |
|----------|-------------|
| `krig.terrain.get_height(x: number, z: number) → number` | Returns interpolated terrain height at `(x, z)`. |
| `krig.terrain.get_vertex_height(x: number, z: number) → number` | Returns the height of the nearest terrain vertex. |
| `krig.terrain.get_vertex_type(x: number, z: number) → number` | Returns the terrain type code at the nearest vertex. |
| `krig.terrain.get_vertex_color(x: number, z: number) → number, number, number` | Returns `r, g, b` of the nearest terrain vertex. |
| `krig.terrain.set_vertex_height(x: number, z: number, h: number)` | Set height of the nearest terrain vertex to `h`. |
| `krig.terrain.set_vertex_type(x: number, z: number, type: number)` | Set the terrain type code at the nearest vertex. |
| `krig.terrain.set_vertex_color(x: number, z: number, r: number, g: number, b: number)` | Set vertex colour at the nearest terrain vertex. |

---

## `krig.vector` — vector math

All functions take and return `{x, y, z}` vector tables.

| Function | Description |
|----------|-------------|
| `krig.vector.add(a: vector, b: vector) → vector` | Returns `a + b`. |
| `krig.vector.subtract(a: vector, b: vector) → vector` | Returns `a - b`. |
| `krig.vector.sum(a: vector, b: vector) → vector` | Alias for `add`. |
| `krig.vector.scale(v: vector, s: number) → vector` | Returns `v` scaled by `s`. |
| `krig.vector.scalar(v: vector, s: number) → vector` | Alias for `scale`. |
| `krig.vector.average(a: vector, b: vector) → vector` | Returns the component-wise average of `a` and `b`. |
| `krig.vector.dot_product(a: vector, b: vector) → number` | Returns the scalar dot product. |
| `krig.vector.cross_product(a: vector, b: vector) → vector` | Returns the cross product vector. |
| `krig.vector.normalize(v: vector) → vector` | Returns a unit-length copy of `v`. |
| `krig.vector.distance(a: vector, b: vector) → number` | Returns the scalar distance between `a` and `b`. |
| `krig.vector.copy(v: vector) → vector` | Returns a new copy of `v`. |

---

## `krig.rotation` — rotations (quaternions)

Rotations are `{x, y, z, w}` quaternion tables.

| Function | Description |
|----------|-------------|
| `krig.rotation.from_euler(vec: vector) → rotation` | Build a quaternion from Euler angles in `vec` (radians). |
| `krig.rotation.from_axis(vec: vector, angle: number) → rotation` | Build a quaternion from axis `vec` and `angle` (radians). |
| `krig.rotation.to_euler(q: rotation) → vector` | Returns Euler angles of `q` as a vector. |
| `krig.rotation.add(q1: rotation, q2: rotation) → rotation` | Compose rotations: returns `q1 * q2`. Non-commutative. Named "add" for historical reasons — it is quaternion multiplication, not component addition. |
| `krig.rotation.copy(q: rotation) → rotation` | Returns a new copy of `q`. |

---

## Object methods

Called on an object reference as `obj:method()`. Always call `obj:load()` before reading properties and `obj:save()` after writing them.

| Method | Description |
|--------|-------------|
| `obj:load()` | Sync C++ object state into the Lua table. Call at the start of `on_update`. |
| `obj:save()` | Push Lua table state back to C++. Call after modifying properties. |
| `obj:set_model(path: string)` | Load a `.mdl` mesh file for this object. |
| `obj:set_texture(path: string)` | Load a PNG texture (used by `SpriteGameObject`). |
| `obj:set_script(path: string)` | Attach a Lua script to this object. |
| `obj:set_height_from_terrain(offset: number)` | Set Y position to terrain height at current X/Z, plus `offset`. |
| `obj:orient_on_terrain(euler: vector)` | Tilt the object to match the terrain normal at its control triangle. Call after `set_height_from_terrain`. |
| `obj:suspend(seconds: number)` | Pause this object's `on_update` for `seconds`. |
| `obj:add_particle_system(script: string)` | Attach a particle system from `script`. |
| `obj:setup_interpolation(start: number, end: number, rate: number)` | Configure value interpolation. |
| `obj:update_interpolation_value()` | Step the interpolation by one frame. |

### Object properties

Read/write via the Lua table after `obj:load()`. Call `obj:save()` to apply changes.

| Property | Type | Description |
|----------|------|-------------|
| `position` | vector | World-space position. |
| `velocity` | vector | Position change per second (applied by engine each frame). |
| `speed` | vector | Alternate velocity (used by some movement paths). |
| `rotation` | rotation | Current orientation. |
| `rotation_velocity` | vector | Rotation change per second (Euler rates). |
| `scale` | vector | Per-axis scale. |
| `scale_rate` | vector | Scale change per second. |
| `direction` | vector | Forward vector (read-only; derived from rotation). Z is negated relative to a raw rotation result. |
| `up` | vector | Up vector (read-only; derived from rotation). |
| `orthogonal` | vector | Right vector (read-only; derived from rotation). |
| `type_id` | number | Integer tag for grouping objects (used by `find_object_of_type` and collision dispatch). |
| `bounding_sphere_radius` | number | Radius used for broad-phase collision. |
| `active` | bool | Whether the object receives updates. |
| `draw_enabled` | bool | Whether the object is rendered. |
| `in_view` | bool | Set by the engine after frustum culling (read-only). |
| `collision_detection_enabled` | bool | Whether this object participates in collision checks. |
| `always_lit` | bool | If true, bypasses cel-shading and renders at full brightness. |
| `interpolation_enabled` | bool | Whether value interpolation is active. |

---

## Script hooks

The engine calls these functions if defined in the appropriate script.

### Engine script (`main.lua` or equivalent)

```lua
function on_load()          end  -- game starts
function on_update(dt)      end  -- every frame; dt = elapsed seconds
function on_unload()        end  -- game exits
```

### Level script (`levels/levelN.lua`)

```lua
function on_load(terrain)               end  -- level starts; terrain is an object ref
function on_update(terrain, dt)         end  -- every frame
function on_draw(terrain, dt)           end  -- after 3D render, before outlines
function on_draw_screen(terrain, dt)    end  -- UI layer (after depth buffer clear)
function on_unload(terrain)             end  -- level unloads
```

### Object script (`scripts/enemy.lua` etc.)

```lua
function on_load(self, options)         end  -- object created; options from add_object
function on_update(self, dt)            end  -- every frame while active
function on_collision(self, other)      end  -- collision with another object
function on_draw(self, camera, dt)      end  -- custom draw (optional)
```
