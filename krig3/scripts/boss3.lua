local enemy = require 'scripts/enemy'
local enemy_ship = require 'scripts/enemy_ship'

-- Configuration
state         = 0
orig_position = nil
interpTime    = 0.0
enemy.score   = 20000
enemy.life    = 40
life        = enemy.life

-- Overridden Engine Callbacks
function on_load(this)
  krig.object.set_model(this, "FishBoss.mdl")
  krig.object.set_scale(this, 16.0, 16.0, 16.0)
  krig.object.set_rotation(this, 0.0, -1.5708, -1.2)

  krig.object.set_type_id(this, 1)
end

function on_update(this, elapsedTime)
  this_pos = krig.object.get_position(this)

  -- Disable collision detection when the boss is close to the screen
  -- since it is a little harder to judge for the player
  if (this_pos[3] > 8.0) then
    krig.object.disable_collision_detection(this)
  else
    krig.object.enable_collision_detection(this)
  end

  if state == 0 then
    state = 2
    orig_position = krig.object.get_position(this)
  elseif state == 1 then
    interpTime = interpTime + elapsedTime
    krig.object.set_interpolation_variable_current_value(this, interpTime)

    if interpTime >= 2.0 then
      state = 2
      krig.object.set_interpolation_enable(this, 0)
    end
  elseif state == 2 then
    interpTime = interpTime + elapsedTime

    if interpTime > 6.0 then
      krig.object.set_speed(this, 0.0)

      new_position   = {}
      start_rotation = {}
      end_rotation   = {}

      action = math.random(0,3)
      if action == 0 then
        -- right to left movement across screen
        new_position = {
          orig_position[1],
          orig_position[2] + math.random(10, 20),
          orig_position[3]
        }
        start_rotation = {0.0, -1.5708, -0.8}
        end_rotation = {start_rotation[1], start_rotation[2], -6.0}
      elseif action == 1 then
        -- left to right movement across screen
        new_position = {
          orig_position[1] - 60.0,
          orig_position[2] + math.random(10, 30),
          orig_position[3]
        }
        start_rotation = {0.0, 1.5708, 0.8}
        end_rotation = {start_rotation[1], start_rotation[2], 6.0}
      elseif action == 2 then
        -- background to foreground movement across screen
        new_position = {
          orig_position[1] + math.random(-60, -20),
          orig_position[2],
          orig_position[3] + math.random(-50.0, -30.0)
        }
        start_rotation = {1.37, 3.14, 3.14}
        end_rotation = {-2.0, start_rotation[2], start_rotation[3]}
      else
        -- foreground to background movement across screen
        new_position = {
          orig_position[1] + math.random(-60, -20),
          orig_position[2] + math.random(15, 35),
          orig_position[3] + 20.0
        }
        start_rotation = {2.0, 0.0, -3.14}
        end_rotation = {-1.37, start_rotation[2], start_rotation[3]}
      end

      -- set orientation and speed
      krig.object.set_position(this, new_position)
      krig.object.set_rotation(this, start_rotation)
      krig.object.set_speed(this, 45.0 + math.random(0, 15))

      -- setup interpolation
      krig.object.set_interpolation_enable(this, 1)
      krig.object.set_interpolation_variable_begin_value(this, 0.0)
      krig.object.set_interpolation_variable_end_value(this, 2.0)
      krig.object.set_interpolation_variable_current_value(this, 0.0)
      krig.object.set_interpolation_rotation_start(this, start_rotation)
      krig.object.set_interpolation_rotation_end(this, end_rotation)
      interpTime = 0.0

      -- transition to active interpolation state
      state = 1
    end
  end
end


function on_collision(this, temp)
  enemy.on_collision(this, temp)
  life = enemy.life
end
