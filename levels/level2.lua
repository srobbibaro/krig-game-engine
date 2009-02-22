dofile("./levels/level_lib.lua")

x_start_camera = 400.0
x_start_player = x_start_camera -- 30.0
--x_start_boss   = x_start_camera + 10.0

bossBattle = 0
boss = nil
bossLife = 0

function buildVGroup(terrain, x,y,z)
    -- Leader
    obj = addObject(terrain, "./scripts/enemy_ship1.lua")
    setPosition(obj, x, y, z)

    -- First two
    obj1 = addObject(terrain, "./scripts/enemy_ship1.lua")
    obj2 = addObject(terrain, "./scripts/enemy_ship1.lua")
    x = x + 2
    y1 = y + 2
    y2 = y - 2
    setPosition(obj1, x, y1, z)
    setPosition(obj2, x, y2, z)

    -- Back two
    obj1 = addObject(terrain, "./scripts/enemy_ship1.lua")
    obj2 = addObject(terrain, "./scripts/enemy_ship1.lua")
    x = x + 2
    y1 = y1 + 2
    y2 = y2 - 2
    setPosition(obj1, x, y1, z)
    setPosition(obj2, x, y2, z)
end

function setupEnemyShips(terrain)

    -- Intro ships
    obj = addObject(terrain, "./scripts/enemy_ship1.lua")
    setPosition(obj, 160, 15, 7.5)

    obj = addObject(terrain, "./scripts/enemy_ship1.lua")
    setPosition(obj, 180, 22, 7.5)

    obj = addObject(terrain, "./scripts/enemy_ship1.lua")
    setPosition(obj, 200, 20, 7.5)

    obj = addObject(terrain, "./scripts/enemy_ship1.lua")
    setPosition(obj, 220, 10, 7.5)

    -- The trickster...
    obj = addObject(terrain, "./scripts/enemy_ship1.lua")
    setPosition(obj, 260, 17, 7.5)

    -- "V" groups.
    buildVGroup(terrain, 270, 17, 7.5)
    buildVGroup(terrain, 290, 10, 7.5)
    buildVGroup(terrain, 310, 17, 7.5)
    buildVGroup(terrain, 330, 10, 7.5)
    buildVGroup(terrain, 350, 17, 7.5)

    -- Interlacing swoops
    for i = 500, 570, 10 do
        obj = addObject(terrain, "./scripts/enemy_ship2.lua")
        setPosition(obj, i, 22.0, 7.5)
    end

    for i = 505, 575, 10 do
        obj = addObject(terrain, "./scripts/enemy_ship3.lua")
        setPosition(obj, i, 8.0, 7.5)
    end
end

function setAsteroidWave(terrain, x_start, x_end)
    for i = x_start, x_end, 15 do
        obj = addObject(terrain, "./scripts/asteroid.lua")
	    setPosition(obj, i, 30.0, 7.5)

        obj = addObject(terrain, "./scripts/asteroid.lua")
	    setPosition(obj, (i + 5.0), 30.0, 7.5)

        obj = addObject(terrain, "./scripts/asteroid.lua")
	    setPosition(obj, (i + 10.0), 30.0, 7.5)
    end

    return
end

function setupAsteroids(terrain)
    -- First wave
    setAsteroidWave(terrain, 370, 460)
    -- Second wave
    setAsteroidWave(terrain, 620, 720)
end

function on_load(terrain)
    setSkyBox(0.0, 0.0, 0.3,
              0.6, 0.6, 0.6,
              1.0, 1.0, 1.0)

    setLightDirection(0.25, 0.25, 0.5)

    setTerrain(terrain, "./terrains/level2.txt")

    player = getPlayer()
    setScript(player, "./scripts/player1.lua")
    setPosition(player, x_start_player, 20.0, 7.5)

    camera = getCamera()
    setScript(camera, "./scripts/camera1.lua")
    setPosition(camera, x_start_camera, 15.0, 35.0)

    addParticleSystem(camera, 2)

    -- gameplay obstacles (ships, asteroids, etc)
    setupEnemyShips(terrain)
    setupAsteroids(terrain)

    -- scenery



    --obj = addObject(terrain, "./scripts/boss2.lua")
    --setPosition(obj, x_start_boss, 15.0, 7.5)

    --playBgMusic("./music/Woodman.ogg", 1)
    
    return
end

function on_update()
    return
end

function on_unload()
    return
end

function on_draw()
    return
end

function on_draw_screen()
    display_hud(0,0)
    display_debug()
    return
end
