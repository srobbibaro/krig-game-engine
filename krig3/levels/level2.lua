dofile("./levels/level_lib.lua")

-- Configuration
X_START_CAMERA = 110.0
X_START_PLAYER = X_START_CAMERA - 30.0

boss     = nil
bossLife = 0

-- Overridden Engine Callbacks
function on_load(terrain)
  setSkyBox(
    0.0, 0.0, 0.3,
    0.6, 0.6, 0.6,
    1.0, 1.0, 1.0
  )

  setLightDirection(0.25, 0.25, 0.5)

  setTerrain(terrain, "./terrains/level2.txt")

  player = getPlayer()
  setScript(player, "./scripts/player1.lua")
  setPosition(player, X_START_PLAYER, 20.0, 7.5)

  camera = getCamera()
  setScript(camera, "./scripts/camera1.lua")
  setPosition(camera, X_START_CAMERA, 15.0, 35.0)

  addParticleSystem(camera, 2)

  -- gameplay obstacles (ships, asteroids, etc)
  setupEnemyShips()
  setupAsteroids()

  playBgMusic("./music/level2.ogg", 1)
end

function on_update(terrain, elapsedTime)
  if bossBattle == 0  then
    camera = getCamera()
    cam_pos = getPosition(camera)

    if cam_pos[1] >= 1935.0 then
      bossBattle = 1

      -- Set the camera's velocity
      setVelocity(camera, 0.0, 0.0, 0.0)

      -- Set the player's velocity
      player = getPlayer()
      plr_vel = getVelocity(player)
      setVelocity(player, (plr_vel[1] - 10.0), plr_vel[2], plr_vel[3])

      -- Create the boss...
      boss = addObject("./scripts/boss2.lua")
      setPosition(boss, 1960.0, 15.0, 7.5)
    end
  elseif bossBattle == 1 then
    bossLife = 0
    if boss ~= nil then bossLife = getScriptValue(boss, "life") end
    if bossLife == 0 then boss = nil end
    update_level(elapsedTime, bossLife)
  end
end

function on_draw_screen()
  if bossBattle == 1 then
    bossLife = 0
    if boss ~= nil then bossLife = getScriptValue(boss, "life") end
    if bossLife == 0 then boss = nil end
  end

  display_hud(bossBattle, bossLife)
  display_debug()
end

-- Helper Functions
function setupEnemyShips()
  -- Intro ships
  obj = addObject("./scripts/enemy_ship1.lua")
  setPosition(obj, 160, 15, 7.5)

  obj = addObject("./scripts/enemy_ship1.lua")
  setPosition(obj, 180, 22, 7.5)

  obj = addObject("./scripts/enemy_ship1.lua")
  setPosition(obj, 200, 20, 7.5)

  obj = addObject("./scripts/enemy_ship1.lua")
  setPosition(obj, 220, 10, 7.5)

  -- The trickster...
  obj = addObject("./scripts/enemy_ship1.lua")
  setPosition(obj, 260, 17, 7.5)

  -- "V" groups.
  buildVGroup(270, 17, 7.5)
  buildVGroup(290, 10, 7.5)
  buildVGroup(310, 17, 7.5)
  buildVGroup(330, 10, 7.5)
  buildVGroup(350, 17, 7.5)

  -- Interlacing swoops
  setSweepingFromBelowEnemyShips(500, 570, 10, 22.0, 7.5)
  setSweepingFromBelowEnemyShips(505, 575, 10, 8.0, 7.5)

  buildCircleGroup(5, 900.0, 25.0, 7.5)
  buildCircleGroup(5, 975.0, 20.0, 7.5)
  buildCircleGroup(5, 1050.0, 15.0, 7.5)
  buildCircleGroup(5, 1125.0, 10.0, 7.5)
  buildCircleGroup(5, 1200.0, 15.0, 7.5)
  buildCircleGroup(5, 1275.0, 20.0, 7.5)
  buildCircleGroup(5, 1350.0, 25.0, 7.5)

  buildDockedGroup(645, 890, 25, 30.25, -53.0)

  setupFlyingCircleEnemyShips()
end

function setupFlyingCircleEnemyShips()
  obj = addObject("./scripts/enemy_ship12.lua")
  setPosition(obj, 1410.0, 5.0, 7.5)
  obj = addObject("./scripts/enemy_ship12.lua")
  setPosition(obj, 1420.0, 8.0, 7.5)
  obj = addObject("./scripts/enemy_ship12.lua")
  setPosition(obj, 1430.0, 11.0, 7.5)
  obj = addObject("./scripts/enemy_ship12.lua")
  setPosition(obj, 1440.0, 13.0, 7.5)
  obj = addObject("./scripts/enemy_ship12.lua")
  setPosition(obj, 1450.0, 13.0, 7.5)
  obj = addObject("./scripts/enemy_ship12.lua")
  setPosition(obj, 1460.0, 11.0, 7.5)
  obj = addObject("./scripts/enemy_ship12.lua")
  setPosition(obj, 1470.0, 8.0, 7.5)
  obj = addObject("./scripts/enemy_ship12.lua")
  setPosition(obj, 1480.0, 5.0, 7.5)

  buildFlyingCircleUpGroup(4, 5, 1510.0, 11.0, 7.5)
  buildFlyingCircleUpGroup(4, 5, 1555.0, 8.0, 7.5)
  buildFlyingCircleUpGroup(4, 5, 1600.0, 5.0, 7.5)

  buildFlyingCircleUpGroup(4, 5, 1645.0, 15.0, 7.5)
  buildFlyingCircleDownGroup(4, 5, 1645.0, 13.0, 7.5)

  buildFlyingCircleDownGroup(7, 20, 1690.0, 20.0, 7.5)
  buildFlyingCircleUpGroup(7, 20, 1700.0, 8.0, 7.5)
end

function setupAsteroids()
  -- First wave
  setAsteroidWave(370, 460)
  -- Second wave
  setAsteroidWave(620, 720)
end
