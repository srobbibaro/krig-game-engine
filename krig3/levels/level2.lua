local level_lib = require './levels/level_lib'

-- Configuration
X_START_CAMERA = 110.0
X_START_PLAYER = X_START_CAMERA - 30.0

boss     = nil
bossLife = 0

-- Overridden Engine Callbacks
function on_load(terrain)
  krig.level.set_sky_box(
    0.0, 0.0, 0.3,
    0.6, 0.6, 0.6,
    1.0, 1.0, 1.0
  )

  krig.level.set_light_direction(0.25, 0.25, 0.5)

  krig.level.set_terrain(terrain, "./terrains/level2.txt")

  player = krig.get_player()
  krig.object.set_script(player, "./scripts/player1.lua")
  krig.object.set_position(player, X_START_PLAYER, 20.0, 7.5)

  camera = krig.get_camera()
  krig.object.set_script(camera, "./scripts/camera1.lua")
  krig.object.set_position(camera, X_START_CAMERA, 15.0, 35.0)

  krig.object.add_particle_system(camera, 2)

  -- gameplay obstacles (ships, asteroids, etc)
  setupEnemyShips()
  setupAsteroids()

  krig.level.play_music("./music/level2.ogg", 1)
end

function on_update(terrain, elapsedTime)
  if bossBattle == 0  then
    camera = krig.get_camera()
    cam_pos = krig.object.get_position(camera)

    if cam_pos[1] >= 1935.0 then
      bossBattle = 1

      -- Set the camera's velocity
      krig.object.set_velocity(camera, 0.0, 0.0, 0.0)

      -- Set the player's velocity
      player = krig.get_player()
      plr_vel = krig.object.get_velocity(player)
      krig.object.set_velocity(player, (plr_vel[1] - 10.0), plr_vel[2], plr_vel[3])

      -- Create the boss...
      boss = krig.level.add_object("./scripts/boss2.lua")
      krig.object.set_position(boss, 1960.0, 15.0, 7.5)
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

-- Helper Functions
function setupEnemyShips()
  -- Intro ships
  obj = krig.level.add_object("./scripts/enemy_ship1.lua")
  krig.object.set_position(obj, 160, 15, 7.5)

  obj = krig.level.add_object("./scripts/enemy_ship1.lua")
  krig.object.set_position(obj, 180, 22, 7.5)

  obj = krig.level.add_object("./scripts/enemy_ship1.lua")
  krig.object.set_position(obj, 200, 20, 7.5)

  obj = krig.level.add_object("./scripts/enemy_ship1.lua")
  krig.object.set_position(obj, 220, 10, 7.5)

  -- The trickster...
  obj = krig.level.add_object("./scripts/enemy_ship1.lua")
  krig.object.set_position(obj, 260, 17, 7.5)

  -- "V" groups.
  level_lib.buildVGroup(270, 17, 7.5)
  level_lib.buildVGroup(290, 10, 7.5)
  level_lib.buildVGroup(310, 17, 7.5)
  level_lib.buildVGroup(330, 10, 7.5)
  level_lib.buildVGroup(350, 17, 7.5)

  -- Interlacing swoops
  level_lib.setSweepingFromBelowEnemyShips(500, 570, 10, 22.0, 7.5)
  level_lib.setSweepingFromBelowEnemyShips(505, 575, 10, 8.0, 7.5)

  level_lib.buildCircleGroup(5, 900.0, 25.0, 7.5)
  level_lib.buildCircleGroup(5, 975.0, 20.0, 7.5)
  level_lib.buildCircleGroup(5, 1050.0, 15.0, 7.5)
  level_lib.buildCircleGroup(5, 1125.0, 10.0, 7.5)
  level_lib.buildCircleGroup(5, 1200.0, 15.0, 7.5)
  level_lib.buildCircleGroup(5, 1275.0, 20.0, 7.5)
  level_lib.buildCircleGroup(5, 1350.0, 25.0, 7.5)

  level_lib.buildDockedGroup(645, 890, 25, 30.25, -53.0)

  setupFlyingCircleEnemyShips()
end

function setupFlyingCircleEnemyShips()
  obj = krig.level.add_object("./scripts/enemy_ship12.lua")
  krig.object.set_position(obj, 1410.0, 5.0, 7.5)
  obj = krig.level.add_object("./scripts/enemy_ship12.lua")
  krig.object.set_position(obj, 1420.0, 8.0, 7.5)
  obj = krig.level.add_object("./scripts/enemy_ship12.lua")
  krig.object.set_position(obj, 1430.0, 11.0, 7.5)
  obj = krig.level.add_object("./scripts/enemy_ship12.lua")
  krig.object.set_position(obj, 1440.0, 13.0, 7.5)
  obj = krig.level.add_object("./scripts/enemy_ship12.lua")
  krig.object.set_position(obj, 1450.0, 13.0, 7.5)
  obj = krig.level.add_object("./scripts/enemy_ship12.lua")
  krig.object.set_position(obj, 1460.0, 11.0, 7.5)
  obj = krig.level.add_object("./scripts/enemy_ship12.lua")
  krig.object.set_position(obj, 1470.0, 8.0, 7.5)
  obj = krig.level.add_object("./scripts/enemy_ship12.lua")
  krig.object.set_position(obj, 1480.0, 5.0, 7.5)

  level_lib.buildFlyingCircleUpGroup(4, 5, 1510.0, 11.0, 7.5)
  level_lib.buildFlyingCircleUpGroup(4, 5, 1555.0, 8.0, 7.5)
  level_lib.buildFlyingCircleUpGroup(4, 5, 1600.0, 5.0, 7.5)

  level_lib.buildFlyingCircleUpGroup(4, 5, 1645.0, 15.0, 7.5)
  level_lib.buildFlyingCircleDownGroup(4, 5, 1645.0, 13.0, 7.5)

  level_lib.buildFlyingCircleDownGroup(7, 20, 1690.0, 20.0, 7.5)
  level_lib.buildFlyingCircleUpGroup(7, 20, 1700.0, 8.0, 7.5)
end

function setupAsteroids()
  -- First wave
  level_lib.setAsteroidWave(370, 460)
  -- Second wave
  level_lib.setAsteroidWave(620, 720)
end
