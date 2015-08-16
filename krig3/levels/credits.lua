dofile("./levels/level_lib.lua")

-- Configuration
creditsPos     = 0.0
credTrans      = 0.0

x_start_camera = 160.0
x_start_player = x_start_camera

creds = { "Head Programmer", "Steven Robbibaro",
          "Assistant Programmer", "Andrew Yochum",
          "Terrain Generation/Design", "Steven Robbibaro",
          "Model Design", "Andrew Yochum",
          "Object Behavior", "Steven Robbibaro",
          "Sound", "Andrew Yochum",
          "Camera Behavior", "Steven Robbibaro",
          "Menu Design", "Andrew Yochum",
          "Data Storage", "Steven Robbibaro", "Andrew Yochum",
          "Game Logic", "Steven Robbibaro", "Andrew Yochum",
          "Controls", "Steven Robbibaro", "Andrew Yochum",
          "Sound Editing", "Kelly McMeans",
          "Music by", "The NESkimos", "http://www.neskimos.com", "Wily's Theme - Stage 1", "MegaMan2 - Intro", "MegaMan2 - Ending",
          "Special Thanks", "NeHe Productions:", "http://nehe.gamedev.net", "http://www.openal.org", "Capcom",
          "THE END",
          "Press Escape to Continue" }

credCoord = { { -0.2   , 0.0  }, { -0.22, -0.1 },
              { -0.25  , -0.4 }, { -0.2 , -0.5 },
              { -0.3   , -0.8 }, { -0.22, -0.9 },
              { -0.17  , -1.2 }, { -0.2 , -1.3 },
              { -0.2   , -1.6 }, { -0.22, -1.7 },
              { -0.08  , -2.0 }, { -0.2 , -2.1 },
              { -0.2   , -2.4 }, { -0.22, -2.5 },
              { -0.15  , -2.8 }, { -0.2 , -2.9 },
              { -0.165 , -3.2 }, { -0.22, -3.3 }, { -0.2 , -3.4 },
              { -0.142 , -3.6 }, { -0.22, -3.7 }, { -0.2 , -3.8 },
              { -0.12  , -4.0 }, { -0.22, -4.1 }, { -0.2 , -4.2 },
              { -0.18  , -4.4 }, { -0.18, -4.5 },
              { -0.12  , -4.8 }, { -0.17, -4.9 }, { -0.29 , -5.0 }, { -0.26 , -5.1 }, { -0.26 , -5.15 }, { -0.26 , -5.2 },
              { -0.2   , -5.4 }, { -0.23, -5.5 }, { -0.29 , -5.55}, { -0.27 , -5.7 }, { -0.12 , -5.85 },
              { -0.16  , -7.0 },
              { -0.37  , -7.8 } }

-- Overriden Engine Callbacks
function on_draw_screen(terrain, elapsedTime)
  display_debug()

  delay = elapsedTime

  if creditsPos >= 7.0 then
    CredTrans = 0.0
  else
    CredTrans = 0.1
  end

  creditsPos = creditsPos + (CredTrans * delay)

  gl.PushMatrix()
  gl.Translate (0.0, creditsPos, -2.0)

  for i = 1,40 do
    str = creds[i]

    if i == 0 or i == 2 or i == 4 or i == 6 or i == 8 or i == 10 or i == 12 or i == 14 or
      i == 16 or i == 19 or i == 22 or i == 25 or i == 27 or i == 33 then

      gl.Color (1.0, 1.0, 1.0)
      renderText(str, credCoord[i][1], credCoord[i][2])
    elseif i == 38 or i == 39 then
      gl.Color (1.0, 1.0, 1.0)
      renderText(str, credCoord[i][1], credCoord[i][2])
    else
      gl.Color (1.0, 0.0, 0.0)
      renderText(str, credCoord[i][1], credCoord[i][2])
    end
  end
  gl.PopMatrix()
end

function on_load(terrain)
  setSkyBox(
    0.0, 0.0, 0.0,
    0.0, 0.0, 0.3,
    1.0, 0.0, 0.5
  )

  setLightDirection(0.96, 0.07, 0.13)

  setTerrain(terrain, "./terrains/level4.txt")

  camera = getCamera()
  setScript(camera, "./scripts/camera_free.lua")
  setPosition(camera, x_start_camera, 55.0, 105.0)

  addParticleSystem(camera, 1)

  playBgMusic("./music/credits.ogg", 0)
end

function on_update(this)
  if engine_testKeyPressed(27) == 1 then
    playSound(this, "pause.wav")
    -- TODO: Title screen/menu should restart game rather than returning to
    --       the credits.
  end
end

function on_draw()
  player = getPlayer()
  plr_pos = getPosition(player)

  for i=1,4 do
    gl.PushMatrix()
    gl.Translate(plr_pos[1], plr_pos[2], plr_pos[3] - (25.0 * i))
    gl.Color(0.0, 0.4, 0.7)
    gl.Begin("LINES")
    gl.Vertex(-1.0, -1.0, -1.0)
    gl.Vertex(1.0, -1.0, -1.0)

    gl.Vertex(-1.0, 1.0, -1.0)
    gl.Vertex(1.0, 1.0, -1.0)

    gl.Vertex(-1.0, -1.0, -1.0)
    gl.Vertex(-1.0, 1.0, -1.0)

    gl.Vertex(1.0, -1.0, -1.0)
    gl.Vertex(1.0, 1.0, -1.0)
    gl.End()
    gl.PopMatrix()
  end
end

function on_unload() end
