dofile("./levels/level_lib.lua")

x_start_camera = 610.0
x_start_player = x_start_camera -- 30.0
--x_start_boss   = x_start_camera + 10.0

bossBattle = 0
boss = nil
bossLife = 0

enemy_ship_wave = {
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


function buildCircleGroup(terrain, num, x, y, z)
    for i = 0, num, 1 do
        obj = addObject(terrain, "./scripts/enemy_ship10.lua")
        setPosition(obj, x + (i * 5.0), y, z)
    end
end

function buildDockedGroup(terrain)
    for i = 645, 890, 25 do
        obj = addObject(terrain, "./scripts/enemy_ship11.lua")
        setPosition(obj, i, 30.25, -53.0)
        setRotation(obj, 0.2, 0.0, 0.0)

        obj = addObject(terrain, "./scripts/enemy_ship11.lua")
        setPosition(obj, i + 5.0, 30.25, -53.0)
        setRotation(obj, 0.2, 0.0, 0.0)
    end
end

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

    buildCircleGroup(terrain, 5, 750.0, 25.0, 7.5)
    buildCircleGroup(terrain, 5, 825.0, 20.0, 7.5)
    buildCircleGroup(terrain, 5, 900.0, 15.0, 7.5)
    buildCircleGroup(terrain, 5, 975.0, 10.0, 7.5)
    buildCircleGroup(terrain, 5, 1050.0, 15.0, 7.5)
    buildCircleGroup(terrain, 5, 1125.0, 20.0, 7.5)
    buildCircleGroup(terrain, 5, 1200.0, 25.0, 7.5)

    buildDockedGroup(terrain)


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

    --addParticleSystem(camera, 2)

    -- gameplay obstacles (ships, asteroids, etc)
    setupEnemyShips(terrain)
    setupAsteroids(terrain)

--[[
    for i,v in ipairs(enemy_ship_wave) do
        obj = addObject(terrain, v.script)
        setPositionv(obj, v.position)
        --setVelocityv(obj, v.velocity)
    end
--]]


    -- scenery



    --obj = addObject(terrain, "./scripts/boss2.lua")
    --setPosition(obj, x_start_boss, 15.0, 7.5)

    --playBgMusic("./music/Woodman.ogg", 1)
    
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
