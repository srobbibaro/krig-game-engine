stopped = 0

function on_load(this)
    setVelocity(this, 10.0, 0.0, 0.0)
    return
end

function on_update(this, elapsedTime)
    mx, my, mz = getPosition(this)
    if stopped == 0 then 
        if mx >= 970.0 then
            stopped = 1
            setVelocity(this, 0.0, 0.0, 0.0)
            player = getPlayer()
            pvx, pvy, pvz = getVelocity(player)
            setVelocity(player, (pvx - 10.0), pvy, pyz)
        end
    end

    return
end

function on_unload(this)
    return
end

function on_collision(this, temp)
    return
end
