local level_lib = require './levels/level_lib'

-- Configuration
X_START_BOSS   = 2300
X_START_CAMERA = 110.0
X_START_PLAYER = X_START_CAMERA - 15.0

boss     = nil
bossLife = 40

-- Overridden Engine Callbacks
function on_load(terrain)
  krig.level.set_sky_box(
    0.8, 0.2, 0.5,
    0.4, 0.4, 0.6,
    0.7, 1.0, 0.2
  )

  krig.level.set_light_direction(0.25, 0.25, 0.5)

  krig.level.set_terrain(terrain, "./terrains/level3.txt")

  player = krig.get_player()
  player:set_script("./scripts/player1.lua")
  player.position = {X_START_PLAYER, 20.0, 7.5}
  player:save()

  camera = krig.get_camera()
  camera:set_script("./scripts/camera1.lua")
  camera.position = {X_START_CAMERA, 15.0, 35.0}
  camera:save()

  setupSailboats()
  setupCannonboats()

  buildWaterStructure(220.0, -75.0)
  buildWaterStructure(280.0, -100.0)
  buildWaterStructure(500.0, 0.0)

  level_lib.buildCircleGroup(5, 170.0, 25.0, 7.5)
  level_lib.buildCircleGroup(5, 170.0, 15.0, 7.5)
  level_lib.buildCircleGroup(5, 250.0, 25.0, 7.5)
  level_lib.buildCircleGroup(5, 250.0, 15.0, 7.5)

  level_lib.buildVGroup(350.0, 17.0, 7.5)
  level_lib.buildVGroup(370.0, 10.0, 7.5)

  level_lib.setup_volcano(440.0, -75.0)
  level_lib.setup_volcano(670.0, -115.0)
  level_lib.setup_volcano(1620.0, -65.0)
  level_lib.setup_volcano(2195.0, -105.0)

  buildBossScenery()

  krig.level.play_music("./music/level.ogg", 1)
end

function on_update(terrain, elapsedTime)
  if bossBattle == 0  then
    camera = krig.get_camera():load()

    if camera.position[1] >= X_START_BOSS then
      bossBattle = 1

      -- Set the camera's velocity
      camera.velocity = {0.0, 0.0, 0.0}
      camera:save()

      -- Set the player's velocity
      player = krig.get_player():load()
      player.velocity[1] = player.velocity[1] - 10.0
      player:save()

      -- Create the boss...
      boss = krig.level.add_object("./scripts/boss3.lua", {
        position = {X_START_BOSS + 40, -20.0, 7.5}
      })
    end
  elseif bossBattle == 1 then
    bossLife = 0
    if boss ~= nil then bossLife = krig.get_script_value(boss, "life") end
    if bossLife == 0 then boss = nil end
    level_lib.update_level(elapsedTime, bossLife)
  end
end

function on_draw_screen()
  if bossBattle == 1 then
    bossLife = 0
    if boss ~= nil then bossLife = krig.get_script_value(boss, "life") end
    if bossLife == 0 then boss = nil end
  end

  level_lib.display_hud(bossBattle, bossLife)
  level_lib.display_debug()
end

function buildWaterStructure(xpos, zpos)
  krig.level.add_object("./scripts/beam_scenery.lua", {
    scale    = {5.0, 3.0, 5.0},
    position = {xpos, 5.0, zpos},
    rotation = krig.rotation.from_euler({0.0, 0.0, 1.57})
  })

  krig.level.add_object("./scripts/beam_scenery.lua", {
    scale    = {6.0, 3.0, 5.0},
    position = {xpos-8.0, 5.0, zpos},
    rotation = krig.rotation.from_euler({0.0, 0.0, 2.0})
  })

  krig.level.add_object("./scripts/beam_scenery.lua", {
    scale    = {5.0, 3.0, 5.0},
    position = {xpos+5.0, 5.0, zpos+15},
    rotation = krig.rotation.from_euler({0.0, 0.0, -2.0})
  })
end

function buildBossScenery()
  local boss_scenery_positions = {
    {200.0, -15.0, -180.0},
    {400.0, -15.0, -150.0},
    {800.0, -15.0, -100.0},
    {1100.0, -15.0, -60.0},
    {1400.0, -15.0, -20.0}
  }

  for i = 1, #boss_scenery_positions do
    krig.level.add_object("./scripts/boss3_scenery.lua", {
      position = boss_scenery_positions[i]
    })
  end
end

function setupSailboats()
  local sail_boat_positions = {
    {270.0, 0.0, -65.0},
    {350.0, 0.0, -100.0},
    {550.0, 0.0, -80.0}
  }

  for i = 1, #sail_boat_positions do
    krig.level.add_object("./scripts/sail_boat.lua", {
      position = sail_boat_positions[i]
    })
  end
end

function setupCannonboats()
  local boat_cannon_positions = {
    {800.0, 0.0, -40.0},
    {800.0, 5.0, -38.5},
    {840.0, 0.0, -35.0},
    {840.0, 5.0, -33.5}
  }

  for i = 1, #boat_cannon_positions do
    krig.level.add_object("./scripts/cannon_boat.lua", {
      position = boat_cannon_positions[i]
    })
  end
end
