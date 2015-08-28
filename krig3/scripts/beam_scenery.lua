-- Overridden Engine Callbacks
function on_load(this)
  setModel(this, "Beam.mdl")
  setRotation(this, 0.0, 0.0, 0.0)
  disableCollisionDetection(this)
end
