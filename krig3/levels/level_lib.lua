-- Configuration
bossBattle   = 0
bossDefeated = 0
endTimer     = 10.0
debugEnabled = nil

-- Helper Functions
local function display_debug()
  if (debugEnabled == nil) then
    debugEnabled = krig.test_debug_enabled()
  end

  if (debugEnabled == 1) then
    player    = krig.get_player():load()
    camera    = krig.get_camera():load()
    plr_pos   = player.position
    cam_pos   = camera.position
    cam_dir   = camera.direction
    cam_up    = camera.up
    cam_rot   = krig.rotation.to_euler(camera.rotation)
    light_dir = krig.level.get_light_direction()

    gl.PushMatrix()
    gl.Translate (0.0, 0.0, -2.0)
    gl.Color (1.0, 1.0, 1.0)

    krig.render_text(string.format("plr pos: %.04f %.04f %.04f", plr_pos[1], plr_pos[2], plr_pos[3]), -1.0, 0.55)
    krig.render_text(string.format("cam pos: %.04f %.04f %.04f", cam_pos[1], cam_pos[2], cam_pos[3]), -1.0, 0.48)
    krig.render_text(string.format("cam dir: %.04f %.04f %.04f", cam_dir[1], cam_dir[2], cam_dir[3]), -1.0, 0.40)
    krig.render_text(string.format("cam up: %.04f %.04f %.04f", cam_up[1], cam_up[2], cam_up[3]), -1.0, 0.33)
    krig.render_text(string.format("cam rot: %.04f %.04f %.04f", cam_rot[1], cam_rot[2], cam_rot[3]), -1.0, 0.26)
    krig.render_text(string.format("fps: %.04f", krig.get_fps()), -1.0, 0.18)
    krig.render_text(string.format("cam id: %d", krig.level.get_camera_id()), -1.0, 0.10)

    krig.render_text(string.format("light dir: %.04f %.04f %.04f", light_dir[1], light_dir[2], light_dir[3]), -1.0, 0.0)
    gl.End()

    gl.PopMatrix()
  end
end

local function display_hud(bossBattle, bossLife)
  player = krig.get_player():load()

  ShipEnergy  = krig.get_script_value(player, "life")
  score       = krig.get_script_value(player, "score")
  numMissiles = krig.get_script_value(player, "numMissiles")
  EnemyEnergy = bossLife
  EnergyBar   = -0.31 - (0.068 * (10.0 - ShipEnergy))
  BossEnergy  = 0.31 + (0.017 * (40.0 - EnemyEnergy))

  plr_pos = player.position

  gl.PushMatrix()
  gl.Translate (0.0, 0.0, -2.0)
  gl.Color (1.0, 1.0, 1.0)
  krig.render_text("Lives: " .. krig.get_script_value(player, "lives"), -1.0, 0.7)

  krig.render_text("Score: " .. score, -.2, 0.7)
  krig.render_text("Missiles: " .. numMissiles, -1.0, .75)

  if bossBattle == 1 then
    krig.render_text("Enemy", 0.85, 0.7)
  end

  gl.Begin("QUADS")
  gl.Color (0.0, 0.3, 0.3)
  gl.Vertex (-0.99, 0.67, 0.001)
  gl.Vertex (-0.99, 0.64, 0.001)
  gl.Color (0.0, 1.0, 1.0)
  gl.Vertex (-0.31, 0.64, 0.001)
  gl.Vertex (-0.31, 0.67, 0.001)

  gl.Color (0.0, 0.0, 0.0)
  gl.Vertex (-0.31, 0.64, 0.002)
  gl.Vertex (-0.31, 0.67, 0.002)
  gl.Vertex (EnergyBar, 0.67, 0.002)
  gl.Vertex (EnergyBar, 0.64, 0.002)

  if bossBattle == 1 then
    gl.Color (1.0, 1.0, 0.0)
    gl.Vertex (0.31, 0.67, 0.001)
    gl.Vertex (0.31, 0.64, 0.001)
    gl.Color (0.3, 0.3, 0.0)
    gl.Vertex (0.99, 0.64, 0.001)
    gl.Vertex (0.99, 0.67, 0.001)

    gl.Color (0.0, 0.0, 0.0)
    gl.Vertex (BossEnergy, 0.64, 0.002)
    gl.Vertex (BossEnergy, 0.67, 0.002)
    gl.Vertex (0.31, 0.67, 0.002)
    gl.Vertex (0.31, 0.64, 0.002)
  end
  gl.End()

  gl.PopMatrix()
end

function update_level(timeElapsed, bossLife)
  if bossLife <= 0 then
    if bossDefeated == 0 then
      bossDefeated = 1
    elseif bossDefeated == 1 then
      endTimer = endTimer - timeElapsed
      if endTimer < 0.0 then
        krig.level.set_complete(1)
      end
    end
  end
