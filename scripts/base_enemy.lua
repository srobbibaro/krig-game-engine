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
            removeObject(this)
        end
    end
    return
end
