-- Configuration
duration = 0.0

-- Overridden Engine Callbacks
function on_load(this)
  setModel(this, "blueshot.mdl")
  setTypeId(this, 2)
  enableAlwaysLit(this)

  playSound(this, "laser.wav")
end

function on_update(this, elapsedTime)
  duration = duration + elapsedTime

  if duration > 1.5 then
    removeObject(this)
    duration = 0.0
  end
end

function on_collision(this, temp)
  typeId = getTypeId(temp)

  if typeId ~= 0 and typeId ~= 10 then
    removeObject(this)
    duration = 0.0
  end
end

function on_unload(this) end