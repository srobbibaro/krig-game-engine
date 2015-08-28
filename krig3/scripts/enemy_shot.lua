local shot = require 'scripts/shot'

-- Overridden Engine Callbacks
function on_load(this, options)
  setModel(this, "GreenShot.mdl")
  setTypeId(this, 10)
  shot.on_load(this, options)
end

function on_collision(this, temp)
  typeId = getTypeId(temp)

  if typeId ~= 1 and typeId ~= 3 and typeId ~= 2 then
    removeObject(this)
    duration = 0
  end
end

on_update = shot.on_update
