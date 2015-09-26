local shot = require 'scripts/shot'

-- Overridden Engine Callbacks
function on_load(this, options)
  this:set_model("GreenShot.mdl")
  this.type_id = 10
  shot.on_load(this, options)
  this:save()
end

function on_collision(this, temp)
  type_id = temp:load().type_id

  if type_id ~= 1 and type_id ~= 3 and type_id ~= 2 then
    krig.level.remove_object(this)
    duration = 0
  end
end

on_update = shot.on_update
