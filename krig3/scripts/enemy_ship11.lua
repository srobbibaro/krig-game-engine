local enemy_ship      = require 'scripts/enemy_ship'
local shooting_object = require 'scripts/shooting_object'

-- Configuration
state       = 1
orig_height = 0.0
totalTime   = 1.0

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

  if state == 1 then
    if plr_pos[1] > this_pos[1] - 25.0 then
      setVelocity(this, 0.0, 5.0, 0.0)
      orig_height = this_pos[2]
      state = 2
    end
  elseif state == 2 then
    if this_pos[2] >= orig_height + 3.0 then
      setVelocity(this, 0.0, 0.0, 0.0)
      setSpeed(this, 40.0, 0.0, 0.0)
      state = 3
    end
  elseif state == 3 then
    totalTime = totalTime + elapsedTime

    if totalTime > 2.0 then
      state = 4
    end
    this_dir = getDirection(this)
    this_dir = {0.0, 0.0, 1.0}
    dir_to_player = {this_pos[1] - plr_pos[1] - 2.0, this_pos[2] - plr_pos[2], this_pos[3] - plr_pos[3]}
    dir_to_player = vector_normalize(dir_to_player)

    rot_axis = vector_crossProduct(this_dir, dir_to_player)
    rot_angle = -vector_dotProduct(this_dir, dir_to_player)

    setRotationAxis(this, rot_axis[1], rot_axis[2], rot_axis[3], rot_angle)
  end
end

on_collision = enemy_ship.on_collision
