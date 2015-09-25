local level_lib = require './levels/level_lib'

-- Configuration
X_START_BOSS   = 995.0
X_START_CAMERA = 110.0
X_START_PLAYER = X_START_CAMERA - 20.0

boss     = nil
bossLife = 0

-- Overridden Engine Callbacks
function on_load(terrain)
  krig.level.set_sky_box(
    0.0, 0.0, 0.3,
    0.4, 0.4, 1.0,
    0.8, 0.8, 1.0
  )

  krig.level.set_light_direction(0.7723, 0.1792, 0.6094)

  krig.level.set_terrain(terrain, "./terrains/level1.txt")

  player = krig.get_player()
  player:set_script("./scripts/player1.lua")
  player.position = {X_START_PLAYER, 20.0, 7.5}
  player:save()

  camera = krig.get_camera()
  camera:set_script("./scripts/camera1.lua")
  camera.position = {X_START_CAMERA, 15.0, 35.0}
  camera:save()

  -- setup the enemy ships
  enemy_ship_positions = {
    {160, 15, 7.5}, {180, 20, 7.5}, {200, 10, 7.5}, {220, 15, 7.5}, {240, 20, 7.5},
    {260, 15, 7.5}, {270, 18, 7.5}, {270, 12, 7.5}, {280, 15, 7.5}, {300, 10, 7.5},
    {310, 7, 7.5},  {310, 13, 7.5}, {320, 10, 7.5}, {340, 20, 7.5}, {350, 23, 7.5},
    {350, 17, 7.5}, {360, 20, 7.5}, {390, 15, 7.5}, {390, 22, 7.5}, {390, 8, 7.5}
  }

  for i = 1, #enemy_ship_positions do
    local obj = krig.level.add_object("./scripts/enemy_ship1.lua")
    obj.position = krig.vector.copy(enemy_ship_positions[i])
    obj:save()
  end

  --  setup the asteroids --
  level_lib.setAsteroidWave(410.0, 560.0)

  --  sweeping enemy ships
  level_lib.setSweepingFromAboveEnemyShips(610, 635, 5, 22.0, 7.5)
  level_lib.setSweepingFromBelowEnemyShips(675, 700, 5, 8.0, 7.5)

  -- 2nd wave of asteroids --
  level_lib.setAsteroidWave(730.0, 810.0)

  -- last wave - crashing ships --
  enemy_ship_positions = {
    {882, 15, 37.5}, {880, 10, 37.5}, {890, 12, 37.5}
  }

  for i = 1, #enemy_ship_positions do
    obj = krig.level.add_object("./scripts/enemy_ship4.lua")
    obj.position = krig.vector.copy(enemy_ship_positions[i])
    obj:save()
  end

  enemy_ship_positions = {
    {900, 15, 37.5}, {905, 10, 37.5}, {910, 12, 37.5}
  }

  for i = 1, #enemy_ship_positions do
    obj = krig.level.add_object("./scripts/enemy_ship6.lua")
    obj.position = krig.vector.copy(enemy_ship_positions[i])
    obj:save()
  end

  -- scenery --
  building_setup = {
    { position = {160, 20, -75.0},  scale = {10.0, 20.0, 10.0} },
    { position = {190, 20, -100.0}, scale = {10.0, 30.0, 10.0} },
    { position = {150, 7, -50.0},   scale = {7.0, 10.0, 7.0} },
    { position = {175, 7, -50.0},   scale = {7.0, 7.0, 7.0} },
    { position = {200, 9, -50.0},   scale = {6.0, 9.0, 6.0} },
    { position = {210, 10, -50.0},  scale = {6.0, 10.0, 6.0} },
    { position = {230, 8, -50.0},   scale = {6.0, 8.0, 6.0} }
  }

  for i = 1, #building_setup do
    obj = krig.level.add_object("./scripts/building.lua")
    obj.position = krig.vector.copy(building_setup[i].position)
    obj.scale    = krig.vector.copy(building_setup[i].scale)
    obj:save()
  end

  -- place pine trees
  obj = krig.level.add_object("./scripts/pinetree.lua", {x_pos = 145.0, z_pos = -25.0})
  obj = krig.level.add_object("./scripts/pinetree.lua", {x_pos = 150.0, z_pos = -25.0})
  obj = krig.level.add_object("./scripts/pinetree.lua", {x_pos = 185.0, z_pos = -20.0})
  obj = krig.level.add_object("./scripts/pinetree.lua", {x_pos = 190.0, z_pos = -15.0})
  obj = krig.level.add_object("./scripts/pinetree.lua", {x_pos = 210.0, z_pos = -15.0})
  obj = krig.level.add_object("./scripts/pinetree.lua", {x_pos = 213.0, z_pos = -12.0})
  obj = krig.level.add_object("./scripts/pinetree.lua", {x_pos = 216.0, z_pos = -13.0})
  obj = krig.level.add_object("./scripts/pinetree.lua", {x_pos = 255.0, z_pos = -15.0})
  obj = krig.level.add_object("./scripts/pinetree.lua", {x_pos = 265.0, z_pos = -20.0})
  obj = krig.level.add_object("./scripts/pinetree.lua", {x_pos = 280.0, z_pos = -20.0})
  obj = krig.level.add_object("./scripts/pinetree.lua", {x_pos = 285.0, z_pos = -15.0})
  obj = krig.level.add_object("./scripts/pinetree.lua", {x_pos = 300.0, z_pos = -15.0})
  obj = krig.level.add_object("./scripts/pinetree.lua", {x_pos = 310.0, z_pos = -25.0})
  obj = krig.level.add_object("./scripts/pinetree.lua", {x_pos = 313.0, z_pos = -28.0})
  obj = krig.level.add_object("./scripts/pinetree.lua", {x_pos = 315.0, z_pos = -24.0})
  obj = krig.level.add_object("./scripts/pinetree.lua", {x_pos = 355.0, z_pos = -10.0})
  obj = krig.level.add_object("./scripts/pinetree.lua", {x_pos = 365.0, z_pos = -15.0})
  obj = krig.level.add_object("./scripts/pinetree.lua", {x_pos = 370.0, z_pos = -20.0})
  obj = krig.level.add_object("./scripts/pinetree.lua", {x_pos = 370.0, z_pos = -65.0, scale_factor = 4.0})
  obj = krig.level.add_object("./scripts/pinetree.lua", {x_pos = 375.0, z_pos = -60.0, scale_factor = 4.0})
  obj = krig.level.add_object("./scripts/pinetree.lua", {x_pos = 380.0, z_pos = -55.0, scale_factor = 4.0})
  obj = krig.level.add_object("./scripts/pinetree.lua", {x_pos = 705.0, z_pos = -45.0, scale_factor = 4.0})
  obj = krig.level.add_object("./scripts/pinetree.lua", {x_pos = 708.0, z_pos = -42.0, scale_factor = 4.0})
  obj = krig.level.add_object("./scripts/pinetree.lua", {x_pos = 715.0, z_pos = -45.0, scale_factor = 4.0})
  obj = krig.level.add_object("./scripts/pinetree.lua", {x_pos = 725.0, z_pos = -40.0, scale_factor = 4.0})
  obj = krig.level.add_object("./scripts/pinetree.lua", {x_pos = 735.0, z_pos = -45.0, scale_factor = 4.0})
  obj = krig.level.add_object("./scripts/pinetree.lua", {x_pos = 740.0, z_pos = -42.0, scale_factor = 4.0})
  obj = krig.level.add_object("./scripts/pinetree.lua", {x_pos = 980.0, z_pos = -75.0, scale_factor = 4.0})
  obj = krig.level.add_object("./scripts/pinetree.lua", {x_pos = 985.0, z_pos = -70.0, scale_factor = 4.0})
  obj = krig.level.add_object("./scripts/pinetree.lua", {x_pos = 990.0, z_pos = -80.0, scale_factor = 4.0})
  obj = krig.level.add_object("./scripts/pinetree.lua", {x_pos = 995.0, z_pos = -75.0, scale_factor = 4.0})

  krig.level.play_music("./music/level1.ogg", 1)
end

function on_update(terrain, elapsedTime)
  if bossBattle == 0  then
    camera = krig.get_camera():load()

    if camera.position[1] >= (X_START_BOSS - 25.0) then
      bossBattle = 1

      -- Set the camera's velocity
      camera.velocity = {0.0, 0.0, 0.0}
      camera:save()

      -- Set the player's velocity
      player = krig.get_player():load()
      player.velocity = {(player.velocity[1] - 10.0), player.velocity[2], player.velocity[3]}
      player:save()

      -- Create the boss...
      boss = krig.level.add_object("./scripts/boss1.lua")
      boss.position = {X_START_BOSS, 15.0, 7.5}
      boss:save()
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
