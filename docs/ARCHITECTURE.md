# Krig Game Engine — Architecture Overview

Krig is a C++ game engine with a Lua scripting layer. The engine
handles the game loop, rendering (OpenGL/GLUT), audio (OpenAL), and
scene management. All game logic — level setup, object behavior, and
progression — is authored in Lua scripts.

## High-level structure

```
┌─────────────────────────────────────────────────────┐
│                     Engine                          │
│  (global — g_KRIG_ENGINE; owns the game loop,      │
│   key state, sound effects, and the active level)   │
│                                                     │
│   ┌──────────────────────────────────────────────┐  │
│   │              GameLevel                       │  │
│   │  (scene graph; owns all objects and state    │  │
│   │   for one level; two instances kept for      │  │
│   │   seamless level transitions)                │  │
│   │                                              │  │
│   │  camera_ ──► Camera                          │  │
│   │  player_ ──► ModelGameObject                 │  │
│   │  terrain_ ─► Terrain                         │  │
│   │  music_  ──► Music                           │  │
│   │  objects_ ─► ObjectList (linked list)        │  │
│   │               ├─ ModelGameObject             │  │
│   │               ├─ TextGameObject              │  │
│   │               ├─ SpriteGameObject            │  │
│   │               └─ ...                         │  │
│   │  quadTree_ ─► QuadTree (terrain culling)     │  │
│   │  idToObjectMap_[] ── fast ID→Object lookup   │  │
│   └──────────────────────────────────────────────┘  │
│                                                     │
│  soundFx_ ────► SoundFX  (global sound effects)    │
│  luaState_ ───► lua_State (engine-level script)    │
└─────────────────────────────────────────────────────┘
```

## Object hierarchy

```
ObjectNode  (doubly-linked list node)
  └─ Object  (abstract; position, rotation/quaternion,
   │           velocity, bounding sphere, Lua state)
   ├─ ModelGameObject   3D mesh (.mdl format), cel-shaded
   ├─ Camera            view frustum, projection matrix
   ├─ Terrain           heightfield mesh; also in ObjectList
   ├─ TextGameObject    2D screen-space GLUT text
   └─ SpriteGameObject  2D textured quad
```

`Terrain` inheriting from `Object` is intentional — it lets it
participate in the generic object list and collision system without
special-casing in the engine.

## Game loop (per frame)

```
glutIdleFunc → display() → Engine::gameCycle()

1. Timer          getElapsedSeconds()  (spin-wait to cap FPS)
2. Lua update     engine on_update(dt)
3. Camera         Camera::update()
4. Animate        GameLevel::animateLevel()  →  Object::animate() per object
5. Update logic   GameLevel::updateLevel()   →  Object::update() per object
6. Prepare        GameLevel::prepareLevel()  →  collision detection, QuadTree cull
7. Render         GameLevel::drawLevel()     →  see Rendering below
8. Audio          Music::Update()            →  refill OGG stream buffers
9. Post-draw      postDraw()  →  Lua on_draw_screen() for UI
10. Swap          glutSwapBuffers()
11. Input reset   processCommands()  clears per-frame key states
```

## Lua integration

Engine and level each own a `lua_State*`. Each object can optionally
own a third child state for per-object scripts. The Lua API is exposed
under the `krig.*` namespace from `src/api_*.cpp`.

```
Engine::luaState_          game-level script  (main.lua)
GameLevel::luaState_       level script       (levels/levelN.lua)
Object::L_                 object script      (scripts/enemy.lua)
```

**Hooks called by the engine:**

| Scope  | Hook | Called when |
|--------|------|-------------|
| Engine | `on_load()` | game starts |
| Engine | `on_update(dt)` | every frame |
| Engine | `on_unload()` | game exits |
| Level  | `on_load(terrain)` | level starts |
| Level  | `on_update(terrain, dt)` | every frame |
| Level  | `on_draw(terrain, dt)` | after 3D render, before outlines |
| Level  | `on_draw_screen(terrain, dt)` | UI layer (post depth-clear) |
| Level  | `on_unload(terrain)` | level unloads |
| Object | `on_load(self, options)` | object created |
| Object | `on_update(self, dt)` | every frame if active |
| Object | `on_collision(self, other)` | collision detected |
| Object | `on_draw(self, camera, dt)` | custom draw (optional) |

Objects are passed to Lua as tables with an `id` field. Scripts call
`self:load()` / `self:save()` to sync C++ state into Lua and back. The
C++ side resolves the object via `idToObjectMap_[id]`.

## Rendering pipeline

The engine uses a manual cel-shading approach — no fragment shaders.
Three GL display-list states toggle rendering modes:

```
1. Cel pass   enable 1D luminance-ramp texture
              per-vertex: dot(normal, lightDir) → texture coord → posterized color
              draw all objects (ModelGameObject, Terrain)

2. Outline    cull FRONT faces, polygon mode LINE on BACK, 3 px black lines
              redraw all objects → silhouette outlines

3. Reset      restore fill, depth func, cull BACK

4. Post       clear depth, identity transform
              Lua on_draw_screen() → TextGameObject, HUD
```

The `QuadTree` partitions the terrain in the XZ plane. Each frame
`buildDisplayList()` walks the tree and frustum-tests each node, adding
visible patches to a `DisplayList` for the terrain draw call. It is
used **only for terrain culling** — object collision is a linear O(n²)
pass over `ObjectList`.

## Audio

`Music` streams a single OGG Vorbis file per level via double-buffered
OpenAL, refilled every frame. `SoundFX` pre-loads all OGG files in
`krig3/sounds/` at startup and triggers them by filename. Both use the
single OpenAL context created at engine init.

## Key design decisions

| Decision | Rationale |
|----------|-----------|
| Lua for all game logic | Rapid iteration; C++ is infrastructure only |
| Per-object Lua states | Isolated script contexts; no shared-state bugs between objects |
| Linked list for objects | Simple; works at krig3 scale (~50–100 objects) |
| QuadTree for terrain only | Object count is small; terrain geometry is the culling bottleneck |
| Polled input, not events | Lua scripts poll `krig.test_key_pressed()` during `on_update` |
| Two GameLevel pointers | Enables seamless level transitions via `swapLevel()` |
