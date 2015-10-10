local function setupShots(this, objectScript, initialNextShotTime)
  shooting_object.script                 = objectScript
  shooting_object.initial_next_shot_time = initialNextShotTime
  shooting_object.next_shot_time         = initialNextShotTime
end

local function update_shots(elapsedTime)
  shooting_object.next_shot_time = shooting_object.next_shot_time - elapsedTime
end

local function attemptShot(this, dir_offset, up_offset)
  if shooting_object.next_shot_time <= 0.0 then
    shot = krig.get_active_level():add_object(shooting_object.script)
    setShot(this, shot, dir_offset, up_offset)
    shooting_object.next_shot_time = shooting_object.initial_next_shot_time
  end
end

local function attemptShots(this, num_shots, dir_offset, up_offset)
  if num_shots == nil then num_shots = 1 end

  if shooting_object.next_shot_time <= 0.0 then
    for i = 1, num_shots do
      shot = krig.get_active_level():add_object(shooting_object.script)
      setShot(this, shot, dir_offset[i], up_offset[i])
    end
    shooting_object.next_shot_time = shooting_object.initial_next_shot_time
  end
end

function setShot(this, obj, dir_offset, up_offset)
  up_offset  = up_offset or 0
  dir_offset = dir_offset or 0

  local direction = krig.vector.scale(this.direction, dir_offset)
  local up        = krig.vector.scale(this.up, up_offset)

  obj.position          = krig.vector.add(krig.vector.add(this.position, direction), up)
  obj.rotation          = krig.rotation.copy(this.rotation)
  obj.rotation_velocity = krig.vector.scale(this.direction, 8.0)
  obj.speed             = {25.0, 0.0, 0.0}
  obj:save()
end

shooting_object = {
  setupShots             = setupShots,
  setShot                = setShot,
  update_shots           = update_shots,
  attemptShot            = attemptShot,
  attemptShots           = attemptShots,
  next_shot_time         = 1.0,
  initial_next_shot_time = 1.0,
  script                 = nil
}

return shooting_object
