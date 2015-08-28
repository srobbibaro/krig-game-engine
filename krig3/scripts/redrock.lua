dofile('./scripts/base_enemy.lua')

-- Configuration
score = 500

-- Overridden Engine Callbacks
function on_load(this) 
  setModel(this, "redrock.mdl")
  setScale(this, 1.0, 1.0, 1.0)
  setRotation(this, 0.0, 0.0, 0.0)

  setTypeId(this, 4)
end
