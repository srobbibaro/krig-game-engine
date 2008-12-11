x_start_camera = 50.0
x_start_player = x_start_camera - 10.0
x_start_boss   = x_start_camera + 10.0

function on_load(terrain)
    --setBgMusic("./music/Woodman.ogg")
    setSkyBox(0.0, 0.0, 0.3,
              0.6, 0.6, 0.6,
              1.0, 1.0, 1.0)

    setLightDirection(0.25, 0.25, 0.5)

    setTerrain(terrain, "./terrains/level2.txt")

    obj = addObject(terrain, "./scripts/boss2.lua")
    setPosition(obj, x_start_boss, 15.0, 7.5)

    player = getPlayer(obj)
    setScript(player, "./scripts/player1.lua")
    setPosition(player, x_start_player, 20.0, 7.5)

    camera = getCamera(obj)
    setScript(camera, "./scripts/camera1.lua")
    setPosition(camera, x_start_camera, 20.0, 35.0)

    addParticleSystem(camera, 2)
    
    return
end

function on_update()
    return
end

function on_unload()
    return
end