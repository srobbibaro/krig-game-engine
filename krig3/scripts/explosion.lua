-- Configuration
duration = 0.0

-- Overridden Engine Callbacks
function on_load(this)
  duration = 0.0
  setModel(this, "Explosion.mdl")
  setScaleRate(this, 1.0, 1.0, 1.0)
  disableCollisionDetection(this)
  enableAlwaysLit(this)
end

function on_update(this, elapsedTime)
  duration = duration + elapsedTime

  if duration >= 2.0 then
    removeObject(this)   
  end
end

function on_unload(this) end
function on_collision(this, temp) end
