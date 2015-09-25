local enemy_ship      = require 'scripts/enemy_ship'
local shooting_object = require 'scripts/shooting_object'

-- Configuration
local state      = 0
local interpTime = 0.0

-- Overridden Engine Callbacks
function on_load(this, options)
  enemy_ship.on_load(this, options)
  shooting_object.setupShots(this, "./scripts/enemy_shot.lua", 0.5)
end

function on_update(this, elapsedTime)
  shooting_object.update_shots(elapsedTime)

  this   = this:load()
  camera = krig.get_camera():load()

  if state == 0 then
    if camera.position[1] + 10.0 > this.position[1] then
      this_rotation = krig.rotation.to_euler(this.rotation)
      this_rotation[3] = 1.57
      this:setup_interpolation(
        this.rotation, 0.0,
        krig.rotation.from_euler(this_rotation), 0.5
      )
      this:update_interpolation_value(0.0)
      this.interpolation_enabled = true

      this.speed = {10.0, 0.0, 0.0}
      state      = 1
      interpTime = 0.0

      this:save()
    end
  elseif state == 1 then
    interpTime = interpTime + elapsedTime
    this:update_interpolation_value(interpTime)

    if interpTime >= 0.5 then
      this_rotation = krig.rotation.to_euler(this.rotation)
      this_rotation[3] = 3.14
      this:setup_interpolation(
        this.rotation, 0.0,
        krig.rotation.from_euler(this_rotation), 1.0
      )
      this:update_interpolation_value(0.0)
      this.interpolation_enabled = true

      this.speed = {20.0, 0.0, 0.0}
      state      = 2
      interpTime = 0.0

      this:save()
    end
  elseif state == 2 then
    interpTime = interpTime + elapsedTime
    this:update_interpolation_value(interpTime)

    if interpTime >= 1.0 then
      this_rotation = krig.rotation.to_euler(this.rotation)
      this_rotation[3] = 4.61
      this:setup_interpolation(
        this.rotation, 0.0,
        krig.rotation.from_euler(this_rotation), 1.0
      )
      this:update_interpolation_value(0.0)
      this.interpolation_enabled = true

      this.speed = {20.0, 0.0, 0.0}
      state      = 3
      interpTime = 0.0

      this:save()
    end
  elseif state == 3 then
    interpTime = interpTime + elapsedTime
    this:update_interpolation_value(interpTime)

    if interpTime >= 1.0 then
      this_rotation = krig.rotation.to_euler(this.rotation)
      this_rotation[3] = 6.28
      this:setup_interpolation(
        this.rotation, 0.0,
        krig.rotation.from_euler(this_rotation), 0.5
      )
      this:update_interpolation_value(0.0)
      this.interpolation_enabled = true

      this.speed = {10.0, 0.0, 0.0}
      state      = 4
      interpTime = 0.0

      this:save()
    end
  elseif state == 4 then
    interpTime = interpTime + elapsedTime
    this:update_interpolation_value(interpTime)

    if interpTime >= 0.5 then
      this.interpolation_enabled = false
      this.speed                 = {10.0, 0.0, 0.0}
      state                      = 5
      this:save()
    end
  end

  if this.in_view and state > 0 then
    shooting_object.attemptShot(this, this.bounding_sphere_radius)
  end
end

on_collision = enemy_ship.on_collision
