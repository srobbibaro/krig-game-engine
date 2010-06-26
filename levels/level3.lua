dofile("./levels/level_lib.lua")

x_start_camera = 110.0 -- 110.0
x_start_player = x_start_camera - 20.0

--bossBattle = 0
boss = nil
bossLife = 0

function on_load(terrain)
    setSkyBox(0.8, 0.2, 0.5,
              0.4, 0.4, 0.6,
              0.7, 1.0, 0.2)

    setLightDirection(0.25, 0.25, 0.5)

    setTerrain(terrain, "./terrains/level3.txt")

    player = getPlayer()
    setScript(player, "./scripts/player1.lua")
    setPosition(player, x_start_player, 20.0, 7.5)

    camera = getCamera()
    setScript(camera, "./scripts/camera1.lua")
    setPosition(camera, x_start_camera, 15.0, 35.0)

    buildWaterStructure( terrain, 220.0, -75.0 )
    buildWaterStructure( terrain, 280.0, -100.0 )
    buildWaterStructure( terrain, 500.0, 0.0 )

    buildCircleGroup(terrain, 5, 170.0, 25.0, 7.5)
    buildCircleGroup(terrain, 5, 170.0, 15.0, 7.5)
    buildCircleGroup(terrain, 5, 250.0, 25.0, 7.5)
    buildCircleGroup(terrain, 5, 250.0, 15.0, 7.5)

    buildVGroup(terrain, 350.0, 17.0, 7.5)
    buildVGroup(terrain, 370.0, 10.0, 7.5)

    setup_volcano( terrain, 440.0, -75.0 )
    setup_volcano( terrain, 670.0, -115.0 )
    setup_volcano( terrain, 1620.0, -65.0 )
    setup_volcano( terrain, 2195.0, -105.0 )

--    playBgMusic("./music/Woodman.ogg", 1)

    return
end

function on_update(terrain, elapsedTime)
    if bossBattle == 0  then
        camera = getCamera()
        cam_pos = getPosition(camera)

        if cam_pos[1] >= 2300.0 then 
            bossBattle = 1

            -- Set the camera's velocity
            setVelocity(camera, 0.0, 0.0, 0.0)

            -- Set the player's velocity
            player = getPlayer()
            plr_vel = getVelocity(player)
            setVelocity(player, (plr_vel[1] - 10.0), plr_vel[2], plr_vel[3])
           
            -- Create the boss...
            --boss = addObject(terrain, "./scripts/base_object.lua")
            --setPosition(boss, 2350.0, 15.0, 7.5)
        end
    elseif bossBattle == 1 then
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
    return
end

function on_draw_screen()
    display_hud(bossBattle, bossLife)
    display_debug()
    return
end

function buildWaterStructure( terrain, xpos, zpos )
    local obj = addObject(terrain, "./scripts/beam_scenery.lua")
    setScale(obj, 5.0, 3.0, 5.0)
    setPosition(obj, xpos, 5.0, zpos)
    setRotation(obj, 0.0, 0.0, 1.57)

    obj = addObject(terrain, "./scripts/beam_scenery.lua")
    setScale(obj, 6.0, 3.0, 5.0)
    setPosition(obj, xpos-8.0, 5.0, zpos)
    setRotation(obj, 0.0, 0.0, 2.0)

    obj = addObject(terrain, "./scripts/beam_scenery.lua")
    setScale(obj, 5.0, 3.0, 5.0)
    setPosition(obj, xpos+5.0, 5.0, zpos+15)
    setRotation(obj, 0.0, 0.0, -2.0)
end
