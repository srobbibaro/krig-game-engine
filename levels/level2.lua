dofile("./levels/level_lib.lua")

x_start_camera = 110.0
x_start_player = x_start_camera - 30.0

boss     = nil
bossLife = 0

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
    setSweepingFromBelowEnemyShips(terrain, 500, 570, 10, 22.0, 7.5)
    setSweepingFromBelowEnemyShips(terrain, 505, 575, 10, 8.0, 7.5)

    buildCircleGroup(terrain, 5, 900.0, 25.0, 7.5)
    buildCircleGroup(terrain, 5, 975.0, 20.0, 7.5)
    buildCircleGroup(terrain, 5, 1050.0, 15.0, 7.5)
    buildCircleGroup(terrain, 5, 1125.0, 10.0, 7.5)
    buildCircleGroup(terrain, 5, 1200.0, 15.0, 7.5)
    buildCircleGroup(terrain, 5, 1275.0, 20.0, 7.5)
    buildCircleGroup(terrain, 5, 1350.0, 25.0, 7.5)

    buildDockedGroup(terrain, 645, 890, 25, 30.25, -53.0)

    setupFlyingCircleEnemyShips(terrain)
end

function setupFlyingCircleEnemyShips(terrain)
    obj = addObject(terrain, "./scripts/enemy_ship12.lua")
    setPosition(obj, 1410.0, 5.0, 7.5)
    obj = addObject(terrain, "./scripts/enemy_ship12.lua")
    setPosition(obj, 1420.0, 8.0, 7.5)
    obj = addObject(terrain, "./scripts/enemy_ship12.lua")
    setPosition(obj, 1430.0, 11.0, 7.5)
    obj = addObject(terrain, "./scripts/enemy_ship12.lua")
    setPosition(obj, 1440.0, 13.0, 7.5)
    obj = addObject(terrain, "./scripts/enemy_ship12.lua")
    setPosition(obj, 1450.0, 13.0, 7.5)
    obj = addObject(terrain, "./scripts/enemy_ship12.lua")
    setPosition(obj, 1460.0, 11.0, 7.5)
    obj = addObject(terrain, "./scripts/enemy_ship12.lua")
    setPosition(obj, 1470.0, 8.0, 7.5)
    obj = addObject(terrain, "./scripts/enemy_ship12.lua")
    setPosition(obj, 1480.0, 5.0, 7.5)

    buildFlyingCircleUpGroup(terrain, 4, 5, 1510.0, 11.0, 7.5)
    buildFlyingCircleUpGroup(terrain, 4, 5, 1555.0, 8.0, 7.5)
    buildFlyingCircleUpGroup(terrain, 4, 5, 1600.0, 5.0, 7.5)

    buildFlyingCircleUpGroup(terrain, 4, 5, 1645.0, 15.0, 7.5)
    buildFlyingCircleDownGroup(terrain, 4, 5, 1645.0, 13.0, 7.5)

    buildFlyingCircleDownGroup(terrain, 7, 20, 1690.0, 20.0, 7.5)
    buildFlyingCircleUpGroup(terrain, 7, 20, 1700.0, 8.0, 7.5)
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

    playBgMusic("./music/level2.ogg", 1)
    
    return
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
            boss = addObject(terrain, "./scripts/boss2.lua")
            setPosition(boss, 1960.0, 15.0, 7.5)
        elseif bossBattle == 1 then
            bossLife = 0
            if boss ~= nil then bossLife = getScriptValue(boss, "life") end
            if bossLife == 0 then boss = nil end
            update_level(elapsedTime, bossLife)
        end 
    end

    return
end

function on_unload()
    return
end

function on_draw()
    return
end

function on_draw_screen()
    if bossBattle == 1 then
        bossLife = 0
        if boss ~= nil then bossLife = getScriptValue(boss, "life") end
        if bossLife == 0 then boss = nil end
    end

    display_hud(bossBattle, bossLife)
    display_debug()
    return
end
