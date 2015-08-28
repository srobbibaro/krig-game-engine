local enemy_ship      = require 'scripts/enemy_ship'
local shooting_object = require 'scripts/shooting_object'

-- Overridden Engine Callbacks
function on_load(this, options)
  enemy_ship.on_load(this, options)
  shooting_object.setupShots(this, "./scripts/enemy_shot.lua", 0.5)
end

function on_update(this, elapsedTime)
  shooting_object.update_shots(elapsedTime)

  plr      = getPlayer()
  this_pos = getPosition(this)
  plr_pos  = getPosition(plr)
  in_view  = getInView(this)

  if plr_pos[2] < (this_pos[2] + .5) and
     plr_pos[2] > (this_pos[2] - .5) and
    in_view == 1 then
    shooting_object.attemptShot(this, getBoundingSphereRadius(this))
  end
end

on_collision = enemy_ship.on_collision
