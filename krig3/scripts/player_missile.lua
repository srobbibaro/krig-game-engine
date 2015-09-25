-- Configuration
local duration = 0.0
local obj      = nil

-- Overridden Engine Callbacks
function on_load(this)
  this:set_model("Missle.mdl")
  this.type_id           = 2
  this.always_lit        = true
  this.rotation_velocity = {0.0, 0.0, 0.0}
  this.speed             = {2.0, 0.0, 0.0}
  this:save()

  krig.play_sound(this, "laser.wav")

  obj = krig.level.find_object_of_type(1)
end

function on_update(this, elapsedTime)
  this = this:load()

  duration = duration + elapsedTime

  if duration > 5.0 then
    krig.level.remove_object(this)
    duration = 0.0
    return
  end

  if obj.id >= 0 then
    obj = obj:load()

    this.speed = {this.speed[1] + 2.0 * elapsedTime, 0.0, 0.0}
    this.rotation_velocity  = krig.vector.scale(this.direction, 8.0)

    if obj.position[2] + obj.bounding_sphere_radius < this.position[2] then
      this.rotation_velocity = {0.0, 0.0, (-30.0 * elapsedTime)}
      --this.speed[1]          = 20.0
    elseif obj.position[2] - obj.bounding_sphere_radius > this.position[2] then
      this.rotation_velocity = {0.0, 0.0, (30.0 * elapsedTime)}
      --this.speed[1]          = 20.0
    end
  else
    obj = krig.level.find_object_of_type(1)
  end

  this:save()
end

function on_collision(this, temp)
  if temp:load().type_id ~= 0 then
    krig.level.remove_object(this)
    duration = 0.0
  end
end
