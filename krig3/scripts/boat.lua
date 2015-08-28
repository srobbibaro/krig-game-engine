-- Overridden Engine Callbacks
local function on_collision(this, temp)
  temp_id = getTypeId(temp)

  if temp_id == 100 then
    orientOnTerrain(this)
  end
end

boat = {
  on_collision = on_collision
}

return boat
