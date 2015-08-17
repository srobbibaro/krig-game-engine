-- Overridden Engine Callbacks
function on_load(this)
  setModel(this, "Beam.mdl")
  setRotation(this, 0.0, 0.0, 0.0)
  disableCollisionDetection(this)
end

function on_update(this, elapsedTime) end
function on_unload(this) end
function on_collision(this, temp) end
