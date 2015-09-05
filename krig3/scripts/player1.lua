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
  krig.object.set_model(this, "Ship.mdl")

  krig.object.set_rotation(this, 0.0, 1.57, 0.0)
  krig.object.set_scale(this, 2.0, 2.0, 2.0)
  krig.object.set_velocity(this, 40.0, 0.0, 0.0)
  krig.object.set_rotation_velocity(this, -10.0, 0.0, 0.0)
  krig.object.set_type_id(this, 0)

  krig.object.suspend(this, 0.25)

  shooting_object.setupShots(this, "./scripts/player_shot.lua", 0.25)
end

function on_update(this, elapsedTime)
  shooting_object.update_shots(elapsedTime)

  if nextMissileShot > 0.0 then nextMissileShot = nextMissileShot - elapsedTime end
  if invul > 0.0 then invul = invul - elapsedTime end

  -- handle invul --
  if collisionRecover == 1 then
    isDrawn = krig.object.get_draw_enabled(this)

    if isDrawn == 0 then
      krig.object.enable_draw(this)
    else
      krig.object.disable_draw(this)
    end

    if invul <= 0.0 then
      krig.object.enable_draw(this)
      collisionRecover = 0
    end
  end

  -- handle individual states
  if progress == 0 then
    camera = krig.get_camera()
    this_position = krig.object.get_position(this)
    camera_position = krig.object.get_position(camera)
    tx = camera_position[1] - 20.0
    if this_position[1] >= tx then
      krig.object.set_velocity(this, 8.0, 0.0, 0.0)
      progress = 1
      krig.object.suspend(this, 1.0)
    else
      krig.object.suspend(this, 0.25)
    end
  elseif progress == 1 then
    krig.object.set_velocity(this, 10.0, 0.0, 0.0)
    krig.object.set_rotation_velocity(this, 0.0, 0.0, 0.0)
    progress = 2
  elseif progress == 2 then
    this_rotation = krig.object.get_rotation(this)
    krig.object.set_interpolation_variable_begin_value(this, 0.0)
    krig.object.set_interpolation_variable_end_value(this, 0.5)
    krig.object.set_interpolation_variable_current_value(this, 0.0)
    krig.object.set_interpolation_rotation_start(this, this_rotation)
    krig.object.set_interpolation_rotation_end(this, 0.0, 1.57, 0.0)
    krig.object.set_interpolation_enable(this, 1)

    progress = 3
  elseif progress == 3 then
    interpTime = interpTime + elapsedTime
    krig.object.set_interpolation_variable_current_value(this, interpTime)

    if interpTime > 0.5 then
      krig.object.set_interpolation_enable(this, 0)
      progress = 4
    end
  elseif progress == 4 then
    this_position   = krig.object.get_position(this)
    this_velocity   = krig.object.get_velocity(this)
    camera          = krig.get_camera()
    camera_velocity = krig.object.get_velocity(camera)

    this_velocity[1] = camera_velocity[1]
    this_velocity[2] = camera_velocity[2]

    if krig.test_special_key_pressed(101) == 1 then upDown = 1 end
    if krig.test_special_key_pressed(103) == 1 then downDown = 1 end
    if krig.test_special_key_pressed(100) == 1 then leftDown = 1 end
    if krig.test_special_key_pressed(102) == 1 then rightDown = 1 end

    if krig.test_special_key_released(101) == 1 then upDown = 0 end
    if krig.test_special_key_released(103) == 1 then downDown = 0 end
    if krig.test_special_key_released(100) == 1 then leftDown = 0 end
    if krig.test_special_key_released(102) == 1 then rightDown = 0 end

    if upDown == 1 then this_velocity[2] = this_velocity[2] + 10 end
    if downDown == 1 then this_velocity[2] = this_velocity[2] - 10 end
    if leftDown == 1 then this_velocity[1] = this_velocity[1] - 10 end
    if rightDown == 1 then this_velocity[1] = this_velocity[1] + 10 end

    -- Ax + By + Cz + D = 0
    this_pos = krig.object.get_position(this)
    plane    = krig.camera.get_frustum_plane(1)
    x        = -(((plane[3] * this_pos[3]) + plane[4]) / plane[1])

    if this_pos[1] > x - krig.object.get_bounding_sphere_radius(this) then
      this_pos[1] = x - krig.object.get_bounding_sphere_radius(this)
    end

    plane = krig.camera.get_frustum_plane(0)
    x     = -(((plane[3] * this_pos[3]) + plane[4]) / plane[1])

    if this_pos[1] < x + krig.object.get_bounding_sphere_radius(this) then
      this_pos[1] = x + krig.object.get_bounding_sphere_radius(this)
    end

    plane = krig.camera.get_frustum_plane(2)
    y     = -(((plane[3] * this_pos[3]) + plane[4]) / plane[2])

    if this_pos[2] < y + krig.object.get_bounding_sphere_radius(this) then
      this_pos[2] = y + krig.object.get_bounding_sphere_radius(this)
    end

    plane = krig.camera.get_frustum_plane(3)
    y     = -(((plane[3] * this_pos[3]) + plane[4]) / plane[2])

    if this_pos[2] > y - krig.object.get_bounding_sphere_radius(this) then
      this_pos[2] = y - krig.object.get_bounding_sphere_radius(this)
    end

    krig.object.set_velocity(this, this_velocity)
    krig.object.set_position(this, this_pos)

    if krig.test_key_pressed(32) == 1 then
      shooting_object.attemptShot(this, (krig.object.get_bounding_sphere_radius(this) - 1.0))
    end

    if krig.test_key_pressed(string.byte("m", 1)) == 1 and
       nextMissileShot <= 0.0 and
       numMissiles > 0 then
      obj = shooting_object.setShot(this, "./scripts/player_missile.lua")
      nextMissileShot = .75
      numMissiles = numMissiles - 1
    end
  end
end

function on_collision(this,temp)
  tempId = krig.object.get_type_id(temp)

  if collisionRecover == 0 and (tempId == 1 or tempId == 10 or tempId == 4) then
    life             = life - 1
    collisionRecover = 1
    invul            = 1.0

    if life <= 0 then
      life = 10
      if lives > 0 then lives = lives - 1 end
    end
  elseif collisionRecover == 0 and tempId == 20 then
    life             = life - 2
    collisionRecover = 1
    invul            = 1.0
    if life <= 0 then
      life = 10
      if lives > 0 then lives = lives - 1 end
    end
  elseif tempId == 5 then
    numMissiles = numMissiles + 5
    if numMissiles > 30 then numMissiles = 30 end
  elseif tempId == 6 then
    life = life + 5
    if life > 10 then life = 10 end
  elseif tempId == 7 then
    lives = lives + 1
    if lives > 99 then lives = 99 end
  end
end
