x_start_camera = 110.0
x_start_player = x_start_camera - 20.0

soundPlayed = 0

function on_load(terrain)
    --setBgMusic("./music/Wily.ogg")
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

    return
end

function on_update(terrain, elapsedTime)
    player = getPlayer()

    player_position = getPosition(player)

    if player_position[1] > 250.0 and soundPlayed == 0 then
        playSound(terrain, "bossexplo.wav")

        obj = addObject(terrain, "./scripts/boss2.lua")
        setPosition(obj, player_position[1] + 30.0, player_position[2], player_position[3])
        soundPlayed = 1
    end 

    return
end


function on_unload()
    return
end
