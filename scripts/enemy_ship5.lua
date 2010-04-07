dofile('./scripts/base_object.lua')
dofile('./scripts/base_enemy.lua')

score = 100

state = 0
function on_load(this)
    setModel(this, "Enemy.mdl")
    setScale(this, 2.0, 2.0, 2.0)
    setRotation(this, 0.19625, -2.7475, 0.0)
    setTypeId(this, 1)   
    return
end

function on_update(this, elapsedTime)
    this_pos = getPosition(this)
    camera = getCamera()
    cam_pos = getPosition(camera)
    tx = cam_pos[1] + 70.0
    if tx >= this_pos[1] and state == 0 then
        setSpeed(this, 10.0, 0.0, 0.0)
        this_dir = getDirection(this)
        setRotationVelocityAxis(this, this_dir[1], this_dir[2], this_dir[3], 20.0)
        state = 1
    end
	return
end

function on_unload(this)
	return
end

function on_collision(this, temp)
    handle_collision(this, temp)
    return
end

