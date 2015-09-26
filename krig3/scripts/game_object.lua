local function on_load(this, options)
  if options == nil then return end

  if options.model    then this:set_model(options.model) end
  if options.position then this.position = options.position end
  if options.rotation then this.rotation = options.rotation end
  if options.velocity then this.velocity = options.velocity end
  if options.scale    then this.scale = options.scale end
end

game_object = {
  on_load = on_load
}

return game_object
