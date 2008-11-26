progress = 0

function on_load(this)
    -- set test zone enable false
    setVelocity(this, 40.0, 0.0, 0.0)
    setRotationVelocity(this, -10.0, 0.0, 0.0)
    suspend(this, 0.25)
    return
end

function on_update(this, elapsedTime)
    if progress == 0 then
        camera = getCamera()
        mx, my, mz = getPosition(this)
        cx, cy, cz = getPosition(camera)
        tx = cx - 20.0
        if mx >= tx then
            -- set test zone enable true
            setVelocity(this, 8.0, 0.0, 0.0)
            progress = 1
            suspend(this, 1.0)
	  else
            suspend(this, 0.25)
	  end
    elseif progress == 1 then
        setVelocity(this, 10.0, 0.0, 0.0)
        setRotationVelocity(this, 0.0, 0.0, 0.0)
        progress = 2
    elseif progress == 2 then
        rx, ry, rz = getRotation(this)
        timer = getTimer(this)
        setInterpolationVariable(this, 0, timer, (timer + 0.5))
        setInterpolationRotationStart(this, rx, ry, rz)
        setInterpolationRotationEnd(this, 0.0, 1.57, 0.0)
        setInterpolationEnable(this, 1)
        suspend(this, 0.75)
        progress = 3
    elseif progress == 3 then
        setInterpolationEnable(this, 0)
        progress = 4
    end
    
    return
end

function on_unload(this)
    return
end

