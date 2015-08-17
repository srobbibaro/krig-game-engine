dofile('./scripts/sail_boat.lua')

-- Overridden Engine Callbacks
function on_load(this)
  setModel(this, "BoatCannon.mdl")
  setRotation(this, 0.0, -1.57, 0.0)
  disableCollisionDetection(this)
  setScale(this, 10.0, 10.0, 10.0)
end

function on_update(this, elapsedTime)
  orientOnTerrain(this, 0.0, 0.0, 0.0)
end
