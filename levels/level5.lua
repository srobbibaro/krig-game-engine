dofile("./levels/level_lib.lua")

x_start_camera = 0.0 -- 110.0
x_start_player = x_start_camera - 20.0

boss_battle = 0
boss = nil
bossLife = 0
next_asteroid = 0.0
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

function on_load(terrain)
    setSkyBox(0.0, 0.0, 0.0,
              0.0, 0.0, 0.0,
              0.2, 0.0, 0.2)

    setLightDirection(0.96, 0.07, 0.13)

    player = getPlayer()
    setScript(player, "./scripts/player1.lua")
    setPosition(player, x_start_player, 20.0, 0.0)

    camera = getCamera()
    setScript(camera, "./scripts/camera1.lua")
    setPosition(camera, x_start_camera, 15.0, 27.5)

    for i,v in ipairs(asteroid_wave1) do
        obj = addObject(terrain, "./scripts/asteroid_space.lua")
        setPositionv(obj, v.position)
        setVelocityv(obj, v.velocity)
    end

    for i,v in ipairs(enemy_ship_wave1) do
        obj = addObject(terrain, v.script)
        setPositionv(obj, v.position)
        --setVelocityv(obj, v.velocity)
    end

    obj = addObject(terrain, "./scripts/redrock.lua")
    setPosition(obj, 175.0, 6.0, -30.0)
    setScale(obj, 25.0, 25.0, 25.0)

    obj = addObject(terrain, "./scripts/redrock.lua")
    setPosition(obj, 255.0, 9.0, -60.0)
    setRotation(obj, 3.14, 3.14, 3.14)
    setScale(obj, 35.0, 35.0, 35.0)

    obj = addObject(terrain, "./scripts/redrock.lua")
    setPosition(obj, 295.0, 20.0, -15.0)
    setRotation(obj, 3.14, 0.0, 1.57)
    setScale(obj, 15.0, 15.0, 15.0)

    obj = addObject(terrain, "./scripts/redrock.lua")
    setPosition(obj, 350.0, -22.0, -80.0)
    setScale(obj, 25.0, 25.0, 25.0)

    obj = addObject(terrain, "./scripts/redrock.lua")
    setPosition(obj, 430.0, 42.0, -70.0)
    setRotation(obj, 3.14, 0.0, 1.57)
    setScale(obj, 35.0, 35.0, 35.0)

    obj = addObject(terrain, "./scripts/redrock.lua")
    setPosition(obj, 400.0, 10.0, -45.0)
    setRotation(obj, 1.57, 1.57, 1.57)
    setScale(obj, 15.0, 15.0, 15.0)

    --playBgMusic("./music/Sparkman.ogg", 1)

    return
end

function on_update(terrain, elapsedTime)
    camera = getCamera()
    cam_pos = getPosition(camera)

    if boss_battle == 0 then
        if cam_pos[1] >= 800 then
            setVelocity(camera, 0.0, 0.0, 0.0)
            boss_battle = 1
            boss = addObject(terrain, "./scripts/boss5.lua")
            setPosition(boss, cam_pos[1] - 20.0, cam_pos[2], 0.0) 
        end
    elseif boss_battle == 1 then
        next_asteroid = next_asteroid - elapsedTime
        next_background = next_background - elapsedTime

        if next_asteroid <= 0.0 then
            obj = addObject(terrain, "./scripts/asteroid3.lua")
            --setScale(obj, math.random(2) + 2.0, math.random(2) + 2.0, math.random(2) + 2.0)
            setRotation(obj, math.random(3), math.random(3), math.random(3))
            setVelocity(obj, -28.0, 0.0, 0.0)
            next_asteroid = 0.1
        end        

--[[
        if next_background <= 0.0 then
            obj = addObject(terrain, "./scripts/asteroid3.lua")
            setScale(obj, math.random(10) + 45.0, math.random(10) + 25.0, math.random(10) + 25.0)
            setPosition(obj, cam_pos[1] + 20.0, cam_pos[2] + 10, -60)
            setRotation(obj, math.random(3), math.random(3), math.random(3))
            setVelocity(obj, -28.0, 0.0, 0.0)
            next_background = 4.0 
        end
--]]
        bossLife = 0
        if boss ~= nil then bossLife = getScriptValue(boss, "life") end
        if bossLife == 0 then boss = nil end
        update_level(elapsedTime, bossLife)
    end

    return
end

function on_unload()
    return
end

function on_draw()
--[[
    gl.PushMatrix()
    gl.Color (1.0, 1.0, 1.0)
    gl.Begin("QUADS")
    gl.Vertex (10.0, 10.0, 0.0)
    gl.Vertex (-10.0, 10.0, 0.0)
    gl.Vertex (-10.0, -10.0, 0.0)
    gl.Vertex (10.0, -10.0, 0.0)
    gl.End()
    gl.PopMatrix()
--]]
    return
end

function on_draw_screen()
    if boss_battle == 1 then
        bossLife = 0
        if boss ~= nil then bossLife = getScriptValue(boss, "life") end
        if bossLife == 0 then boss = nil end
    end

    display_hud(boss_battle, bossLife)
    display_debug()

    return
end

