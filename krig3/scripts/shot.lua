local game_object = require 'scripts/game_object'

-- Configuration
local duration = 0.0

-- Overridden Engine Callbacks
local function on_load(this, options)
  game_object.on_load(this, options)
  enableAlwaysLit(this)

  playSound(this, "laser.wav")
end

local function on_update(this, elapsedTime)
  duration = duration + elapsedTime

  if duration > 5.0 then
    removeObject(this)
    duration = 0
  end
end

shot = {
  on_load   = on_load,
  on_update = on_update
}

return shot
