-- Overridden Engine Callbacks
function on_load(this, x_pos, z_pos, scale_factor)
  setModel(this, "Pinetree.mdl")
  setRotation(this, 0.0, 0.0, 0.0)

  if scale_factor ~= nil then
    setScale(this, scale_factor, scale_factor, scale_factor)
  else
    setScale(this, 2.0, 2.0, 2.0)
    scale_factor = 2.0
  end

  if x_pos ~= nil and z_pos ~= nil then
    setPosition(this, x_pos, 0.0, z_pos)
    setHeightFromTerrain(this, scale_factor)
  end

  disableCollisionDetection(this)
end

function on_update(this, elapsedTime) end
function on_unload(this) end
function on_collision(this, temp) end
