local level_lib = require './levels/level_lib'

-- Configuration
X_START_BOSS   = 800
X_START_CAMERA = 0.0
X_START_PLAYER = X_START_CAMERA - 15.0

boss_battle     = 0
boss            = nil
bossLife        = 0
next_asteroid   = 0.0
next_background = 0.0

asteroid_wave1 = {
  {position = {50.0, 28.0, 0.0}, velocity = {0.0, 0.0, 0.0}},
  {position = {60.0, 12.0, 0.0}, velocity = {0.0, 0.0, 0.0}},
  {position = {70.0, 5.0, 0.0}, velocity = {0.0, 0.0, 0.0}},
  {position = {80.0, 15.0, 0.0}, velocity = {0.0, 0.0, 0.0}},
  {position = {94.0, 25.0, 0.0}, velocity = {0.0, 0.0, 0.0}},
  {position = {99.0, 3.0, 0.0}, velocity = {0.0, 0.0, 0.0}},
  {position = {110.0, 25.0, 0.0}, velocity = {0.0, 0.0, 0.0}},
  {position = {120.0, 12.0, 0.0}, velocity = {0.0, 0.0, 0.0}},
  {position = {130.0, 2.0, 0.0}, velocity = {0.0, 0.0, 0.0}},
  {position = {142.0, 11.0, 0.0}, velocity = {0.0, 0.0, 0.0}},
  {position = {152.0, 29.0, 0.0}, velocity = {0.0, 0.0, 0.0}},
  {position = {160.0, 5.0, 0.0}, velocity = {0.0, 0.0, 0.0}},
  {position = {166.0, 25.0, 0.0}, velocity = {0.0, 0.0, 0.0}},
  {position = {176.0, 16.0, 0.0}, velocity = {0.0, 0.0, 0.0}},
  {position = {185.0, 3.0, 0.0}, velocity = {0.0, 0.0, 0.0}},
  {position = {195.0, 17.0, 0.0}, velocity = {0.0, 0.0, 0.0}},
}

