x_start_camera = 110.0
x_start_player = x_start_camera - 20.0

soundPlayed = 0

totalTime = 0.0
songPlayed = 0

function on_load(terrain)
    setSkyBox(0.0, 0.0, 0.3,
	        0.4, 0.4, 1.0,
	        0.8, 0.8, 1.0)

    setLightDirection(0.0, 0.15, 0.85)

    setTerrain(terrain, "./terrains/level1.txt")

    obj = addObject(terrain, "./scripts/boss1.lua")
    setPosition(obj, 981.0, 15.0, 7.5)

    player = getPlayer(obj)
    setScript(player, "./scripts/player1.lua")
    setPosition(player, x_start_player, 20.0, 7.5)

    camera = getCamera(obj)
    setScript(camera, "./scripts/camera1.lua")
    setPosition(camera, x_start_camera, 15.0, 35.0)
    
    -- setup the enemy ships
    obj = addObject(terrain, "./scripts/enemy_ship1.lua")
    setPosition(obj, 160, 15, 7.5)

    obj = addObject(terrain, "./scripts/enemy_ship1.lua")
    setPosition(obj, 180, 20, 7.5)

    obj = addObject(terrain, "./scripts/enemy_ship1.lua")
    setPosition(obj, 200, 10, 7.5)

    obj = addObject(terrain, "./scripts/enemy_ship1.lua")
    setPosition(obj, 220, 15, 7.5)

    obj = addObject(terrain, "./scripts/enemy_ship1.lua")
    setPosition(obj, 240, 20, 7.5)

    obj = addObject(terrain, "./scripts/enemy_ship1.lua")
    setPosition(obj, 260, 15, 7.5)

    obj = addObject(terrain, "./scripts/enemy_ship1.lua")
    setPosition(obj, 270, 18, 7.5)
	
    obj = addObject(terrain, "./scripts/enemy_ship1.lua")
    setPosition(obj, 270, 12, 7.5)

    obj = addObject(terrain, "./scripts/enemy_ship1.lua")
    setPosition(obj, 280, 15, 7.5)

    obj = addObject(terrain, "./scripts/enemy_ship1.lua")
    setPosition(obj, 300, 10, 7.5)
	
    obj = addObject(terrain, "./scripts/enemy_ship1.lua")
    setPosition(obj, 310, 7, 7.5)

    obj = addObject(terrain, "./scripts/enemy_ship1.lua")	
    setPosition(obj, 310, 13, 7.5)

    obj = addObject(terrain, "./scripts/enemy_ship1.lua")
    setPosition(obj, 320, 10, 7.5)
	
    obj = addObject(terrain, "./scripts/enemy_ship1.lua")
    setPosition(obj, 340, 20, 7.5)
	
    obj = addObject(terrain, "./scripts/enemy_ship1.lua")
    setPosition(obj, 350, 23, 7.5)

    obj = addObject(terrain, "./scripts/enemy_ship1.lua")	
    setPosition(obj, 350, 17, 7.5)

    obj = addObject(terrain, "./scripts/enemy_ship1.lua")
    setPosition(obj, 360, 20, 7.5)
	
    obj = addObject(terrain, "./scripts/enemy_ship1.lua")
    setPosition(obj, 390, 15, 7.5)

    obj = addObject(terrain, "./scripts/enemy_ship1.lua")
    setPosition(obj, 390, 22, 7.5)

    obj = addObject(terrain, "./scripts/enemy_ship1.lua")
    setPosition(obj, 390, 8, 7.5)
	      
    --  setup the asteroids --   
    for i = 410, 560, 15 do
        obj = addObject(terrain, "./scripts/asteroid1.lua")
	  setPosition(obj, i, 30, 7.5)
    end

    for i = 415, 565, 15 do
        obj = addObject(terrain, "./scripts/asteroid3.lua")
	  setPosition(obj, i, 30, 7.5)
    end

    for i = 420, 570, 15 do
        obj = addObject(terrain, "./scripts/asteroid2.lua")
	  setPosition(obj, i, 30, 7.5)
    end

      --  sweeping enemy ships - sweeping from above --
	obj = addObject(terrain, "./scripts/enemy_ship2.lua")
	setPosition(obj, 610, 22, 7.5)

	obj = addObject(terrain, "./scripts/enemy_ship2.lua")
	setPosition(obj, 615, 22, 7.5)
	
	obj = addObject(terrain, "./scripts/enemy_ship2.lua")
	setPosition(obj, 620, 22, 7.5)
	
	obj = addObject(terrain, "./scripts/enemy_ship2.lua")
	setPosition(obj, 625, 22, 7.5)

	obj = addObject(terrain, "./scripts/enemy_ship2.lua")
	setPosition(obj, 630, 22, 7.5)

	obj = addObject(terrain, "./scripts/enemy_ship2.lua")
	setPosition(obj, 635, 22, 7.5)
	      
	-- sweeping from below --
	obj = addObject(terrain, "./scripts/enemy_ship3.lua")
	setPosition(obj, 675, 8, 7.5)
	
	obj = addObject(terrain, "./scripts/enemy_ship3.lua")
	setPosition(obj, 680, 8, 7.5)
	
	obj = addObject(terrain, "./scripts/enemy_ship3.lua")
	setPosition(obj, 685, 8, 7.5)
	
	obj = addObject(terrain, "./scripts/enemy_ship3.lua")
	setPosition(obj, 690, 8, 7.5)
	
	obj = addObject(terrain, "./scripts/enemy_ship3.lua")
	setPosition(obj, 695, 8, 7.5)
	
	obj = addObject(terrain, "./scripts/enemy_ship3.lua")
	setPosition(obj, 700, 8, 7.5)
	
    -- 2nd wave of asteroids --
    for i = 730, 810, 15 do
        obj = addObject(terrain, "./scripts/asteroid1.lua"),
	  setPosition(obj, i, 30, 7.5)
    end

    for i = 735, 815, 15 do
        obj = addObject(terrain, "./scripts/asteroid3.lua"),
	  setPosition(obj, i, 30, 7.5)
    end

    for i = 740, 820, 15 do
        obj = addObject(terrain, "./scripts/asteroid2.lua"),
	  setPosition(obj, i, 30, 7.5)
    end
	
      -- last wave - crashing ships --
	obj = addObject(terrain, "./scripts/enemy_ship4.lua")
	setPosition(obj, 882, 20, 37.5)
		
	obj = addObject(terrain, "./scripts/enemy_ship5.lua")
	setPosition(obj, 880, 15, 37.5)
		
	obj = addObject(terrain, "./scripts/enemy_ship4.lua")
	setPosition(obj, 890, 20, 37.5)
	
	obj = addObject(terrain, "./scripts/enemy_ship6.lua")
	setPosition(obj, 900, 20, 37.5)
	
	obj = addObject(terrain, "./scripts/enemy_ship6.lua")
	setPosition(obj, 905, 15, 37.5)
		
	obj = addObject(terrain, "./scripts/enemy_ship6.lua")
	setPosition(obj, 910, 20, 37.5)
	
      -- scenery --

	obj = addObject(terrain, "./scripts/building.lua"),
	setPosition(obj, 160, 20, -100.0)
	setRotation(obj, 0.0, 0.0, 0.0)
	setScale(obj, 10.0, 20.0, 10.0)

	obj = addObject(terrain, "./scripts/building.lua"),
	setPosition(obj, 190, 20, -100.0)
	setRotation(obj, 0.0, 0.0, 0.0)
	setScale(obj, 10.0, 30.0, 10.0)

	obj = addObject(terrain, "./scripts/building.lua"),
	setPosition(obj, 150, 7, -50.0)
	setRotation(obj, 0.0, 0.0, 0.0)
	setScale(obj, 7.0, 10.0, 7.0)

	obj = addObject(terrain, "./scripts/building.lua"),
	setPosition(obj, 175, 7, -50.0)
	setRotation(obj, 0.0, 0.0, 0.0)
	setScale(obj, 7.0, 7.0, 7.0)

	obj = addObject(terrain, "./scripts/building.lua"),
	setPosition(obj, 200, 9, -50.0)
	setRotation(obj, 0.0, 0.0, 0.0)
	setScale(obj, 6.0, 9.0, 6.0)

	obj = addObject(terrain, "./scripts/building.lua"),
	setPosition(obj, 210, 10, -50.0)
	setRotation(obj, 0.0, 0.0, 0.0)
	setScale(obj, 6.0, 10.0, 6.0)

	obj = addObject(terrain, "./scripts/building.lua"),
	setPosition(obj, 230, 8, -50.0)
	setRotation(obj, 0.0, 0.0, 0.0)
	setScale(obj, 6.0, 8.0, 6.0)

	obj = addObject(terrain, "./scripts/pinetree.lua"),
	setPosition(obj, 145, 5, -25.0)
	setRotation(obj, 0.0, 0.0, 0.0)
	setScale(obj, 2.0, 2.0, 2.0)

	obj = addObject(terrain, "./scripts/pinetree.lua"),
	setPosition(obj, 150, 5, -25.0)
	setRotation(obj, 0.0, 0.0, 0.0)
	setScale(obj, 2.0, 2.0, 2.0)

	obj = addObject(terrain, "./scripts/pinetree.lua"),
	setPosition(obj, 185, 4, -20.0)
	setRotation(obj, 0.0, 0.0, 0.0)
	setScale(obj, 2.0, 2.0, 2.0)

	obj = addObject(terrain, "./scripts/pinetree.lua"),
	setPosition(obj, 190, 3, -15.0)
	setRotation(obj, 0.0, 0.0, 0.0)
	setScale(obj, 2.0, 2.0, 2.0)

	obj = addObject(terrain, "./scripts/pinetree.lua"),
	setPosition(obj, 210, 3, -15.0)
	setRotation(obj, 0.0, 0.0, 0.0)
	setScale(obj, 2.0, 2.0, 2.0)

	obj = addObject(terrain, "./scripts/pinetree.lua"),
	setPosition(obj, 213, 2.5, -12.0)
	setRotation(obj, 0.0, 0.0, 0.0)
	setScale(obj, 2.0, 2.0, 2.0)

	obj = addObject(terrain, "./scripts/pinetree.lua"),
	setPosition(obj, 216, 3, -13.0)
	setRotation(obj, 0.0, 0.0, 0.0)
	setScale(obj, 2.0, 2.0, 2.0)

	obj = addObject(terrain, "./scripts/pinetree.lua"),
	setPosition(obj, 255, 3.0, -15.0)
	setRotation(obj, 0.0, 0.0, 0.0)
	setScale(obj, 2.0, 2.0, 2.0)

	obj = addObject(terrain, "./scripts/pinetree.lua"),
	setPosition(obj, 265, 4.5, -20.0)
	setRotation(obj, 0.0, 0.0, 0.0)
	setScale(obj, 2.0, 2.0, 2.0)

	obj = addObject(terrain, "./scripts/pinetree.lua"),
	setPosition(obj, 280, 4.5, -20.0)
	setRotation(obj, 0.0, 0.0, 0.0)
	setScale(obj, 2.0, 2.0, 2.0)

	obj = addObject(terrain, "./scripts/pinetree.lua"),
	setPosition(obj, 285, 3.0, -15.0)
	setRotation(obj, 0.0, 0.0, 0.0)
	setScale(obj, 2.0, 2.0, 2.0)

	obj = addObject(terrain, "./scripts/pinetree.lua"),
	setPosition(obj, 300, 3.0, -15.0)
	setRotation(obj, 0.0, 0.0, 0.0)
	setScale(obj, 2.0, 2.0, 2.0)

	obj = addObject(terrain, "./scripts/pinetree.lua"),
	setPosition(obj, 310, 5.0, -25.0)
	setRotation(obj, 0.0, 0.0, 0.0)
	setScale(obj, 2.0, 2.0, 2.0)

	obj = addObject(terrain, "./scripts/pinetree.lua"),
	setPosition(obj, 313, 6.5, -28.0)
	setRotation(obj, 0.0, 0.0, 0.0)
	setScale(obj, 2.0, 2.0, 2.0)

	obj = addObject(terrain, "./scripts/pinetree.lua"),
	setPosition(obj, 315, 4.5, -24.0)
	setRotation(obj, 0.0, 0.0, 0.0)
	setScale(obj, 2.0, 2.0, 2.0)

	obj = addObject(terrain, "./scripts/pinetree.lua"),
	setPosition(obj, 355, 2.0, -10.0)
	setRotation(obj, 0.0, 0.0, 0.0)
	setScale(obj, 2.0, 2.0, 2.0)

	obj = addObject(terrain, "./scripts/pinetree.lua"),
	setPosition(obj, 365, 3.0, -15.0)
	setRotation(obj, 0.0, 0.0, 0.0)
	setScale(obj, 2.0, 2.0, 2.0)

	obj = addObject(terrain, "./scripts/pinetree.lua"),
	setPosition(obj, 370, 2.0, -10.0)
	setRotation(obj, 0.0, 0.0, 0.0)
	setScale(obj, 2.0, 2.0, 2.0)

	obj = addObject(terrain, "./scripts/pinetree.lua"),
	setPosition(obj, 370, 6.0, -65.0)
	setRotation(obj, 0.0, 0.0, 0.0)
	setScale(obj, 4.0, 4.0, 4.0)

	obj = addObject(terrain, "./scripts/pinetree.lua"),
	setPosition(obj, 375, 6.0, -60.0)
	setRotation(obj, 0.0, 0.0, 0.0)
	setScale(obj, 4.0, 4.0, 4.0)

	obj = addObject(terrain, "./scripts/pinetree.lua"),
	setPosition(obj, 380, 6.0, -55.0)
	setRotation(obj, 0.0, 0.0, 0.0)
	setScale(obj, 4.0, 4.0, 4.0)

	obj = addObject(terrain, "./scripts/pinetree.lua"),
	setPosition(obj, 705, 6.0, -45.0)
	setRotation(obj, 0.0, 0.0, 0.0)
	setScale(obj, 4.0, 4.0, 4.0)

	obj = addObject(terrain, "./scripts/pinetree.lua"),
	setPosition(obj, 708, 6.0, -42.0)
	setRotation(obj, 0.0, 0.0, 0.0)
	setScale(obj, 4.0, 4.0, 4.0)

	obj = addObject(terrain, "./scripts/pinetree.lua"),
	setPosition(obj, 715, 6.0, -45.0)
	setRotation(obj, 0.0, 0.0, 0.0)
	setScale(obj, 4.0, 4.0, 4.0)

	obj = addObject(terrain, "./scripts/pinetree.lua"),
	setPosition(obj, 725, 6.0, -40.0)
	setRotation(obj, 0.0, 0.0, 0.0)
	setScale(obj, 4.0, 4.0, 4.0)

	obj = addObject(terrain, "./scripts/pinetree.lua"),
	setPosition(obj, 735, 6.0, -45.0)
	setRotation(obj, 0.0, 0.0, 0.0)
	setScale(obj, 4.0, 4.0, 4.0)

	obj = addObject(terrain, "./scripts/pinetree.lua"),
	setPosition(obj, 740, 6.0, -42.0)
	setRotation(obj, 0.0, 0.0, 0.0)
	setScale(obj, 4.0, 4.0, 4.0)

      -- ------------------------------------

	obj = addObject(terrain, "./scripts/pinetree.lua"),
	setPosition(obj, 980, 9.0, -75.0)
	setRotation(obj, 0.0, 0.0, 0.0)
	setScale(obj, 4.0, 4.0, 4.0)

	obj = addObject(terrain, "./scripts/pinetree.lua"),
	setPosition(obj, 985, 6.5, -70.0)
	setRotation(obj, 0.0, 0.0, 0.0)
	setScale(obj, 4.0, 4.0, 4.0)

	obj = addObject(terrain, "./scripts/pinetree.lua"),
	setPosition(obj, 990, 10.0, -80.0)
	setRotation(obj, 0.0, 0.0, 0.0)
	setScale(obj, 4.0, 4.0, 4.0)

	obj = addObject(terrain, "./scripts/pinetree.lua"),
	setPosition(obj, 995, 5.5, -75.0)
	setRotation(obj, 0.0, 0.0, 0.0)
	setScale(obj, 4.0, 4.0, 4.0)

      playBgMusic("./music/Sparkman.ogg", 1)

    return
end

function on_update(terrain, elapsedTime)
    totalTime = totalTime + elapsedTime

    if songPlayed == 0 and totalTime > 15.0 then
        playBgMusic("./music/Wily.ogg", 1)
        songPlayed = 1
        totalTime = 0.0 
    end

    player = getPlayer()

    player_position = getPosition(player)

    if player_position[1] > 200.0 and soundPlayed == 0 then
        playSound(terrain, "bossexplo.wav")

        obj = addObject(terrain, "./scripts/boss2.lua")
        setPosition(obj, player_position[1] + 30.0, player_position[2], player_position[3])
        soundPlayed = 1
        setLightDirection(0.0, 1.0, 0.0)
    end 

    if songPlayed == 1 and totalTime <= 10.0 then
        setLightDirection(0.0, 1.0, totalTime)
    end


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
