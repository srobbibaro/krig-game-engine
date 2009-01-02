life = 1

function handle_collision(this, temp)
    typeId = getTypeId(temp)

    if typeId == 2 then
        playSound(this, "explosion1.wav")
        life = life - 1
        if life <= 0 then
            this_position = getPosition(this)

            obj = addObject(this, "./scripts/explosion.lua")
            setPosition(obj, this_position[1], this_position[2], this_position[3])
            
            camera = getCamera()
            cam_vel = getVelocity(camera)

            obj = addText(this, "./scripts/camera1.lua", "5000")
            setPositionv(obj, this_position)
            setScale(obj, 0.75, 0.75, 0.0)
            setVelocity(obj, cam_vel[1], 2.0, cam_vel[3])
            setFadeRate(obj, -.25)
            setColor(obj, 1.0, 1.0, 1.0)

            removeObject(this)
        end
    end
    return
end
