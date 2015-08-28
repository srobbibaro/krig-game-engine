-- Overridden Engine Callbacks
function on_load(this)
  setModel(this, "Building.mdl")
  setRotation(this, 0.0, 1.0, 0.0)
  disableCollisionDetection(this)
end
