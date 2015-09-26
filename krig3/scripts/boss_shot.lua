local shot = require 'scripts/shot'

-- Overridden Engine Callbacks
function on_load(this, options)
  this:set_model("GreenShot.mdl")
  this.type_id = 20
  this.scale   = {4.0, 4.0, 4.0}
  this:save()
  shot.on_load(this, options)
end

function on_update(this, elapsedTime)
  this = this:load()

  if not this.in_view then
    krig.level.remove_object(this)
  end
end

function on_collision(this, temp)
  type_id = temp:load().type_id

  if type_id ~= 1 and type_id ~= 3 and type_id ~= 2 then
    krig.level.remove_object(this)
  end
end
