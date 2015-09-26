local enemy           = require 'scripts/enemy'
local shooting_object = require 'scripts/shooting_object'

-- Configuration
state       = 0
enemy.score = 2000
enemy.life  = 40
life        = enemy.life

-- Overridden Engine Callbacks
function on_load(this)
  this:set_model("Boss.mdl")
  this.scale    = {4.0, 4.0, 4.0}
  this.rotation = krig.rotation.from_euler({0.0, -1.5708, 0.0})
  this.velocity = {-5.0, 0.0, 0.0}
  this.type_id  = 1
  this.collision_detection_enabled = false
  this:save()

  shooting_object.setupShots(this, './scripts/boss_shot.lua', 0.85)
end

function on_update(this, elapsedTime)
  shooting_object.update_shots(elapsedTime)

  this   = this:load()
  camera = krig.get_camera():load()

  if state == 0 then
    if this.position[1] <= (camera.position[1] + 11.0) then
      state                            = 1
      this.collision_detection_enabled = true
      this.velocity                    = {0.0, -10.0, 0.0}
      this:save()
    end
    return
  elseif state == 1 then
    if this.position[2] < (camera.position[2] - 5.0) then
      state         = 2
      this.velocity = {0.0, 10.0, 0.0}
    end
  elseif state == 2 then
    if this.position[2] > (camera.position[2] + 5.0) then
      state         = 1
      this.velocity = {0.0, -10.0, 0.0}
    end
  end

  plr = krig.get_player():load()

  if plr.in_view then
    radius = this.bounding_sphere_radius - 1.25
    shooting_object.attemptShots(this, 2, {radius, radius}, {radius, -radius})
  end

  this:save()
end

function on_collision(this, temp)
  enemy.on_collision(this, temp)
  life = enemy.life
end
