dofile('./scripts/base_enemy.lua')
dofile('./scripts/base_shooting_object.lua')

-- Overridden Engine Callbacks
function on_load(this)
  setModel(this, "Enemy.mdl")
  setScale(this, 2.0, 2.0, 2.0)
  setRotation(this, 0.0, -1.57, 0.0)
  setTypeId(this, 1)

  setupShots(this, "./scripts/enemy_shot.lua", 0.25)

  score = 100
end

function on_update(this, elapsedTime)
  update_shots(elapsedTime)

  plr      = getPlayer()
  this_pos = getPosition(this)
  plr_pos  = getPosition(plr)
  in_view  = getInView(this)

  if in_view == 0 then return end

  if plr_pos[2] > this_pos[2] then
    setVelocity(this, 2.0, 3.0, 0.0)
  elseif plr_pos[2] < this_pos[2] then
    setVelocity(this, 2.0, -3.0, 0.0)
  end

  if plr_pos[2] < (this_pos[2] + .5) and
     plr_pos[2] > (this_pos[2] - .5) and
    in_view == 1 then
    attemptShot(this, getBoundingSphereRadius(this))
  end
end

function on_collision(this, temp)
  handle_collision(this, temp)
end

function on_unload(this) end