end

local function buildCircleGroup(level, num, x, y, z)
  for i = 0, num, 1 do
    local obj = level:add_object("./scripts/enemy_ship10.lua")
    obj.position = {x + (i * 5.0), y, z}
    obj:save()
  end
end

local function buildVGroup(level, x, y, z)
  -- Leader
  local obj = level:add_object("./scripts/enemy_ship1.lua")
  obj.position = {x, y, z}
  obj:save()

  -- First two
  x  = x + 2
  y1 = y + 2
  y2 = y - 2

  obj = level:add_object("./scripts/enemy_ship1.lua")
  obj.position = {x, y1, z}
  obj:save()

  obj = level:add_object("./scripts/enemy_ship1.lua")
  obj.position = {x, y2, z}
  obj:save()

  -- Back two
  x  = x + 2
  y1 = y1 + 2
  y2 = y2 - 2

  obj = level:add_object("./scripts/enemy_ship1.lua")
  obj.position = {x, y1, z}
  obj:save()

  obj = level:add_object("./scripts/enemy_ship1.lua")
  obj.position = {x, y2, z}
  obj:save()
end

local function setup_volcano(level, xpos, zpos)
  level:add_object("./scripts/volcano.lua", {
    position = {xpos, 20.0, zpos}
  })

  for i = -15.0, 25.0, 10 do
    obj = level:add_object("./scripts/lavarock1.lua", {
      position    = {xpos, i, zpos},
      final_x_vel = 5.0
    })
  end

  for i = -20.0, 20.0, 10 do
    level:add_object("./scripts/lavarock1.lua", {
      position    = {xpos, i, zpos},
      final_x_vel = -5.0
    })
  end
end

local function setAsteroidWave(level, x_start, x_end)
  for i = x_start, x_end, 15 do
    local obj = level:add_object("./scripts/asteroid1.lua")
    obj.position = {i, 30.0, 7.5}
    obj:save()

    obj = level:add_object("./scripts/asteroid1.lua")
    obj.position = {(i + 5.0), 30.0, 7.5}
    obj:save()

    obj = level:add_object("./scripts/asteroid1.lua")
    obj.position = {(i + 10.0), 30.0, 7.5}
    obj:save()
  end
end

local function setSweepingFromAboveEnemyShips(level, x_start, x_end, x_step, y, z)
  for i = x_start, x_end, x_step do
    local obj = level:add_object("./scripts/enemy_ship2.lua")
    obj.position = {i, y, z}
    obj:save()
  end
end

local function setSweepingFromBelowEnemyShips(level, x_start, x_end, x_step, y, z)
  for i = x_start, x_end, x_step do
    local obj = level:add_object("./scripts/enemy_ship3.lua")
    obj.position = {i, y, z}
    obj:save()
  end
end

local function buildDockedGroup(level, x_start, x_end, x_step, y, z)
  for i = x_start, x_end, x_step do
    local obj = level:add_object("./scripts/enemy_ship11.lua")
    obj.position = {i, y, z}
    obj.rotation = {0.2, 0.0, 0.0}
    obj:save()

    obj = level:add_object("./scripts/enemy_ship11.lua")
    obj.position = {i + 5.0, y, z}
    obj.rotation = {0.2, 0.0, 0.0}
    obj:save()
  end
end

local function buildFlyingCircleUpGroup(level, num, space, x_start, y, z)
  for i = 0, num, 1 do
    obj = level:add_object("./scripts/enemy_ship12.lua")
    obj.position = {x_start + (i * space), y, z}
    obj:save()
  end
end

local function buildFlyingCircleDownGroup(level, num, space, x_start, y, z)
  for i = 0, num, 1 do
    local obj = level:add_object("./scripts/enemy_ship13.lua")
    obj.position = {x_start + (i * space), y, z}
    obj:save()
  end
end

level_lib = {
  display_debug                  = display_debug,
  display_hud                    = display_hud,
  update_level                   = update_level,
  buildCircleGroup               = buildCircleGroup,
  buildVGroup                    = buildVGroup,
  setup_volcano                  = setup_volcano,
  setAsteroidWave                = setAsteroidWave,
  setSweepingFromAboveEnemyShips = setSweepingFromAboveEnemyShips,
  setSweepingFromBelowEnemyShips = setSweepingFromBelowEnemyShips,
  buildDockedGroup               = buildDockedGroup,
  buildFlyingCircleUpGroup       = buildFlyingCircleUpGroup,
  buildFlyingCircleDownGroup     = buildFlyingCircleDownGroup
}

return level_lib
