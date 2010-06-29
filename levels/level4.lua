dofile("./levels/level_lib.lua")

x_start_camera = 180.0 -- 110.0
x_start_player = x_start_camera - 20.0

boss = nil
bossLife = 0

function on_load(terrain)
    setSkyBox(0.0, 0.0, 0.0,
              0.0, 0.0, 0.3,
              1.0, 0.0, 0.5)

    setLightDirection(0.96, 0.07, 0.13)

    setTerrain(terrain, "./terrains/level4.txt")

    player = getPlayer()
    setScript(player, "./scripts/player1.lua")
    setPosition(player, x_start_player, 20.0, 77.5)

    camera = getCamera()
    setScript(camera, "./scripts/camera1.lua")
    setPosition(camera, x_start_camera, 15.0, 105.0)
    
    --playBgMusic("./music/Sparkman.ogg", 1)

    return
end

function on_update(terrain, elapsedTime)
    camera = getCamera()
    cam_pos = getPosition(camera)

    if cam_pos[1] >= 500.0 then
        setComplete(1)
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

