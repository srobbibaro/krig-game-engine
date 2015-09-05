local shot = require 'scripts/shot'

-- Overridden Engine Callbacks
function on_load(this, options)
  krig.object.set_model(this, "blueshot.mdl")
  krig.object.set_type_id(this, 2)
  shot.on_load(this, options)
end

function on_collision(this, temp)
  typeId = krig.object.get_type_id(temp)

  if typeId ~= 0 and typeId ~= 10 then
    krig.level.remove_object(this)
    duration = 0.0
  end
end

on_update = shot.on_update
