local level_lib = require './levels/level_lib'

-- Configuration
X_START_BOSS   = 1935.0
X_START_CAMERA = 110.0
X_START_PLAYER = X_START_CAMERA - 15.0

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
  player:set_script("./scripts/player1.lua")
  player.position = {X_START_PLAYER, 20.0, 7.5}
  player:save()

  camera = krig.get_camera()
  camera:set_script("./scripts/camera1.lua")
  camera.position = {X_START_CAMERA, 15.0, 35.0}
  camera:save()

  camera:add_particle_system(2)

  -- gameplay obstacles (ships, asteroids, etc)
  setupEnemyShips()
  setupAsteroids()

  krig.level.play_music("./music/level2.ogg", 1)
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
      boss = krig.level.add_object("./scripts/boss2.lua", {position = {X_START_BOSS + 25.0, 15.0, 7.5}})
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
  enemy_ship_positions = {
    {160, 15, 7.5}, {180, 22, 7.5}, {200, 20, 7.5}, {220, 10, 7.5},
    -- The trickster...
    {260, 17, 7.5}
  }

  for i = 1, #enemy_ship_positions do
    local obj = krig.level.add_object("./scripts/enemy_ship1.lua")
    obj.position = krig.vector.copy(enemy_ship_positions[i])
    obj:save()
  end

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
  enemy_ship_positions = {
    {1410.0, 5.0, 7.5},  {1420.0, 8.0, 7.5},  {1430.0, 11.0, 7.5}, {1440.0, 13.0, 7.5},
    {1450.0, 13.0, 7.5}, {1460.0, 11.0, 7.5}, {1470.0, 8.0, 7.5},  {1480.0, 5.0, 7.5}
  }

  for i = 1, #enemy_ship_positions do
    local obj = krig.level.add_object("./scripts/enemy_ship12.lua")
    obj.position = krig.vector.copy(enemy_ship_positions[i])
    obj:save()
  end

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
