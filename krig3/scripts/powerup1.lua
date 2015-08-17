-- Configuration
MAX_DURATION = 7.0
duration     = 0.0

-- Overridden Engine Callbacks
function on_load(this)
  setModel(this, "Powerup.mdl")
  setScale(this, 2.0, 2.0, 2.0)
  setRotationVelocity(this, 0.0, 2.0, 0.0);
  setTypeId(this, 5);
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
