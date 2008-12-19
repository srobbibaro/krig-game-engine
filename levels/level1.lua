dofile("./levels/level_lib.lua")

x_start_camera = 110.0 -- 110.0
x_start_player = x_start_camera - 20.0

bossBattle = 0

function setAsteroidWave(terrain, x_start, x_end)
    for i = x_start, x_end, 15 do
        obj = addObject(terrain, "./scripts/asteroid1.lua")
	    setPosition(obj, i, 30.0, 7.5)

        obj = addObject(terrain, "./scripts/asteroid3.lua")
	    setPosition(obj, (i + 5.0), 30.0, 7.5)

        obj = addObject(terrain, "./scripts/asteroid2.lua")
	    setPosition(obj, (i + 10.0), 30.0, 7.5)
    end

    return
end

function on_load(terrain)
    setSkyBox(0.0, 0.0, 0.3,
	          0.4, 0.4, 1.0,
	          0.8, 0.8, 1.0)

    setLightDirection(0.0, 0.15, 0.85)

    setTerrain(terrain, "./terrains/level1.txt")

    player = getPlayer()
    setScript(player, "./scripts/player1.lua")
    setPosition(player, x_start_player, 20.0, 7.5)

    camera = getCamera()
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
    setAsteroidWave(terrain, 410.0, 560.0)

    --  sweeping enemy ships - sweeping from above --
    for i = 610, 635, 5 do
	    obj = addObject(terrain, "./scripts/enemy_ship2.lua")
	    setPosition(obj, i, 22.0, 7.5)
    end
	      
	-- sweeping from below --
    for i = 675, 700, 5 do
	    obj = addObject(terrain, "./scripts/enemy_ship3.lua")
	    setPosition(obj, i, 8.0, 7.5)
    end
	
    -- 2nd wave of asteroids --
    setAsteroidWave(terrain, 730.0, 810.0)
	
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
	setScale(obj, 10.0, 20.0, 10.0)

	obj = addObject(terrain, "./scripts/building.lua"),
	setPosition(obj, 190, 20, -100.0)
	setScale(obj, 10.0, 30.0, 10.0)

	obj = addObject(terrain, "./scripts/building.lua"),
	setPosition(obj, 150, 7, -50.0)
	setScale(obj, 7.0, 10.0, 7.0)

	obj = addObject(terrain, "./scripts/building.lua"),
	setPosition(obj, 175, 7, -50.0)
	setScale(obj, 7.0, 7.0, 7.0)

	obj = addObject(terrain, "./scripts/building.lua"),
	setPosition(obj, 200, 9, -50.0)
	setScale(obj, 6.0, 9.0, 6.0)

	obj = addObject(terrain, "./scripts/building.lua"),
	setPosition(obj, 210, 10, -50.0)
	setScale(obj, 6.0, 10.0, 6.0)

	obj = addObject(terrain, "./scripts/building.lua"),
	setPosition(obj, 230, 8, -50.0)
	setScale(obj, 6.0, 8.0, 6.0)

	obj = addObject(terrain, "./scripts/pinetree.lua"),
	setPosition(obj, 145, 5, -25.0)
	setScale(obj, 2.0, 2.0, 2.0)

	obj = addObject(terrain, "./scripts/pinetree.lua"),
	setPosition(obj, 150, 5, -25.0)
	setScale(obj, 2.0, 2.0, 2.0)

	obj = addObject(terrain, "./scripts/pinetree.lua"),
	setPosition(obj, 185, 4, -20.0)
	setScale(obj, 2.0, 2.0, 2.0)

	obj = addObject(terrain, "./scripts/pinetree.lua"),
	setPosition(obj, 190, 3, -15.0)
	setScale(obj, 2.0, 2.0, 2.0)

	obj = addObject(terrain, "./scripts/pinetree.lua"),
	setPosition(obj, 210, 3, -15.0)
	setScale(obj, 2.0, 2.0, 2.0)

	obj = addObject(terrain, "./scripts/pinetree.lua"),
	setPosition(obj, 213, 2.5, -12.0)
	setScale(obj, 2.0, 2.0, 2.0)

	obj = addObject(terrain, "./scripts/pinetree.lua"),
	setPosition(obj, 216, 3, -13.0)
	setScale(obj, 2.0, 2.0, 2.0)

	obj = addObject(terrain, "./scripts/pinetree.lua"),
	setPosition(obj, 255, 3.0, -15.0)
	setScale(obj, 2.0, 2.0, 2.0)

	obj = addObject(terrain, "./scripts/pinetree.lua"),
	setPosition(obj, 265, 4.5, -20.0)
	setScale(obj, 2.0, 2.0, 2.0)

	obj = addObject(terrain, "./scripts/pinetree.lua"),
	setPosition(obj, 280, 4.5, -20.0)
	setScale(obj, 2.0, 2.0, 2.0)

	obj = addObject(terrain, "./scripts/pinetree.lua"),
	setPosition(obj, 285, 3.0, -15.0)
	setScale(obj, 2.0, 2.0, 2.0)

	obj = addObject(terrain, "./scripts/pinetree.lua"),
	setPosition(obj, 300, 3.0, -15.0)
	setScale(obj, 2.0, 2.0, 2.0)

	obj = addObject(terrain, "./scripts/pinetree.lua"),
	setPosition(obj, 310, 5.0, -25.0)
	setScale(obj, 2.0, 2.0, 2.0)

	obj = addObject(terrain, "./scripts/pinetree.lua"),
	setPosition(obj, 313, 6.5, -28.0)
	setScale(obj, 2.0, 2.0, 2.0)

	obj = addObject(terrain, "./scripts/pinetree.lua"),
	setPosition(obj, 315, 4.5, -24.0)
	setScale(obj, 2.0, 2.0, 2.0)

	obj = addObject(terrain, "./scripts/pinetree.lua"),
	setPosition(obj, 355, 2.0, -10.0)
	setScale(obj, 2.0, 2.0, 2.0)

	obj = addObject(terrain, "./scripts/pinetree.lua"),
	setPosition(obj, 365, 3.0, -15.0)
	setScale(obj, 2.0, 2.0, 2.0)

	obj = addObject(terrain, "./scripts/pinetree.lua"),
	setPosition(obj, 370, 2.0, -10.0)
	setScale(obj, 2.0, 2.0, 2.0)

	obj = addObject(terrain, "./scripts/pinetree.lua"),
	setPosition(obj, 370, 6.0, -65.0)
	setScale(obj, 4.0, 4.0, 4.0)

	obj = addObject(terrain, "./scripts/pinetree.lua"),
	setPosition(obj, 375, 6.0, -60.0)
	setScale(obj, 4.0, 4.0, 4.0)

	obj = addObject(terrain, "./scripts/pinetree.lua"),
	setPosition(obj, 380, 6.0, -55.0)
	setScale(obj, 4.0, 4.0, 4.0)

	obj = addObject(terrain, "./scripts/pinetree.lua"),
	setPosition(obj, 705, 6.0, -45.0)
	setScale(obj, 4.0, 4.0, 4.0)

	obj = addObject(terrain, "./scripts/pinetree.lua"),
	setPosition(obj, 708, 6.0, -42.0)
	setScale(obj, 4.0, 4.0, 4.0)

	obj = addObject(terrain, "./scripts/pinetree.lua"),
	setPosition(obj, 715, 6.0, -45.0)
	setScale(obj, 4.0, 4.0, 4.0)

	obj = addObject(terrain, "./scripts/pinetree.lua"),
	setPosition(obj, 725, 6.0, -40.0)
	setScale(obj, 4.0, 4.0, 4.0)

	obj = addObject(terrain, "./scripts/pinetree.lua"),
	setPosition(obj, 735, 6.0, -45.0)
	setScale(obj, 4.0, 4.0, 4.0)

	obj = addObject(terrain, "./scripts/pinetree.lua"),
	setPosition(obj, 740, 6.0, -42.0)
	setScale(obj, 4.0, 4.0, 4.0)

    ---------------------------------------
	obj = addObject(terrain, "./scripts/pinetree.lua"),
	setPosition(obj, 980, 9.0, -75.0)
	setScale(obj, 4.0, 4.0, 4.0)

	obj = addObject(terrain, "./scripts/pinetree.lua"),
	setPosition(obj, 985, 6.5, -70.0)
	setScale(obj, 4.0, 4.0, 4.0)

	obj = addObject(terrain, "./scripts/pinetree.lua"),
	setPosition(obj, 990, 10.0, -80.0)
	setScale(obj, 4.0, 4.0, 4.0)

	obj = addObject(terrain, "./scripts/pinetree.lua"),
	setPosition(obj, 995, 5.5, -75.0)
	setScale(obj, 4.0, 4.0, 4.0)

    playBgMusic("./music/Wily.ogg", 1)

    return
end

function on_update(terrain, elapsedTime)
    if bossBattle == 0  then
        camera = getCamera()
        cam_pos = getPosition(camera)

        if cam_pos[1] >= 970.0 then 
            bossBattle = 1

            -- Set the camera's velocity
            setVelocity(camera, 0.0, 0.0, 0.0)

            -- Set the player's velocity
            player = getPlayer()
            plr_vel = getVelocity(player)
            setVelocity(player, (plr_vel[1] - 10.0), plr_vel[2], plr_vel[3])
           
            -- Create the boss...
            obj = addObject(terrain, "./scripts/boss1.lua")
            setPosition(obj, 995.0, 15.0, 7.5)
        end
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
    display_hud(bossBattle)
    display_debug()
    return
end

