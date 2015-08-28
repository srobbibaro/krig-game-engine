local enemy_ship = require 'scripts/enemy_ship'
local shooting_object = require 'scripts/shooting_object'

-- Configuration
state      = 0
interpTime = 0.0

-- Overridden Engine Callbacks
function on_load(this, options)
  enemy_ship.on_load(this, options)
  shooting_object.setupShots(this, "./scripts/enemy_shot.lua", 0.5)
end

function on_update(this, elapsedTime)
  shooting_object.update_shots(elapsedTime)

  this_pos = getPosition(this)
  camera   = getCamera()
  cam_pos  = getPosition(camera)
  in_view  = getInView(this)
  player   = getPlayer()
  plr_pos  = getPosition(player)

  if state == 0 then
    if cam_pos[1] + 10.0 > this_pos[1] then
      this_rotation = getRotation(this)
      setInterpolationVariableBeginValue(this, 0.0)
      setInterpolationVariableEndValue(this, 0.5)
      setInterpolationVariableCurrentValue(this, 0.0)
      setInterpolationRotationStartv(this, this_rotation)

      this_rotation[3] = -1.57
      setInterpolationRotationEndv(this, this_rotation)
      setInterpolationEnable(this, 1)

      setSpeed(this, 10.0)
      state = 1
      interpTime = 0.0
    end
  elseif state == 1 then
    interpTime = interpTime + elapsedTime
    setInterpolationVariableCurrentValue(this, interpTime)

    if interpTime >= 0.5 then
      this_rotation = getRotation(this)
      setInterpolationVariableBeginValue(this, 0.0)
      setInterpolationVariableEndValue(this, 1.0)
      setInterpolationVariableCurrentValue(this, 0.0)
      setInterpolationRotationStartv(this, this_rotation)

      this_rotation[3] = -3.14
      setInterpolationRotationEndv(this, this_rotation)
      setInterpolationEnable(this, 1)

      setSpeed(this, 20.0)
      state = 2
      interpTime = 0.0
    end
  elseif state == 2 then
    interpTime = interpTime + elapsedTime
    setInterpolationVariableCurrentValue(this, interpTime)

    if interpTime >= 1.0 then
      this_rotation = getRotation(this)
      setInterpolationVariableBeginValue(this, 0.0)
      setInterpolationVariableEndValue(this, 1.0)
      setInterpolationVariableCurrentValue(this, 0.0)
      setInterpolationRotationStartv(this, this_rotation)

      this_rotation[3] = -4.61
      setInterpolationRotationEndv(this, this_rotation)
      setInterpolationEnable(this, 1)

      setSpeed(this, 20.0)
      state = 3
      interpTime = 0.0
    end
  elseif state == 3 then
    interpTime = interpTime + elapsedTime
    setInterpolationVariableCurrentValue(this, interpTime)

    if interpTime >= 1.0 then
      this_rotation = getRotation(this)
      setInterpolationVariableBeginValue(this, 0.0)
      setInterpolationVariableEndValue(this, 0.5)
      setInterpolationVariableCurrentValue(this, 0.0)
      setInterpolationRotationStartv(this, this_rotation)

      this_rotation[3] = -6.28
      setInterpolationRotationEndv(this, this_rotation)
      setInterpolationEnable(this, 1)

      setSpeed(this, 10.0)
      state = 4
      interpTime = 0.0
    end
  elseif state == 4 then
    interpTime = interpTime + elapsedTime
    setInterpolationVariableCurrentValue(this, interpTime)

    if interpTime >= 0.5 then
      setInterpolationEnable(this, 0)
      setSpeed(this, 10.0)
      state = 5
    end
  end

  if in_view == 1 and state > 0 then
    shooting_object.attemptShot(this, getBoundingSphereRadius(this))
  end
end

on_collision = enemy_ship.on_collision
