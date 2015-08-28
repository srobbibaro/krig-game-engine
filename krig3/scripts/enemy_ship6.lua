dofile('./scripts/base_enemy.lua')

-- Configuration
local state      = 0
local delay      = 0.0
local interpTime = 0.0

-- Overridden Engine Callbacks
function on_load(this)
  setModel(this, "Enemy.mdl")
  setScale(this, 2.0, 2.0, 2.0)
  setRotation(this, 0.19625, -2.7475, 0.0)
  setTypeId(this, 1)
end

function on_update(this, elapsedTime)
  this_pos = getPosition(this)
  camera   = getCamera()
  cam_pos  = getPosition(camera)
  tx       = cam_pos[1] + 70.0

  if state == 0 then
    if tx >= this_pos[1] then
      setSpeed(this, 10.0, 0.0, 0.0)
      this_dir = getDirection(this)
      setRotationVelocityAxis(this, this_dir[1], this_dir[2], this_dir[3], 20.0)
      state = 1
      delay = 5.0
    end
  elseif state == 1 then
    delay = delay - elapsedTime
    if delay <= 0 then
      setSpeed(this, 5.0, 0.0, 0.0)
      state = 2
      delay = 0.0
    end
  elseif state == 2 then
    this_rotation = getRotation(this)
    setInterpolationVariableBeginValue(this, 0.0)
    setInterpolationVariableEndValue(this, 0.5)
    setInterpolationVariableCurrentValue(this, 0.0)
    setInterpolationRotationStartv(this, this_rotation)
    setInterpolationRotationEnd(this, -0.785, -3.925, 0.0)
    setInterpolationEnable(this, 1)

    setSpeed(this, 30.0)
    setRotationVelocity(this, 0.0, 0.0, 0.0)
    state      = 3
    interpTime = 0.0
  elseif state == 3 then
    interpTime = interpTime + elapsedTime
    setInterpolationVariableCurrentValue(this, interpTime)
    if interpTime >= 0.5 then
      setInterpolationEnable(this, 0)
      state = 4
      delay = 0.0
    end
  end
end

function on_collision(this, temp)
  handle_collision(this, temp)
end

function on_unload(this) end
