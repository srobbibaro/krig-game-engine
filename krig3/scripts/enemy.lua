-- Helper Functions
local function create_score_text(this)
  camera        = krig.get_camera()
  cam_vel       = krig.object.get_velocity(camera)
  this_position = krig.object.get_position(this)

  obj = krig.level.add_text("./scripts/camera1.lua", enemy.score)

  krig.object.set_position(obj, this_position)
  krig.object.set_scale(obj, 0.25, 0.25, 0.0)
  krig.object.set_velocity(obj, cam_vel[1], 2.0, cam_vel[3])
  krig.text.set_fade_rate(obj, -.25)
  krig.text.set_color(obj, 1.0, 1.0, 1.0)
end

local function create_powerup(this, powerup_num)
  local obj = krig.level.add_object(string.format("./scripts/powerup%d.lua", powerup_num))

  if obj ~= nil then
    this_position = krig.object.get_position(this)
    krig.object.set_position(obj, this_position)
  end
end

local function create_explosion(this)
  this_position = krig.object.get_position(this)

  obj = krig.level.add_object("./scripts/explosion.lua")
  krig.object.set_position(obj, this_position)
end

local function on_collision(this, temp)
  typeId     = krig.object.get_type_id(temp)
  thisTypeId = krig.object.get_type_id(this)

  if typeId == 2 or (typeId == 4 and thisTypeId ~= 4) then
    krig.play_sound(this, "explosion1.wav")

    enemy.life = enemy.life - 1
    if enemy.life <= 0 then
      player = krig.get_player()
      player_score = krig.get_script_value(player, "score") + enemy.score
      krig.set_script_value(player, "score", player_score)
      create_score_text(this)

      powerup_rand = math.random(100)

      if powerup_rand < 10 then
        create_powerup(this, 1)
      elseif powerup_rand  >= 10 and powerup_rand < 20 then
        create_powerup(this, 2)
      elseif powerup_rand  >= 20 and powerup_rand < 25 then
        create_powerup(this, 3)
      else
        create_explosion(this)
      end

      krig.level.remove_object(this)
    end
  end
end

enemy = {
  on_collision     = on_collision,
  create_explosion = create_explosion,
  life             = 1,
  score            = 0
}

return enemy
