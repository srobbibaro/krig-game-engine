-- Helper Functions
local function create_score_text(this)
  krig.level.add_text("./scripts/points.lua", enemy.score, {position = this.position})
end

local function create_powerup(this, powerup_num)
  local obj = krig.level.add_object(string.format("./scripts/powerup%d.lua", powerup_num))

  if obj ~= nil then
    obj.position = krig.vector.copy(this.position)
    obj:save()
  end
end

local function create_explosion(this)
  obj = krig.level.add_object("./scripts/explosion.lua")
  obj.position = this.position
  obj:save()
end

local function on_collision(this, temp)
  this    = this:load()
  type_id = temp:load().type_id

  if type_id == 2 or (type_id == 4 and this.type_id ~= 4) then
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
