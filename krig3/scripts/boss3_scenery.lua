-- Configuration
state = 0
score = 0
life  = 1

-- Overridden Engine Callbacks
function on_load(this)
  setModel(this, "FishBoss.mdl")
  setScale(this, 16.0, 16.0, 16.0)
  setRotation(this, 0.0, 1.5708, 1.2)

  life = 40
  setTypeId(this, 3)

  disableCollisionDetection(this)
end

function on_update(this, elapsedTime)
  this_pos = getPosition(this)
  camera   = getCamera()
  cam_pos  = getPosition(camera)

  if state == 0 then
    if cam_pos[1] - 50.0 > this_pos[1] then
      this_rotation = getRotation(this)
      setInterpolationVariableBeginValue(this, 0.0)
      setInterpolationVariableEndValue(this, 4.0)
      setInterpolationVariableCurrentValue(this, 0.0)
      setInterpolationRotationStartv(this, this_rotation)

      this_rotation[3] = -2.0
      setInterpolationRotationEndv(this, this_rotation)
      setInterpolationEnable(this, 1)

      setSpeed(this, 40.0)
      state      = 1
      interpTime = 0.0
    end
  elseif state == 1 then
    interpTime = interpTime + elapsedTime
    setInterpolationVariableCurrentValue(this, interpTime)

    if interpTime >= 4.0 then
      state = 2
    end
  elseif state == 2 then
    if this_pos[2] < -10.0 then
      removeObject(this)
    end
  end
end

function on_unload(this) end
function on_collision(this, temp) end
