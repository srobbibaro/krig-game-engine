dofile('./scripts/base_enemy.lua')

-- Configuration
score = 500

-- Overridden Engine Callbacks
function on_load(this) 
  setModel(this, "Asteroid.mdl")
  setScale(this, 5.0, 5.0, 5.0)
  setRotation(this, 0.0, 0.0, 0.0)
  life = 3

  setTypeId(this, 4)
end

function on_update(this, elapsedTime)
  camera          = getCamera()
  this_position   = getPosition(this)
  camera_position = getPosition(camera)

  if this_position[2] < camera_position[2] - 20 then
    removeObject(this)
  end
end

function on_collision(this, temp)
  handle_collision(this, temp)
end

function on_unload(this) end
