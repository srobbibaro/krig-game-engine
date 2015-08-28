local shot = require 'scripts/shot'

-- Overridden Engine Callbacks
function on_load(this, options)
  setModel(this, "blueshot.mdl")
  setTypeId(this, 2)
  shot.on_load(this, options)
end

function on_collision(this, temp)
  typeId = getTypeId(temp)

  if typeId ~= 0 and typeId ~= 10 then
    removeObject(this)
    duration = 0.0
  end
end

on_update = shot.on_update
