local shooting_object = require 'scripts/shooting_object'

-- Configuration
progress        = 0
life            = 10
lives           = 3
numMissiles     = 0
nextMissileShot = 0.0

leftDown        = 0
rightDown       = 0
upDown          = 0
downDown        = 0

invul            = 0.0
collisionRecover = 0
interpTime       = 0.0

-- Overridden Engine Callbacks
function on_load(this)
  this:set_model("Ship.mdl")

  this.rotation = krig.rotation.from_euler({0.0, 1.57, 0.0})
  this.scale             = {2.0, 2.0, 2.0}
  this.velocity          = {40.0, 0.0, 0.0}
  this.rotation_velocity = {-10.0, 0.0, 0.0}
  this.type_id           = 0
  this:save()

  this:suspend(0.25)

  shooting_object.setupShots(this, "./scripts/player_shot.lua", 0.25)
end

function on_update(this, elapsedTime)
  this = this:load()

  shooting_object.update_shots(elapsedTime)

  if nextMissileShot > 0.0 then nextMissileShot = nextMissileShot - elapsedTime end
  if invul > 0.0 then invul = invul - elapsedTime end

  -- handle invul --
  if collisionRecover == 1 then
    if this.draw_enabled then
      this.draw_enabled = false
    else
      this.draw_enabled = true
    end

    if invul <= 0.0 then
      this.draw_enabled = true
      collisionRecover  = 0
    end
  end

  -- handle individual states
  if progress == 0 then
    camera = krig.get_camera():load()
    tx     = camera.position[1] - 20.0
    if this.position[1] >= tx then
      this.velocity = {8.0, 0.0, 0.0}
      progress = 1
      this:suspend(1.0)
    else
      this:suspend(0.25)
    end
  elseif progress == 1 then
    this.velocity = {10.0, 0.0, 0.0}
    this.rotation_velocity = {0.0, 0.0, 0.0}
    progress = 2
  elseif progress == 2 then
    this:setup_interpolation(this.rotation, 0.0, krig.rotation.from_euler(0.0, 1.57, 0.0), 0.5)
    this:update_interpolation_value(0.0)
    this.interpolation_enabled = true

    progress = 3
  elseif progress == 3 then
    interpTime = interpTime + elapsedTime
    this:update_interpolation_value(interpTime)

    if interpTime > 0.5 then
      this.interpolation_enabled = false
      progress = 4
    end
  elseif progress == 4 then
    camera = krig.get_camera():load()

    this.velocity[1] = camera.velocity[1]
    this.velocity[2] = camera.velocity[2]

    if krig.test_special_key_pressed(101) == 1 then upDown = 1 end
    if krig.test_special_key_pressed(103) == 1 then downDown = 1 end
    if krig.test_special_key_pressed(100) == 1 then leftDown = 1 end
    if krig.test_special_key_pressed(102) == 1 then rightDown = 1 end

    if krig.test_special_key_released(101) == 1 then upDown = 0 end
    if krig.test_special_key_released(103) == 1 then downDown = 0 end
    if krig.test_special_key_released(100) == 1 then leftDown = 0 end
    if krig.test_special_key_released(102) == 1 then rightDown = 0 end

    if upDown == 1 then this.velocity[2] = this.velocity[2] + 10 end
    if downDown == 1 then this.velocity[2] = this.velocity[2] - 10 end
    if leftDown == 1 then this.velocity[1] = this.velocity[1] - 10 end
    if rightDown == 1 then this.velocity[1] = this.velocity[1] + 10 end

    -- Ax + By + Cz + D = 0
    plane = camera:get_frustum_plane(1)
    x     = -(((plane[3] * this.position[3]) + plane[4]) / plane[1])

    if this.position[1] > x - this.bounding_sphere_radius then
      this.position[1] = x - this.bounding_sphere_radius
    end

    plane = camera:get_frustum_plane(0)
    x     = -(((plane[3] * this.position[3]) + plane[4]) / plane[1])

    if this.position[1] < x + this.bounding_sphere_radius then
      this.position[1] = x + this.bounding_sphere_radius
    end

    plane = camera:get_frustum_plane(2)
    y     = -(((plane[3] * this.position[3]) + plane[4]) / plane[2])

    if this.position[2] < y + this.bounding_sphere_radius then
      this.position[2] = y + this.bounding_sphere_radius
    end

    plane = camera:get_frustum_plane(3)
    y     = -(((plane[3] * this.position[3]) + plane[4]) / plane[2])

    if this.position[2] > y - this.bounding_sphere_radius then
      this.position[2] = y - this.bounding_sphere_radius
    end

    if krig.test_key_pressed(32) == 1 then
      shooting_object.attemptShot(this, (this.bounding_sphere_radius - 1.0))
    end

    if krig.test_key_pressed(string.byte("m", 1)) == 1 and
       nextMissileShot <= 0.0 and
       numMissiles > 0 then
      shot = krig.level.add_object("./scripts/player_missile.lua")
      obj  = shooting_object.setShot(this, shot)
      nextMissileShot = .75
      numMissiles = numMissiles - 1
    end
  end

  this:save()
end

function on_collision(this, temp)
  temp_id = temp:load().type_id

  if collisionRecover == 0 and (temp_id == 1 or temp_id == 10 or temp_id == 4) then
    life             = life - 1
    collisionRecover = 1
    invul            = 1.0

    if life <= 0 then
      life = 10
      if lives > 0 then lives = lives - 1 end
    end
  elseif collisionRecover == 0 and temp_id == 20 then
    life             = life - 2
    collisionRecover = 1
    invul            = 1.0
    if life <= 0 then
      life = 10
      if lives > 0 then lives = lives - 1 end
    end
  elseif temp_id == 5 then
    numMissiles = numMissiles + 5
    if numMissiles > 30 then numMissiles = 30 end
  elseif temp_id == 6 then
    life = life + 5
    if life > 10 then life = 10 end
  elseif temp_id == 7 then
    lives = lives + 1
    if lives > 99 then lives = 99 end
  end
end
