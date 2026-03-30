# Claude instructions for krig-game-engine

Krig is a C++ game engine with a Lua scripting interface. The engine (`src/`) handles the game loop, rendering (OpenGL/GLUT), audio (OpenAL), physics, and scene management. Games are authored entirely in Lua scripts. Unit tests use Catch v1 in `test/`.

Before making any commits, read `CONTRIBUTING.md` for the required commit message format and architecture rules.

Before writing or modifying tests, read `docs/TESTING.md` for scope conventions and known counterintuitive behaviour the suite locks in.

Before touching math, spatial, or rotation code, read `docs/MATH_AND_TESTING_CONVENTIONS.md` for non-obvious engine behaviour (matrix layout, intersectBox, quaternion slerp, quadtree partitioning, etc.).

Before writing or reviewing Lua game scripts, or modifying any `api_*.cpp` file, read `docs/LUA_API.md` for the full Lua API surface and hook conventions.
