x_start_camera = 50.0
x_start_player = x_start_camera - 20.0

function on_load(terrain)
    --playBgMusic("./music/Sparkman.ogg", 1)
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
    
    return
end

function on_update()
    return
end

function on_unload()
    return
end
