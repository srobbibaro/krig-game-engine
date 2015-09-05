local enemy = require 'scripts/enemy'

-- Configuration
enemy.score = 500

-- Overridden Engine Callbacks
function on_load(this, options)
  krig.object.set_model(this, "redrock.mdl")
  krig.object.set_scale(this, 1.0, 1.0, 1.0)
  krig.object.set_rotation(this, 0.0, 0.0, 0.0)

  krig.object.set_type_id(this, 4)
end
