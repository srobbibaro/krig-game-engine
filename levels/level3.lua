dofile("./levels/level_lib.lua")

x_start_camera = 350.0 -- 110.0
x_start_player = x_start_camera - 20.0

bossBattle = 0
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
    
    local obj = addObject(terrain, "./scripts/volcano.lua")
    setPosition(obj, 440.0, 20.0, -75.0); 

    obj = addObject(terrain, "./scripts/lavarock1.lua")
    setPosition(obj, 440.0, 25.0, -75.0);

    obj = addObject(terrain, "./scripts/lavarock2.lua")
    setPosition(obj, 440.0, 20.0, -75.0);

    obj = addObject(terrain, "./scripts/lavarock1.lua")
    setPosition(obj, 440.0, 15.0, -75.0);

    obj = addObject(terrain, "./scripts/lavarock2.lua")
    setPosition(obj, 440.0, 10.0, -75.0);

    obj = addObject(terrain, "./scripts/lavarock1.lua")
    setPosition(obj, 440.0, 5.0, -75.0);

    obj = addObject(terrain, "./scripts/lavarock2.lua")
    setPosition(obj, 440.0, 0.0, -75.0);

    obj = addObject(terrain, "./scripts/lavarock1.lua")
    setPosition(obj, 440.0, -5.0, -75.0);

    obj = addObject(terrain, "./scripts/lavarock2.lua")
    setPosition(obj, 440.0, -10.0, -75.0);

    obj = addObject(terrain, "./scripts/lavarock1.lua")
    setPosition(obj, 440.0, -15.0, -75.0);

    obj = addObject(terrain, "./scripts/lavarock2.lua")
    setPosition(obj, 440.0, -20.0, -75.0);

--    playBgMusic("./music/Woodman.ogg", 1)

    return
end

function on_update(terrain, elapsedTime)
    camera = getCamera()
    cam_pos = getPosition(camera)

    if cam_pos[1] >= 2000.0 then
        --setComplete(1)
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

