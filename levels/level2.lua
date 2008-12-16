x_start_camera = 50.0
x_start_player = x_start_camera - 10.0
x_start_boss   = x_start_camera + 10.0

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
    setPosition(camera, x_start_camera, 20.0, 35.0)

    addParticleSystem(camera, 2)

    obj = addObject(terrain, "./scripts/boss2.lua")
    setPosition(obj, x_start_boss, 15.0, 7.5)

    playBgMusic("./music/Woodman.ogg", 1)
    
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

function display_hud()
    player = getPlayer()

    ShipEnergy = getScriptValue(player, "life")
    EnemyEnergy = 30
    EnergyBar = -0.99 + (0.068 * ShipEnergy)
    BossEnergy = 0.99 - (0.017 * EnemyEnergy)

    plr_pos = getPosition(player)
    
    gl.PushMatrix()
        gl.Translate (0.0, 0.0, -2.0)
        gl.Color (1.0, 1.0, 1.0)
        displayText("Lives: " .. getScriptValue(player, "lives"), -1.0, 0.7)

        if plr_pos[1] > 300.0 then
            displayText("Enemy", 0.85, 0.7)
        end 

        displayText("Score: 1000", -.2, 0.7)
        displayText("Missiles: 30", -1.0, .75)
        
        gl.Begin("QUADS")
            gl.Color (1.0, 0.0, 0.0)
            gl.Vertex (-1.0, 0.68, 0.0)
            gl.Vertex (-1.0, 0.63, 0.0)
            gl.Vertex (-0.3, 0.63, 0.0)
            gl.Vertex (-0.3, 0.68, 0.0)
            
            gl.Color (0.0, 0.0, 0.0)
            gl.Vertex (-0.99, 0.67, 0.001)
            gl.Vertex (-0.99, 0.64, 0.001)
            gl.Vertex (-0.31, 0.64, 0.001)
            gl.Vertex (-0.31, 0.67, 0.001)
            
            gl.Color (0.0, 1.0, 1.0)
            gl.Vertex (-0.99, 0.67, 0.002)
            gl.Vertex (-0.99, 0.64, 0.002)
            gl.Vertex (EnergyBar, 0.64, 0.002)
            gl.Vertex (EnergyBar, 0.67, 0.002)
            
            if plr_pos[1] > 300.0 then
                gl.Color (1.0, 0.0, 0.0)
                gl.Vertex (0.3, 0.68, 0.0)
                gl.Vertex (0.3, 0.63, 0.0)
                gl.Vertex (1.0, 0.63, 0.0)
                gl.Vertex (1.0, 0.68, 0.0)
            
                gl.Color (0.0, 0.0, 0.0)
                gl.Vertex (0.31, 0.67, 0.001)
                gl.Vertex (0.31, 0.64, 0.001)
                gl.Vertex (0.99, 0.64, 0.001)
                gl.Vertex (0.99, 0.67, 0.001)
            
                gl.Color (1.0, 1.0, 0.0)
                gl.Vertex (BossEnergy, 0.67, 0.002)
                gl.Vertex (BossEnergy, 0.64, 0.002)
                gl.Vertex (0.99, 0.64, 0.002)
                gl.Vertex (0.99, 0.67, 0.002)
            end
        gl.End()
        
    gl.PopMatrix()

    return
end


function on_draw_screen()
    display_hud()
    display_debug()
    return
end


function display_debug()
    player = getPlayer()
    camera = getCamera()
    plr_pos = getPosition(player)
    cam_pos = getPosition(camera)
    cam_dir = getDirection(camera)
    cam_up = getUp(camera)
    cam_rot = getRotation(camera)
    light_dir = getLightDirection()

    gl.PushMatrix()
        gl.Translate (0.0, 0.0, -2.0)
        gl.Color (1.0, 1.0, 1.0)

        displayText(string.format("plr pos: %.04f %.04f %.04f", plr_pos[1], plr_pos[2], plr_pos[3]), -1.0, 0.55)
        displayText(string.format("cam pos: %.04f %.04f %.04f", cam_pos[1], cam_pos[2], cam_pos[3]), -1.0, 0.48)
        displayText(string.format("cam dir: %.04f %.04f %.04f", cam_dir[1], cam_dir[2], cam_dir[3]), -1.0, 0.40)
        displayText(string.format("cam up: %.04f %.04f %.04f", cam_up[1], cam_up[2], cam_up[3]), -1.0, 0.33)
        displayText(string.format("cam rot: %.04f %.04f %.04f", cam_rot[1], cam_rot[2], cam_rot[3]), -1.0, 0.26)
        displayText(string.format("fps: %.04f", getFps()), -1.0, 0.18)
        displayText(string.format("cam id: %d", getCameraId()), -1.0, 0.10)
        
        displayText(string.format("light dir: %.04f %.04f %.04f", light_dir[1], light_dir[2], light_dir[3]), -1.0, 0.0)
        gl.End()
        
    gl.PopMatrix()

    return
end