local enemy       = require 'scripts/enemy'
local enemy_ship  = require 'scripts/enemy_ship'
local game_object = require 'scripts/game_object'

-- Configuration
state         = 0
orig_position = nil
interpTime    = 0.0
enemy.score   = 20000
enemy.life    = 40
life          = enemy.life

-- Overridden Engine Callbacks
function on_load(this, options)
  game_object.on_load(this, options)

  this:set_model("FishBoss.mdl")
  this.scale    = {16.0, 16.0, 16.0}
  this.rotation = krig.rotation.from_euler({0.0, -1.5708, -1.2})
  this.type_id  = 1
  this:save()
end

function on_update(this, elapsedTime)
  this = this:load()

  -- Disable collision detection when the boss is close to the screen
  -- since it is a little harder to judge for the player
  if (this.position[3] > 8.0) then
    this.collision_detection_enabled = false
  else
    this.collision_detection_enabled = true
  end

  if state == 0 then
    state = 2
    orig_position = krig.vector.copy(this.position)
  elseif state == 1 then
    interpTime = interpTime + elapsedTime
    this:update_interpolation_value(interpTime)

    if interpTime >= 2.0 then
      state = 2
      this.interpolation_enabled = false
    end
  elseif state == 2 then
    interpTime = interpTime + elapsedTime

    if interpTime > 6.0 then
      this.speed = {0.0, 0.0, 0.0}

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
        start_rotation = krig.rotation.from_euler({0.0, -1.5708, -0.8})
        end_rotation   = krig.rotation.from_euler({0.0, -1.5708, -6.0})
      elseif action == 1 then
        -- left to right movement across screen
        new_position = {
          orig_position[1] - 60.0,
          orig_position[2] + math.random(10, 30),
          orig_position[3]
        }
        start_rotation = krig.rotation.from_euler({0.0, 1.5708, 0.8})
        end_rotation   = krig.rotation.from_euler({0.0, 1.5708, 6.0})
      elseif action == 2 then
        -- background to foreground movement across screen
        new_position = {
          orig_position[1] + math.random(-60, -20),
          orig_position[2],
          orig_position[3] + math.random(-50.0, -30.0)
        }
        start_rotation = krig.rotation.from_euler({1.37, 3.14, 3.14})
        end_rotation   = krig.rotation.from_euler({-2.0, 3.14, 3.14})
      else
        -- foreground to background movement across screen
        new_position = {
          orig_position[1] + math.random(-60, -20),
          orig_position[2] + math.random(15, 35),
          orig_position[3] + 20.0
        }
        start_rotation = krig.rotation.from_euler({2.0, 0.0, -3.14})
        end_rotation   = krig.rotation.from_euler({-1.37, 0.0, -3.14})
      end

      -- set orientation and speed
      this.position = new_position
      this.rotation = start_rotation
      this.speed = {45.0 + math.random(0, 15), 0.0, 0.0}

      -- setup interpolation
      this:setup_interpolation(
        start_rotation, 0.0,
        end_rotation, 2.0
      )
      this:update_interpolation_value(0.0)
      this.interpolation_enabled = true

      interpTime = 0.0

      -- transition to active interpolation state
      state = 1
    end
  end

  this:save()
end

function on_collision(this, temp)
  enemy.on_collision(this, temp)
  life = enemy.life
end
