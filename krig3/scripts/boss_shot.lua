local shot = require 'scripts/shot'

-- Overridden Engine Callbacks
function on_load(this)
  krig.object.set_model(this, "GreenShot.mdl")
  krig.object.set_type_id(this, 20)
  krig.object.set_scale(this, 4.0, 4.0, 4.0)
  shot.on_load(this, options)
end

function on_update(this, elapsedTime)
  in_view = krig.object.get_in_view(this)

  if in_view == 0 then
    krig.level.remove_object(this)
  end
end

function on_collision(this, temp)
  typeId = krig.object.get_type_id(temp)

  if typeId ~= 1 and typeId ~= 3 and typeId ~= 2 then
    krig.level.remove_object(this)
  end
end