enemy_ship_wave1 = {
  {position = {230.0, 25.0, 0.0}, velocity = {0.0, 0.0, 0.0}, script = "./scripts/enemy_ship2.lua"},
  {position = {205.0, 15.0, 0.0}, velocity = {0.0, 0.0, 0.0}, script = "./scripts/enemy_ship7.lua"},
  {position = {230.0, 5.0, 0.0}, velocity = {0.0, 0.0, 0.0}, script = "./scripts/enemy_ship3.lua"},
  {position = {260.0, 22.0, 0.0}, velocity = {0.0, 0.0, 0.0}, script = "./scripts/enemy_ship2.lua"},
  {position = {235.0, 12.0, 0.0}, velocity = {0.0, 0.0, 0.0}, script = "./scripts/enemy_ship7.lua"},
  {position = {260.0, 2.0, 0.0}, velocity = {0.0, 0.0, 0.0}, script = "./scripts/enemy_ship3.lua"},
  {position = {290.0, 28.0, 0.0}, velocity = {0.0, 0.0, 0.0}, script = "./scripts/enemy_ship2.lua"},
  {position = {265.0, 18.0, 0.0}, velocity = {0.0, 0.0, 0.0}, script = "./scripts/enemy_ship7.lua"},
  {position = {290.0, 8.0, 0.0}, velocity = {0.0, 0.0, 0.0}, script = "./scripts/enemy_ship3.lua"},
  {position = {320.0, 25.0, 0.0}, velocity = {0.0, 0.0, 0.0}, script = "./scripts/enemy_ship2.lua"},
  {position = {295.0, 15.0, 0.0}, velocity = {0.0, 0.0, 0.0}, script = "./scripts/enemy_ship7.lua"},
  {position = {320.0, 5.0, 0.0}, velocity = {0.0, 0.0, 0.0}, script = "./scripts/enemy_ship3.lua"},
  {position = {350.0, 22.0, 0.0}, velocity = {0.0, 0.0, 0.0}, script = "./scripts/enemy_ship2.lua"},
  {position = {325.0, 12.0, 0.0}, velocity = {0.0, 0.0, 0.0}, script = "./scripts/enemy_ship7.lua"},
  {position = {350.0, 2.0, 0.0}, velocity = {0.0, 0.0, 0.0}, script = "./scripts/enemy_ship3.lua"},
  {position = {380.0, 28.0, 0.0}, velocity = {0.0, 0.0, 0.0}, script = "./scripts/enemy_ship2.lua"},
  {position = {355.0, 18.0, 0.0}, velocity = {0.0, 0.0, 0.0}, script = "./scripts/enemy_ship7.lua"},
  {position = {380.0, 8.0, 0.0}, velocity = {0.0, 0.0, 0.0}, script = "./scripts/enemy_ship3.lua"},
  {position = {400.0, 30.0, 0.0}, velocity = {0.0, 0.0, 0.0}, script = "./scripts/enemy_ship8.lua"},
  {position = {405.0, 30.0, 0.0}, velocity = {0.0, 0.0, 0.0}, script = "./scripts/enemy_ship8.lua"},
  {position = {410.0, 30.0, 0.0}, velocity = {0.0, 0.0, 0.0}, script = "./scripts/enemy_ship8.lua"},
  {position = {415.0, 30.0, 0.0}, velocity = {0.0, 0.0, 0.0}, script = "./scripts/enemy_ship8.lua"},
  {position = {420.0, 30.0, 0.0}, velocity = {0.0, 0.0, 0.0}, script = "./scripts/enemy_ship8.lua"},
  {position = {430.0, 30.0, 0.0}, velocity = {0.0, 0.0, 0.0}, script = "./scripts/enemy_ship8.lua"},
  {position = {435.0, 30.0, 0.0}, velocity = {0.0, 0.0, 0.0}, script = "./scripts/enemy_ship8.lua"},
  {position = {445.0, 30.0, 0.0}, velocity = {0.0, 0.0, 0.0}, script = "./scripts/enemy_ship8.lua"},
  {position = {450.0, 30.0, 0.0}, velocity = {0.0, 0.0, 0.0}, script = "./scripts/enemy_ship8.lua"},
  {position = {453.0, 30.0, 0.0}, velocity = {0.0, 0.0, 0.0}, script = "./scripts/enemy_ship8.lua"},
  {position = {456.0, 30.0, 0.0}, velocity = {0.0, 0.0, 0.0}, script = "./scripts/enemy_ship8.lua"},
  {position = {459.0, 30.0, 0.0}, velocity = {0.0, 0.0, 0.0}, script = "./scripts/enemy_ship8.lua"},
  {position = {490.0, 0.0, 0.0}, velocity = {0.0, 0.0, 0.0}, script = "./scripts/enemy_ship9.lua"},
  {position = {495.0, 0.0, 0.0}, velocity = {0.0, 0.0, 0.0}, script = "./scripts/enemy_ship9.lua"},
  {position = {500.0, 0.0, 0.0}, velocity = {0.0, 0.0, 0.0}, script = "./scripts/enemy_ship9.lua"},
  {position = {505.0, 0.0, 0.0}, velocity = {0.0, 0.0, 0.0}, script = "./scripts/enemy_ship9.lua"},
  {position = {510.0, 0.0, 0.0}, velocity = {0.0, 0.0, 0.0}, script = "./scripts/enemy_ship9.lua"},
  {position = {515.0, 0.0, 0.0}, velocity = {0.0, 0.0, 0.0}, script = "./scripts/enemy_ship9.lua"},
  {position = {520.0, 0.0, 0.0}, velocity = {0.0, 0.0, 0.0}, script = "./scripts/enemy_ship9.lua"},
  {position = {570.0, 10.0, 0.0}, velocity = {0.0, 0.0, 0.0}, script = "./scripts/enemy_ship10.lua"},
  {position = {575.0, 10.0, 0.0}, velocity = {0.0, 0.0, 0.0}, script = "./scripts/enemy_ship10.lua"},
  {position = {580.0, 10.0, 0.0}, velocity = {0.0, 0.0, 0.0}, script = "./scripts/enemy_ship10.lua"},
  {position = {630.0, 15.0, 0.0}, velocity = {0.0, 0.0, 0.0}, script = "./scripts/enemy_ship10.lua"},
  {position = {635.0, 15.0, 0.0}, velocity = {0.0, 0.0, 0.0}, script = "./scripts/enemy_ship10.lua"},
  {position = {640.0, 15.0, 0.0}, velocity = {0.0, 0.0, 0.0}, script = "./scripts/enemy_ship10.lua"},
  {position = {690.0, 20.0, 0.0}, velocity = {0.0, 0.0, 0.0}, script = "./scripts/enemy_ship10.lua"},
  {position = {695.0, 20.0, 0.0}, velocity = {0.0, 0.0, 0.0}, script = "./scripts/enemy_ship10.lua"},
  {position = {700.0, 20.0, 0.0}, velocity = {0.0, 0.0, 0.0}, script = "./scripts/enemy_ship10.lua"},
  {position = {750.0, 25.0, 0.0}, velocity = {0.0, 0.0, 0.0}, script = "./scripts/enemy_ship10.lua"},
  {position = {755.0, 25.0, 0.0}, velocity = {0.0, 0.0, 0.0}, script = "./scripts/enemy_ship10.lua"},
  {position = {760.0, 25.0, 0.0}, velocity = {0.0, 0.0, 0.0}, script = "./scripts/enemy_ship10.lua"},
}

