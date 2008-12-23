dofile("./levels/level_lib.lua")

x_start_camera = 50.0
x_start_player = x_start_camera - 40.0

function on_load(terrain)
    setSkyBox(0.0, 0.0, 0.0,
              0.0, 0.0, 0.3,
	          1.0, 0.0, 0.5)

    setLightDirection(0.0, 0.2, 0.8)

    setTerrain(terrain, "./terrains/level4.txt")

    player = getPlayer(terrain)
    setScript(player, "./scripts/player1.lua")
    setPosition(player, x_start_player, 15.0, 7.5)

    camera = getCamera(obj)
    setScript(camera, "./scripts/camera1.lua")
    setPosition(camera, x_start_camera, 15.0, 27.5)

    playBgMusic("./music/Sparkman.ogg", 1)
    
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
