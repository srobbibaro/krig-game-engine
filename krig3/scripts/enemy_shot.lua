local shot = require 'scripts/shot'

-- Overridden Engine Callbacks
function on_load(this, options)
  krig.object.set_model(this, "GreenShot.mdl")
  krig.object.set_type_id(this, 10)
  shot.on_load(this, options)
end

function on_collision(this, temp)
  typeId = krig.object.get_type_id(temp)

  if typeId ~= 1 and typeId ~= 3 and typeId ~= 2 then
    krig.level.remove_object(this)
    duration = 0
  end
end

on_update = shot.on_update
