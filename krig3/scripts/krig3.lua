local game_object = require 'scripts/game_object'

-- Overridden Engine Callbacks
function on_load(this, options)
  game_object.on_load(this, options)
  this:save()
  this:set_texture("krig3.png")
end

function on_draw(this)
  gl.PushMatrix()
  gl.Translate (0.0, 0.0, -4.0)
  gl.Begin("TRIANGLES")
  gl.Color (1.0, 1.0, 1.0)
  gl.Vertex (-4.0, 3.0, 0.0)
  gl.Color (0.0, 0.0, 0.0)
  gl.Vertex (0.0, 0.0, 0.0)
  gl.Color (1.0, 1.0, 1.0)
  gl.Vertex (4.0, 3.0, 0.0)

  gl.Vertex (-4.0, 3.0, 0.0)
  gl.Vertex (-4.0, -3.0, 0.0)
  gl.Color (0.0, 0.0, 0.0)
  gl.Vertex (0.0, 0.0, 0.0)

  gl.Color (1.0, 1.0, 1.0)
  gl.Vertex (-4.0, -3.0, 0.0)
  gl.Vertex (4.0, -3.0, 0.0)
  gl.Color (0.0, 0.0, 0.0)
  gl.Vertex (0.0, 0.0, 0.0)

  gl.Color (1.0, 1.0, 1.0)
  gl.Vertex (4.0, -3.0, 0.0)
  gl.Vertex (4.0, 3.0, 0.0)
  gl.Color (0.0, 0.0, 0.0)
  gl.Vertex (0.0, 0.0, 0.0)
  gl.End()
  gl.PopMatrix()
end
