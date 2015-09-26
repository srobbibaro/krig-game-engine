local level_lib = require './levels/level_lib'

-- Configuration
creditsPos     = 0.0
credTrans      = 0.0

x_start_camera = 160.0
x_start_player = x_start_camera

creds = {
  {"KRIG 3", "Krig Game Engine"},
  {"Game Programming", "Steven Robbibaro", "Andrew Yochum"},
  {"Model Design", "Andrew Yochum"},
  {"Terrain Generation/Design", "Steven Robbibaro"},
  {"Sound Editing", "Kelly McMeans"},
  {"Music by", "The NESkimos", "http://www.neskimos.com"},
  {"Thank you for playing!"},
  {"THE END"},
  {"Press Escape to Continue"}
}

credCoord = {
  {{-0.09 , -0.2} , {-0.20, -0.3 }},
  {{-0.23 , -0.7} , {-0.22, -0.8 }, { -0.2 , -0.9 }},
  {{-0.17 , -1.3} , {-0.2 , -1.4 }},
  {{-0.3  , -1.8} , {-0.22, -1.9 }},
  {{-0.18 , -2.3} , {-0.18, -2.4 }},
  {{-0.12 , -2.8} , {-0.17, -2.9 }, { -0.29, -3.0 }},
  {{-0.25 , -4.5}},
  {{-0.10 , -7.0}},
  {{-0.28 , -7.8}}
}

-- Overriden Engine Callbacks
function on_draw_screen(terrain, elapsedTime)
  level_lib.display_debug()

  local delay = elapsedTime

  if creditsPos >= 7.0 then
    CredTrans = 0.0
  else
    CredTrans = 0.1
  end

  creditsPos = creditsPos + (CredTrans * delay)

  gl.PushMatrix()
  gl.Translate(0.0, creditsPos, -2.0)

  for i = 1, #creds do
    for u = 1, #creds[i] do
      if u == 1 then
        gl.Color(1.0, 1.0, 1.0)
      else
        gl.Color(1.0, 0.0, 0.0)
      end
      krig.render_text(creds[i][u], credCoord[i][u][1], credCoord[i][u][2])
    end
  end
  gl.PopMatrix()
end

function on_load(terrain)
  krig.level.set_sky_box(
    0.0, 0.0, 0.0,
    0.0, 0.0, 0.3,
    1.0, 0.0, 0.5
  )

  krig.level.set_light_direction(0.96, 0.07, 0.13)

  krig.level.set_terrain(terrain, "./terrains/level4.txt")

  camera = krig.get_camera()
  camera:set_script("./scripts/camera_free.lua")
  camera.position = {x_start_camera, 55.0, 105.0}
  camera:save()

  camera:add_particle_system(1)

  krig.level.play_music("./music/credits.ogg", 0)
end

function on_update(this)
  if krig.test_key_pressed(27) == 1 then
    playSound(this, "pause.wav")
    -- TODO: Title screen/menu should restart game rather than returning to
    --       the credits.
  end

  light = krig.level.get_light_direction()

  if krig.test_key_pressed(string.byte("[", 1)) == 1 then
    light[1] = light[1] + .1
    light = vector_normalize(light)
  elseif krig.test_key_pressed(string.byte("]", 1)) == 1 then
    light[2] = light[2] + .1
    light = vector_normalize(light)
  elseif krig.test_key_pressed(string.byte("\\", 1)) == 1 then
    light[3] = light[3] + .1
    light = vector_normalize(light)
  elseif krig.test_key_pressed(string.byte("{", 1)) == 1 then
    light[1] = light[1] - .1
    light = vector_normalize(light)
  elseif krig.test_key_pressed(string.byte("}", 1)) == 1 then
    light[2] = light[2] - .1
    light = vector_normalize(light)
  elseif krig.test_key_pressed(string.byte("|", 1)) == 1 then
    light[3] = light[3] - .1
    light = vector_normalize(light)
  end

  krig.level.set_light_direction(light)
end
