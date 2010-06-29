dofile("./levels/level_lib.lua")

x_start_camera = 110.0 -- 110.0
x_start_player = x_start_camera - 20.0

boss = nil
bossLife = 0

function on_load(terrain)
    setSkyBox(0.0, 0.0, 0.3,
	          0.4, 0.4, 1.0,
	          0.8, 0.8, 1.0)

    setLightDirection(0.7723, 0.1792, 0.6094)

    setTerrain(terrain, "./terrains/level1.txt")

    player = getPlayer()
    setScript(player, "./scripts/player1.lua")
    setPosition(player, x_start_player, 20.0, 7.5)

    camera = getCamera()
    setScript(camera, "./scripts/camera1.lua")
    setPosition(camera, x_start_camera, 15.0, 35.0)

    -- setup the enemy ships
    local obj = addObject(terrain, "./scripts/enemy_ship1.lua")
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

    --  sweeping enemy ships 
    setSweepingFromAboveEnemyShips(terrain, 610, 635, 5, 22.0, 7.5)
    setSweepingFromBelowEnemyShips(terrain, 675, 700, 5, 8.0, 7.5)
	
    -- 2nd wave of asteroids --
    setAsteroidWave(terrain, 730.0, 810.0)
	
    -- last wave - crashing ships --
	obj6 = addObject(terrain, "./scripts/enemy_ship4.lua")
	setPosition(obj6, 882, 15, 37.5)
		
	obj5 = addObject(terrain, "./scripts/enemy_ship5.lua")
	setPosition(obj5, 880, 10, 37.5)
		
	obj4 = addObject(terrain, "./scripts/enemy_ship4.lua")
	setPosition(obj4, 890, 15, 37.5)
	
	obj3 = addObject(terrain, "./scripts/enemy_ship6.lua")
	setPosition(obj3, 900, 15, 37.5)
	
	obj2 = addObject(terrain, "./scripts/enemy_ship6.lua")
	setPosition(obj2, 905, 10, 37.5)
		
	obj1 = addObject(terrain, "./scripts/enemy_ship6.lua")
	setPosition(obj1, 910, 12, 37.5)
	
    -- scenery --
	obj = addObject(terrain, "./scripts/building.lua")
	setPosition(obj, 160, 20, -75.0)
	setScale(obj, 10.0, 20.0, 10.0)

	obj = addObject(terrain, "./scripts/building.lua")
	setPosition(obj, 190, 20, -100.0)
	setScale(obj, 10.0, 30.0, 10.0)

	obj = addObject(terrain, "./scripts/building.lua")
	setPosition(obj, 150, 7, -50.0)
	setScale(obj, 7.0, 10.0, 7.0)

	obj = addObject(terrain, "./scripts/building.lua")
	setPosition(obj, 175, 7, -50.0)
	setScale(obj, 7.0, 7.0, 7.0)

	obj = addObject(terrain, "./scripts/building.lua")
	setPosition(obj, 200, 9, -50.0)
	setScale(obj, 6.0, 9.0, 6.0)

	obj = addObject(terrain, "./scripts/building.lua")
	setPosition(obj, 210, 10, -50.0)
	setScale(obj, 6.0, 10.0, 6.0)

	obj = addObject(terrain, "./scripts/building.lua")
	setPosition(obj, 230, 8, -50.0)
	setScale(obj, 6.0, 8.0, 6.0)

    -- place pine trees
	obj = addObject(terrain, "./scripts/pinetree.lua", 145.0, -25.0)
	obj = addObject(terrain, "./scripts/pinetree.lua", 150.0, -25.0)
	obj = addObject(terrain, "./scripts/pinetree.lua", 185.0, -20.0)
	obj = addObject(terrain, "./scripts/pinetree.lua", 190.0, -15.0)
	obj = addObject(terrain, "./scripts/pinetree.lua", 210.0, -15.0)
	obj = addObject(terrain, "./scripts/pinetree.lua", 213.0, -12.0)
	obj = addObject(terrain, "./scripts/pinetree.lua", 216.0, -13.0)
	obj = addObject(terrain, "./scripts/pinetree.lua", 255.0, -15.0)
	obj = addObject(terrain, "./scripts/pinetree.lua", 265.0, -20.0)
	obj = addObject(terrain, "./scripts/pinetree.lua", 280.0, -20.0)
	obj = addObject(terrain, "./scripts/pinetree.lua", 285.0, -15.0)
	obj = addObject(terrain, "./scripts/pinetree.lua", 300.0, -15.0)
	obj = addObject(terrain, "./scripts/pinetree.lua", 310.0, -25.0)
	obj = addObject(terrain, "./scripts/pinetree.lua", 313.0, -28.0)
	obj = addObject(terrain, "./scripts/pinetree.lua", 315.0, -24.0)
	obj = addObject(terrain, "./scripts/pinetree.lua", 355.0, -10.0)
	obj = addObject(terrain, "./scripts/pinetree.lua", 365.0, -15.0)
	obj = addObject(terrain, "./scripts/pinetree.lua", 370.0, -20.0)
	obj = addObject(terrain, "./scripts/pinetree.lua", 370.0, -65.0, 4.0)
	obj = addObject(terrain, "./scripts/pinetree.lua", 375.0, -60.0, 4.0)
	obj = addObject(terrain, "./scripts/pinetree.lua", 380.0, -55.0, 4.0)
	obj = addObject(terrain, "./scripts/pinetree.lua", 705.0, -45.0, 4.0)
	obj = addObject(terrain, "./scripts/pinetree.lua", 708.0, -42.0, 4.0)
	obj = addObject(terrain, "./scripts/pinetree.lua", 715.0, -45.0, 4.0)
	obj = addObject(terrain, "./scripts/pinetree.lua", 725.0, -40.0, 4.0)
	obj = addObject(terrain, "./scripts/pinetree.lua", 735.0, -45.0, 4.0)
	obj = addObject(terrain, "./scripts/pinetree.lua", 740.0, -42.0, 4.0)
	obj = addObject(terrain, "./scripts/pinetree.lua", 980.0, -75.0, 4.0)
	obj = addObject(terrain, "./scripts/pinetree.lua", 985.0, -70.0, 4.0)
	obj = addObject(terrain, "./scripts/pinetree.lua", 990.0, -80.0, 4.0)
    obj = addObject(terrain, "./scripts/pinetree.lua", 995.0, -75.0, 4.0)

--    playBgMusic("./music/Wily.ogg", 1)

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
            boss = addObject(terrain, "./scripts/boss1.lua")
            setPosition(boss, 995.0, 15.0, 7.5)
        end
    elseif bossBattle == 1 then
        bossLife = 0
        if boss ~= nil then bossLife = getScriptValue(boss, "life") end
        if bossLife == 0 then boss = nil end
        update_level(elapsedTime, bossLife)
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
    if bossBattle == 1 then
        bossLife = 0
        if boss ~= nil then bossLife = getScriptValue(boss, "life") end
        if bossLife == 0 then boss = nil end
    end

    display_hud(bossBattle, bossLife)
    display_debug()
    return
end

