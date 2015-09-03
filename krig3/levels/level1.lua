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
  krig.object.set_script(player, "./scripts/player1.lua")
  krig.object.set_position(player, X_START_PLAYER, 20.0, 7.5)

  camera = krig.get_camera()
  krig.object.set_script(camera, "./scripts/camera1.lua")
  krig.object.set_position(camera, X_START_CAMERA, 15.0, 35.0)

  -- setup the enemy ships
  local obj = krig.level.add_object("./scripts/enemy_ship1.lua")
  krig.object.set_position(obj, 160, 15, 7.5)

  obj = krig.level.add_object("./scripts/enemy_ship1.lua")
  krig.object.set_position(obj, 180, 20, 7.5)

  obj = krig.level.add_object("./scripts/enemy_ship1.lua")
  krig.object.set_position(obj, 200, 10, 7.5)

  obj = krig.level.add_object("./scripts/enemy_ship1.lua")
  krig.object.set_position(obj, 220, 15, 7.5)

  obj = krig.level.add_object("./scripts/enemy_ship1.lua")
  krig.object.set_position(obj, 240, 20, 7.5)

  obj = krig.level.add_object("./scripts/enemy_ship1.lua")
  krig.object.set_position(obj, 260, 15, 7.5)

  obj = krig.level.add_object("./scripts/enemy_ship1.lua")
  krig.object.set_position(obj, 270, 18, 7.5)

  obj = krig.level.add_object("./scripts/enemy_ship1.lua")
  krig.object.set_position(obj, 270, 12, 7.5)

  obj = krig.level.add_object("./scripts/enemy_ship1.lua")
  krig.object.set_position(obj, 280, 15, 7.5)

  obj = krig.level.add_object("./scripts/enemy_ship1.lua")
  krig.object.set_position(obj, 300, 10, 7.5)

  obj = krig.level.add_object("./scripts/enemy_ship1.lua")
  krig.object.set_position(obj, 310, 7, 7.5)

  obj = krig.level.add_object("./scripts/enemy_ship1.lua")
  krig.object.set_position(obj, 310, 13, 7.5)

  obj = krig.level.add_object("./scripts/enemy_ship1.lua")
  krig.object.set_position(obj, 320, 10, 7.5)

  obj = krig.level.add_object("./scripts/enemy_ship1.lua")
  krig.object.set_position(obj, 340, 20, 7.5)

  obj = krig.level.add_object("./scripts/enemy_ship1.lua")
  krig.object.set_position(obj, 350, 23, 7.5)

  obj = krig.level.add_object("./scripts/enemy_ship1.lua")
  krig.object.set_position(obj, 350, 17, 7.5)

  obj = krig.level.add_object("./scripts/enemy_ship1.lua")
  krig.object.set_position(obj, 360, 20, 7.5)

  obj = krig.level.add_object("./scripts/enemy_ship1.lua")
  krig.object.set_position(obj, 390, 15, 7.5)

  obj = krig.level.add_object("./scripts/enemy_ship1.lua")
  krig.object.set_position(obj, 390, 22, 7.5)

  obj = krig.level.add_object("./scripts/enemy_ship1.lua")
  krig.object.set_position(obj, 390, 8, 7.5)

  --  setup the asteroids --
  level_lib.setAsteroidWave(410.0, 560.0)

  --  sweeping enemy ships
  level_lib.setSweepingFromAboveEnemyShips(610, 635, 5, 22.0, 7.5)
  level_lib.setSweepingFromBelowEnemyShips(675, 700, 5, 8.0, 7.5)

  -- 2nd wave of asteroids --
  level_lib.setAsteroidWave(730.0, 810.0)

  -- last wave - crashing ships --
  obj6 = krig.level.add_object("./scripts/enemy_ship4.lua")
  krig.object.set_position(obj6, 882, 15, 37.5)

  obj5 = krig.level.add_object("./scripts/enemy_ship4.lua")
  krig.object.set_position(obj5, 880, 10, 37.5)

  obj4 = krig.level.add_object("./scripts/enemy_ship4.lua")
  krig.object.set_position(obj4, 890, 12, 37.5)

  obj3 = krig.level.add_object("./scripts/enemy_ship6.lua")
  krig.object.set_position(obj3, 900, 15, 37.5)

  obj2 = krig.level.add_object("./scripts/enemy_ship6.lua")
  krig.object.set_position(obj2, 905, 10, 37.5)

  obj1 = krig.level.add_object("./scripts/enemy_ship6.lua")
  krig.object.set_position(obj1, 910, 12, 37.5)

  -- scenery --
  obj = krig.level.add_object("./scripts/building.lua")
  krig.object.set_position(obj, 160, 20, -75.0)
  krig.object.set_scale(obj, 10.0, 20.0, 10.0)

  obj = krig.level.add_object("./scripts/building.lua")
  krig.object.set_position(obj, 190, 20, -100.0)
  krig.object.set_scale(obj, 10.0, 30.0, 10.0)

  obj = krig.level.add_object("./scripts/building.lua")
  krig.object.set_position(obj, 150, 7, -50.0)
  krig.object.set_scale(obj, 7.0, 10.0, 7.0)

  obj = krig.level.add_object("./scripts/building.lua")
  krig.object.set_position(obj, 175, 7, -50.0)
  krig.object.set_scale(obj, 7.0, 7.0, 7.0)

  obj = krig.level.add_object("./scripts/building.lua")
  krig.object.set_position(obj, 200, 9, -50.0)
  krig.object.set_scale(obj, 6.0, 9.0, 6.0)

  obj = krig.level.add_object("./scripts/building.lua")
  krig.object.set_position(obj, 210, 10, -50.0)
  krig.object.set_scale(obj, 6.0, 10.0, 6.0)

  obj = krig.level.add_object("./scripts/building.lua")
  krig.object.set_position(obj, 230, 8, -50.0)
  krig.object.set_scale(obj, 6.0, 8.0, 6.0)

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
    camera = krig.get_camera()
    cam_pos = krig.object.get_position(camera)

    if cam_pos[1] >= (X_START_BOSS - 25.0) then
      bossBattle = 1

      -- Set the camera's velocity
      krig.object.set_velocity(camera, 0.0, 0.0, 0.0)

      -- Set the player's velocity
      player = krig.get_player()
      plr_vel = krig.object.get_velocity(player)
      krig.object.set_velocity(player, (plr_vel[1] - 10.0), plr_vel[2], plr_vel[3])

      -- Create the boss...
      boss = krig.level.add_object("./scripts/boss1.lua")
      krig.object.set_position(boss, X_START_BOSS, 15.0, 7.5)
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
