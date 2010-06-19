bossBattle = 0
bossDefeated = 0
endTimer = 10.0

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

        renderText(string.format("plr pos: %.04f %.04f %.04f", plr_pos[1], plr_pos[2], plr_pos[3]), -1.0, 0.55)
        renderText(string.format("cam pos: %.04f %.04f %.04f", cam_pos[1], cam_pos[2], cam_pos[3]), -1.0, 0.48)
        renderText(string.format("cam dir: %.04f %.04f %.04f", cam_dir[1], cam_dir[2], cam_dir[3]), -1.0, 0.40)
        renderText(string.format("cam up: %.04f %.04f %.04f", cam_up[1], cam_up[2], cam_up[3]), -1.0, 0.33)
        renderText(string.format("cam rot: %.04f %.04f %.04f", cam_rot[1], cam_rot[2], cam_rot[3]), -1.0, 0.26)
        renderText(string.format("fps: %.04f", getFps()), -1.0, 0.18)
        renderText(string.format("cam id: %d", getCameraId()), -1.0, 0.10)
        
        renderText(string.format("light dir: %.04f %.04f %.04f", light_dir[1], light_dir[2], light_dir[3]), -1.0, 0.0)
        gl.End()
        
    gl.PopMatrix()

    return
end

function display_hud(bossBattle, bossLife)
    player = getPlayer()

    ShipEnergy  = getScriptValue(player, "life")
    score       = getScriptValue(player, "score")
    numMissiles = getScriptValue(player, "numMissiles")
    EnemyEnergy = bossLife 
    EnergyBar = -0.31 - (0.068 * (10.0 - ShipEnergy))
    BossEnergy = 0.31 + (0.017 * (40.0 - EnemyEnergy))

    plr_pos = getPosition(player)
    
    gl.PushMatrix()
        gl.Translate (0.0, 0.0, -2.0)
        gl.Color (1.0, 1.0, 1.0)
        renderText("Lives: " .. getScriptValue(player, "lives"), -1.0, 0.7)

        renderText("Score: " .. score, -.2, 0.7)
        renderText("Missiles: " .. numMissiles, -1.0, .75)

        if bossBattle == 1 then
            renderText("Enemy", 0.85, 0.7)
        end
        
        gl.Begin("QUADS")
        --[[
            gl.Color (0.3, 0.0, 0.0)
            gl.Vertex (-1.0, 0.68, 0.0)
            gl.Vertex (-1.0, 0.63, 0.0)
            gl.Color (1.0, 0.0, 0.0)
            gl.Vertex (-0.3, 0.63, 0.0)
            gl.Vertex (-0.3, 0.68, 0.0)
        --]]
            
            gl.Color (0.0, 0.3, 0.3)
            gl.Vertex (-0.99, 0.67, 0.001)
            gl.Vertex (-0.99, 0.64, 0.001)
            gl.Color (0.0, 1.0, 1.0)
            gl.Vertex (-0.31, 0.64, 0.001)
            gl.Vertex (-0.31, 0.67, 0.001)
            
            gl.Color (0.0, 0.0, 0.0)
            gl.Vertex (-0.31, 0.64, 0.002)
            gl.Vertex (-0.31, 0.67, 0.002)
            gl.Vertex (EnergyBar, 0.67, 0.002)
            gl.Vertex (EnergyBar, 0.64, 0.002)
            
            if bossBattle == 1 then
            --[[
                gl.Color (1.0, 0.0, 0.0)
                gl.Vertex (0.3, 0.68, 0.0)
                gl.Vertex (0.3, 0.63, 0.0)
                gl.Vertex (1.0, 0.63, 0.0)
                gl.Vertex (1.0, 0.68, 0.0)
            --]]
            
                gl.Color (1.0, 1.0, 0.0)
                gl.Vertex (0.31, 0.67, 0.001)
                gl.Vertex (0.31, 0.64, 0.001)
                gl.Color (0.3, 0.3, 0.0)
                gl.Vertex (0.99, 0.64, 0.001)
                gl.Vertex (0.99, 0.67, 0.001)
            
                gl.Color (0.0, 0.0, 0.0)
                gl.Vertex (BossEnergy, 0.64, 0.002)
                gl.Vertex (BossEnergy, 0.67, 0.002)
                gl.Vertex (0.31, 0.67, 0.002)
                gl.Vertex (0.31, 0.64, 0.002)
            end
        gl.End()
        
    gl.PopMatrix()

    return
end

function update_level(timeElapsed, bossLife)
    if bossLife <= 0 then
        if bossDefeated == 0 then
            bossDefeated = 1
        elseif bossDefeated == 1 then
            endTimer = endTimer - timeElapsed
            if endTimer < 0.0 then
                setComplete(1)
            end
        end
    end
end
