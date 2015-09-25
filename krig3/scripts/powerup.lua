-- Configuration
local duration = 0.0

-- Overridden Engine Callbacks
local function on_update(this, elapsedTime)
  duration = duration + elapsedTime

  if duration >= powerup.max_duration then
    krig.level.remove_object(this)
  end
end

local function on_collision(this, temp)
  if temp:load().type_id == 0 then
    krig.play_sound(this, "pause.wav")
    krig.level.remove_object(this)
  end
end

powerup = {
  on_update    = on_update,
  on_collision = on_collision,
  max_duration = 2.0
}

return powerup
