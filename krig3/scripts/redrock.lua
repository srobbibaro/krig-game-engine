local enemy = require 'scripts/enemy'

-- Configuration
enemy.score = 500

-- Overridden Engine Callbacks
function on_load(this, options)
  setModel(this, "redrock.mdl")
  setScale(this, 1.0, 1.0, 1.0)
  setRotation(this, 0.0, 0.0, 0.0)

  setTypeId(this, 4)
end
