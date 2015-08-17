-- Configuration
MAX_DURATION = 2.0
duration     = 0.0

-- Overridden Engine Callbacks
function on_load(this)
  setModel(this, "Ship.mdl")
  setRotationVelocity(this, 0.0, 2.0, 0.0);
  setTypeId(this, 7);
end

function on_update(this, elapsedTime)
  duration = duration + elapsedTime

  if duration >= MAX_DURATION then
    removeObject(this)
  end
end

function on_collision(this, temp)
  typeId = getTypeId(temp)
  if typeId == 0 then
    playSound(this, "pause.wav")
    removeObject(this)
  end
end

function on_unload(this) end
