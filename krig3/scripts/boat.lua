-- Overridden Engine Callbacks
local function on_collision(this, temp)
  temp_id = krig.object.get_type_id(temp)

  if temp_id == 100 then
    krig.object.orient_on_terrain(this)
  end
end

boat = {
  on_collision = on_collision
}

return boat