-- Overridden Engine Callbacks
function on_load(terrain)
  krig.level.set_sky_box(
    0.2, 0.0, 0.2,
    0.0, 0.0, 0.0,
    0.2, 0.0, 0.2
  )

  krig.level.set_light_direction(0.96, 0.07, 0.13)

  player = krig.get_player()
  player:set_script("./scripts/player1.lua")
  player.position = {X_START_PLAYER, 20.0, 0.0}
  player:save()

  camera = krig.get_camera()
  camera:set_script("./scripts/camera1.lua")
  camera.position = {X_START_CAMERA, 15.0, 27.5}
  camera:save()

  camera:add_particle_system(1)

  for i, v in ipairs(asteroid_wave1) do
    krig.level.add_object("./scripts/asteroid_space.lua", v)
  end

  for i, v in ipairs(enemy_ship_wave1) do
    krig.level.add_object(v.script, v)
  end

  krig.level.add_object("./scripts/redrock.lua", {
    position = {175.0, 6.0, -30.0},
    scale    = {25.0, 25.0, 25.0}
  })

  krig.level.add_object("./scripts/redrock.lua", {
    position = {255.0, 9.0, -60.0},
    rotation = krig.rotation.from_euler({3.14, 3.14, 3.14}),
    scale    = {35.0, 35.0, 35.0}
  })

  krig.level.add_object("./scripts/redrock.lua", {
    position = {295.0, 20.0, -15.0},
    rotation = krig.rotation.from_euler({3.14, 0.0, 1.57}),
    scale    = {15.0, 15.0, 15.0}
  })

  krig.level.add_object("./scripts/redrock.lua", {
    position = {350.0, -22.0, -80.0},
    scale    = {25.0, 25.0, 25.0}
  })

  krig.level.add_object("./scripts/redrock.lua", {
    position = {430.0, 42.0, -70.0},
    rotation = krig.rotation.from_euler({3.14, 0.0, 1.57}),
    scale    = {35.0, 35.0, 35.0}
  })

  krig.level.add_object("./scripts/redrock.lua", {
    position = {400.0, 10.0, -45.0},
    rotation = krig.rotation.from_euler({1.57, 1.57, 1.57}),
    scale    = {15.0, 15.0, 15.0}
  })

  krig.level.play_music("./music/level.ogg", 1)
end

function on_update(terrain, elapsedTime)
  if boss_battle == 0 then
    camera = krig.get_camera():load()

    if camera.position[1] >= X_START_BOSS then
      boss_battle = 1

      -- Set the camera's velocity
      camera.velocity = {0.0, 0.0, 0.0}
      camera:save()

      -- Create the boss
      boss = krig.level.add_object("./scripts/boss5.lua", {
        position = {camera.position[1] - 20.0, camera.position[2], 0.0}
      })
    end
  elseif boss_battle == 1 then
    next_asteroid   = next_asteroid - elapsedTime
    next_background = next_background - elapsedTime

    if next_asteroid <= 0.0 then
      krig.level.add_object("./scripts/asteroid3.lua", {
        rotation = krig.rotation.from_euler({math.random(3), math.random(3), math.random(3)}),
        velocity = {-28.0, 0.0, 0.0}
      })
      next_asteroid = 0.1
    end

    bossLife = 0
    if boss ~= nil then bossLife = krig.get_script_value(boss, "life") end
    if bossLife == 0 then boss = nil end
    level_lib.update_level(elapsedTime, bossLife)
  end
end

function on_draw_screen()
  if boss_battle == 1 then
    bossLife = 0
    if boss ~= nil then bossLife = krig.get_script_value(boss, "life") end
    if bossLife == 0 then boss = nil end
  end

  level_lib.display_hud(boss_battle, bossLife)
  level_lib.display_debug()
end
