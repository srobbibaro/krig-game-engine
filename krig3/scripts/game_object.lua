local function on_load(this, options)
  if options == nil then return end

  if options.model then setModel(this, options.model) end
  if options.position then setPositionv(this, options.position) end
  if options.rotation then setRotationv(this, options.rotation) end
  if options.velocity then setVelocityv(this, options.velocity) end
  if options.scale then setScalev(this, options.scale) end
end

game_object = {
  on_load = on_load
}

return game_object
