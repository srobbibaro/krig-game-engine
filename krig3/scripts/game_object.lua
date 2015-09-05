local function on_load(this, options)
  if options == nil then return end

  if options.model then krig.object.set_model(this, options.model) end
  if options.position then krig.object.set_position(this, options.position) end
  if options.rotation then krig.object.set_rotation(this, options.rotation) end
  if options.velocity then krig.object.set_velocity(this, options.velocity) end
  if options.scale then krig.object.set_scale(this, options.scale) end
end

game_object = {
  on_load = on_load
}

return game_object
