life = 1

score = 0

function handle_collision(this, temp)
    typeId = getTypeId(temp)

    if typeId == 2 or typeId == 4 then
        playSound(this, "explosion1.wav")
        life = life - 1
        if life <= 0 then
            player = getPlayer()
            player_score = getScriptValue(player, "score") + score
            setScriptValue(player, "score", player_score)
            create_score_text(this)
            
            math.randomseed( os.time() )
            powerup_rand = math.random(100) 

            --print ("powerup_rand:".. powerup_rand)

            if powerup_rand < 10 then
                create_powerup(this, 1)
            elseif powerup_rand  >= 10 and powerup_rand < 20 then
                create_powerup(this, 2)
            elseif powerup_rand  >= 20 and powerup_rand < 30 then
                create_powerup(this, 3)
            else
                create_explosion(this)
            end

            removeObject(this)
        end
    end
    return
end

function create_powerup(this, powerup_num)
    local obj = nil

    if powerup_num == 1 then
        obj = addObject(this, "./scripts/powerup1.lua")
    elseif powerup_num == 2 then
        obj = addObject(this, "./scripts/powerup2.lua")
    elseif powerup_num == 3 then
        obj = addObject(this, "./scripts/powerup3.lua")
    end
   
    if obj ~= nil then 
        this_position = getPosition(this)
        setPosition(obj, this_position[1], this_position[2], this_position[3])
    end
end

function create_explosion(this)
    this_position = getPosition(this)

    obj = addObject(this, "./scripts/explosion.lua")
    setPosition(obj, this_position[1], this_position[2], this_position[3])
end

function create_score_text(this)
    camera = getCamera()
    cam_vel = getVelocity(camera)
    this_position = getPosition(this)

    obj = addText(this, "./scripts/camera1.lua", score)

    setPositionv(obj, this_position)
    setScale(obj, 0.25, 0.25, 0.0)
    setVelocity(obj, cam_vel[1], 2.0, cam_vel[3])
    setFadeRate(obj, -.25)
    setColor(obj, 1.0, 1.0, 1.0)
end
