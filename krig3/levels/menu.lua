-- Configuration
local cursor      = -0.2
local alph        = 0.1
local alphaChange = 0.01
local updown      = 0

-- Overridden Engine Callbacks
function on_load(this)
  krig.level.play_music("./music/menu.ogg", 0)
end

function on_update(this, elapsed_time)
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

function on_draw_screen(this, elapsedTime)
  gl.Translate (0.0, 0.0, -2.0)

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

  gl.Enable("BLEND")

  gl.LineWidth(15.0)
  gl.Color(0.0, 0.0, 0.0)
  krig.display_text("KRIG", -0.69, 0.5, -0.01, 0.005, 0.003)

  gl.LineWidth(3.0)
  gl.Color(0.0, 0.0, 1.0)
  krig.display_text("KRIG", -0.69, 0.5, -0.01, 0.005, 0.003)
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

  -- Displays text for the game instructions
  gl.PushMatrix()
  gl.Translate(0.0, -0.4, 0.0)
  gl.Color(1.0, 0.0, 0.0)
  krig.render_text("Controls:", 0.0, 0.2)

  gl.Color(0.0, 0.0, 0.0)
  krig.render_text("Arrow Keys", 0.1, 0.15)
  krig.render_text("Enter", 0.1, 0.1)
  krig.render_text("Spacebar", 0.1, 0.05)
  krig.render_text("m", 0.1, 0.0)
  krig.render_text("Escape", 0.1, -0.05)

  krig.render_text(": Move Ship/Menu", 0.4, 0.15)
  krig.render_text(": Choose Option", 0.4, 0.1)
  krig.render_text(": Fire Weapon", 0.4, 0.05)
  krig.render_text(": Fire Missile", 0.4, 0.0)
  krig.render_text(": Pause Menu", 0.4, -0.05)

  gl.Color(1.0, 0.0, 0.0)
  krig.render_text("Game Description:", 0.0, -0.1)

  gl.Color(0.0, 0.0, 0.0)
  krig.render_text("An alien army, the Krig, has come", 0.1, -0.15)
  krig.render_text("to destroy the planet! Shoot down", 0.1, -0.2)
  krig.render_text("their fighters and avoid asteroids", 0.1, -0.25)
  krig.render_text("to stop the full scale invasion", 0.1, -0.3)
  krig.render_text("of the Krig!!!", 0.1, -0.35)

  krig.render_text("v 3.0", 0.6, .87)

  -- TODO: Use a real highscore
  gl.Color(1.0, 0.0, 0.0)
  krig.render_text("HighScore: 1000250", -0.25, .8)

  gl.PopMatrix()

  if alph >= 0.5 then
    alphaChange = -0.01
  elseif alph <= 0.1 then
    alphaChange = 0.01
  end
  alph = alph + alphaChange

  gl.LineWidth(3.0)
end
