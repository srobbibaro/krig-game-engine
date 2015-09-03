local level_lib = require './levels/level_lib'

-- Configuration
X_START_CAMERA = 110.0
X_START_PLAYER = X_START_CAMERA - 20.0

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
  krig.object.set_script(player, "./scripts/player1.lua")
  krig.object.set_position(player, X_START_PLAYER, 20.0, 7.5)

  camera = krig.get_camera()
  krig.object.set_script(camera, "./scripts/camera1.lua")
  krig.object.set_position(camera, X_START_CAMERA, 15.0, 35.0)

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

  krig.level.play_music("./music/level3.ogg", 1)
end

function on_update(terrain, elapsedTime)
  if bossBattle == 0  then
    camera = krig.get_camera()
    cam_pos = krig.object.get_position(camera)

    if cam_pos[1] >= 2300.0 then
      bossBattle = 1

      -- Set the camera's velocity
      krig.object.set_velocity(camera, 0.0, 0.0, 0.0)

      -- Set the player's velocity
      player = krig.get_player()
      plr_vel = krig.object.get_velocity(player)
      krig.object.set_velocity(player, (plr_vel[1] - 10.0), plr_vel[2], plr_vel[3])

      -- Create the boss...
      boss = krig.level.add_object("./scripts/boss3.lua")
      krig.object.set_position(boss, 2340.0, -20.0, 7.5)
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
  local obj = krig.level.add_object("./scripts/beam_scenery.lua")
  krig.object.set_scale(obj, 5.0, 3.0, 5.0)
  krig.object.set_position(obj, xpos, 5.0, zpos)
  krig.object.set_rotation(obj, 0.0, 0.0, 1.57)

  obj = krig.level.add_object("./scripts/beam_scenery.lua")
  krig.object.set_scale(obj, 6.0, 3.0, 5.0)
  krig.object.set_position(obj, xpos-8.0, 5.0, zpos)
  krig.object.set_rotation(obj, 0.0, 0.0, 2.0)

  obj = krig.level.add_object("./scripts/beam_scenery.lua")
  krig.object.set_scale(obj, 5.0, 3.0, 5.0)
  krig.object.set_position(obj, xpos+5.0, 5.0, zpos+15)
  krig.object.set_rotation(obj, 0.0, 0.0, -2.0)
end

function buildBossScenery()
  local obj = krig.level.add_object("./scripts/boss3_scenery.lua")
  krig.object.set_position(obj, 200.0, -15.0, -180.0 )

  obj = krig.level.add_object("./scripts/boss3_scenery.lua")
  krig.object.set_position(obj, 400.0, -15.0, -150.0 )

  obj = krig.level.add_object("./scripts/boss3_scenery.lua")
  krig.object.set_position(obj, 800.0, -15.0, -100.0 )

  obj = krig.level.add_object("./scripts/boss3_scenery.lua")
  krig.object.set_position(obj, 1100.0, -15.0, -60.0 )

  obj = krig.level.add_object("./scripts/boss3_scenery.lua")
  krig.object.set_position(obj, 1400.0, -15.0, -20.0 )
end

function setupSailboats()
  local obj = krig.level.add_object("./scripts/sail_boat.lua")
  krig.object.set_position(obj, 270.0, 0.0, -65.0 )

  obj = krig.level.add_object("./scripts/sail_boat.lua")
  krig.object.set_position(obj, 350.0, 0.0, -100.0 )

  obj = krig.level.add_object("./scripts/sail_boat.lua")
  krig.object.set_position(obj, 550.0, 0.0, -80.0 )
end

function setupCannonboats()
  local obj = krig.level.add_object("./scripts/cannon_boat.lua")
  krig.object.set_position(obj, 800.0, 0.0, -40.0 )

  obj = krig.level.add_object("./scripts/cannonball.lua")
  krig.object.set_position(obj, 800.0, 5.0, -38.5 )

  obj = krig.level.add_object("./scripts/cannon_boat.lua")
  krig.object.set_position(obj, 840.0, 0.0, -35.0 )

  obj = krig.level.add_object("./scripts/cannonball.lua")
  krig.object.set_position(obj, 840.0, 5.0, -33.5 )
end
