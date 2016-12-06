-- Configuration
local cursor      = -0.2
local alph        = 0.1
local alphaChange = 0.01
local updown      = 0

-- Overridden Engine Callbacks
function on_load()
  krig.level.play_music("./music/menu.ogg", 0)

  krig.level.set_sky_box(
    0.7, 0.0, 0.2,
    0.2, 0.0, 0.2,
    0.0, 0.0, 0.3
  )

  krig.level.set_light_direction(0.96, 0.07, 0.13)

  obj = krig.level.add_object("./scripts/scenery.lua")
  obj:set_model("Ship.mdl")
  obj.position = {-5.0, -1.0, -50.0}
  obj.scale    = {5.0, 5.0, 5.0}
  obj.rotation = krig.rotation.from_euler({0.2, .5, 0.5})
  obj:save()

  obj = krig.level.add_object("./scripts/scenery.lua")
  obj:set_model("Enemy.mdl")
  obj.position = {7.5, -1.5, -20.0}
  obj.scale    = {5.0, 5.0, 5.0}
  obj.rotation = krig.rotation.from_euler({-0.1, -.1, -0.1})
  obj:save()

  obj = krig.level.add_object("./scripts/scenery.lua")
  obj:set_model("blueshot.mdl")
  obj.position = {2.5, -4.5, -15.0}
  obj.scale    = {3.0, 3.0, 3.0}
  obj.rotation = krig.rotation.from_euler({0.3, .2, 0.8})
  obj.always_lit = true
  obj:save()

  obj = krig.level.add_object("./scripts/scenery.lua")
  obj:set_model("blueshot.mdl")
  obj.position = {1.5, -1.5, -25.0}
  obj.scale    = {3.0, 3.0, 3.0}
  obj.rotation = krig.rotation.from_euler({0.3, .4, 0.8})
  obj.always_lit = true
  obj:save()

  camera = krig.get_camera()
  camera:add_particle_system(1)

  obj = krig.level.add_sprite("./scripts/krig3.lua", {
    position = {0.0, 13.0, -60.0},
    scale    = {30.0, 10.0, 1.0}
  })
  obj:save()
end

function on_update(this)
  if krig.test_special_key_pressed(101) == 1 then
    updown = updown + 1
    krig.play_sound(this, "menu.wav")
  end
  if krig.test_special_key_pressed(103) == 1 then
    updown = updown - 1
    krig.play_sound(this, "menu.wav")
  end

  if updown > 1 then updown = 0 end
  if updown < 0 then updown = 1 end
  if updown == 1 then cursor = 0.0 else cursor = -0.2 end

  if krig.test_key_pressed(13) == 1 then
    krig.level.stop_music()

    if cursor == 0.0 then
      krig.level.set_complete(1)
    else
      krig.shutdown()
    end
  end
end

function on_draw_screen(elapsedTime)
  gl.Translate (0.0, 0.0, -2.0)

  gl.Enable("BLEND")

  gl.PushMatrix()
  gl.Rotate(50.0, 0.0, 1.0, 0.0)
  gl.Translate (-0.9, -0.2, 0.0)

  gl.LineWidth(2.0)
  gl.Color(1.0, 0.0, 0.0)
  krig.display_text("Start Game", 0.0, 0.0, 0.0, 0.001, 0.001)
  krig.display_text("Quit", 0.0, -0.2, 0.0, 0.001, 0.001)
  gl.LineWidth(1.0)

  gl.PushMatrix() --Draws Blue translucent rectangle. Cursor for Menu
  gl.Translate(0.0, cursor, 0.0)
  for i = 1, 2 do  --1 for lines, 2 for solid. Makes borders
    if i == 2 then
      gl.Color(0.0, 0.0, 0.0)
      gl.DepthFunc("LEQUAL")
      gl.CullFace("FRONT")
      gl.PolygonMode ("BACK", "LINE")
      gl.LineWidth(10.0)
    else
      gl.Color(0.0, 0.0, 1.0, alph)
    end

    gl.Begin("QUADS")
    gl.Vertex(-0.05, -0.05, 0.05)
    gl.Vertex(0.75, -0.05, 0.05)
    gl.Vertex(0.75, 0.15, 0.05)
    gl.Vertex(-0.05, 0.15, 0.05)

    gl.Vertex(-0.05, 0.15, 0.05)
    gl.Vertex(0.75, 0.15, 0.05)
    gl.Vertex(0.75, 0.15, -0.05)
    gl.Vertex(-0.05, 0.15, -0.05)

    gl.Vertex(-0.05, -0.05, -0.05)
    gl.Vertex(-0.05, -0.05, 0.05)
    gl.Vertex(-0.05, 0.15, 0.05)
    gl.Vertex(-0.05, 0.15, -0.05)

    gl.Vertex(0.75, -0.05, 0.05)
    gl.Vertex(0.75, -0.05, -0.05)
    gl.Vertex(0.75, 0.15, -0.05)
    gl.Vertex(0.75, 0.15, 0.05)

    gl.Vertex(-0.05, -0.05, -0.05)
    gl.Vertex(0.75, -0.05, -0.05)
    gl.Vertex(0.75, -0.05, 0.05)
    gl.Vertex(-0.05, -0.05, 0.05)

    gl.Vertex(0.75, -0.05, -0.05)
    gl.Vertex(-0.05, -0.05, -0.05)
    gl.Vertex(-0.05, 0.15, -0.05)
    gl.Vertex(0.75, 0.15, -0.05)
    gl.End()

    if i == 2 then
      gl.PolygonMode("BACK", "FILL")
      gl.DepthFunc("LESS")
      gl.CullFace("BACK")
      gl.LineWidth(1.0)
    end
  end
  gl.PopMatrix()
  gl.PopMatrix()
  gl.Disable("BLEND")

  if alph >= 0.5 then
    alphaChange = -0.01
  elseif alph <= 0.1 then
    alphaChange = 0.01
  end
  alph = alph + alphaChange

  gl.LineWidth(3.0)
end
