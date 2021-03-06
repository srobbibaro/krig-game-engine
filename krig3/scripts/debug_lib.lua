-- Configuration
local paint_all   = 0
local paint_color = 0

local last_x      = 0
local last_z      = 0
local last_type   = 0
local last_red    = 0.0
local last_green  = 0.0
local last_blue   = 0.0
local last_height = 0.0

local mouse_x = nil
local mouse_y = nil

-- Helper Functions
local function get_terrain_coords(x, z)
  x =  math.floor(x / 5.0)
  z = -math.floor(z / 5.0)

  return x, z
end

local function handle_debug_controls(elapsedTime)
  -- handle global user control
  cam_id  = krig.level.get_camera_id()
  camera  = krig.get_camera():load()
  cam_dir = camera.direction
  cam_up  = camera.up
  cam_pos = camera.position
  cam_rot = camera.rotation

  if cam_id == 3 or cam_id == 4 then
    mouse_coords = krig.get_mouse_coordinates()

    mouse_coords[1] = mouse_coords[1] - 400
    mouse_coords[2] = mouse_coords[2] - 300

    if mouse_x == nil then mouse_x = mouse_coords[1] end
    if mouse_y == nil then mouse_y = mouse_coords[2] end

    if mouse_coords[2] ~= mouse_y then
      if math.abs(mouse_y - mouse_coords[2]) < 30.0 then
        rot = krig.rotation.from_euler({((mouse_coords[2] - mouse_y) / 400.0), 0.0, 0.0})
        camera.rotation = krig.rotation.add(cam_rot, rot)
        camera:save()
      end
      mouse_y = mouse_coords[2]
    end

    if mouse_coords[1] ~= mouse_x then
      if math.abs(mouse_x - mouse_coords[1]) < 30.0 then
        rot = krig.rotation.from_euler({0.0, (mouse_coords[1] - mouse_x) / 300.0, 0.0})
        camera.rotation = krig.rotation.add(cam_rot, rot)
        camera:save()
      end
      mouse_x = mouse_coords[1]
    end
  end

  cam_orth = camera.orthogonal

  if krig.test_key_pressed(string.byte("*", 1)) == 1 then
    swapLevel()
  end

  if krig.test_key_pressed(string.byte("q", 1)) == 1 or
     krig.test_key_pressed(string.byte("Q", 1)) == 1 then
    krig.shutdown()
  end

  if krig.test_key_pressed(string.byte("k", 1)) == 1 or
    krig.test_key_pressed(string.byte("K", 1)) == 1 then
    krig.level.load(levels[levelNum])
  end

  if krig.test_key_pressed(string.byte("w", 1)) == 1 or
    krig.test_key_pressed(string.byte("W", 1)) == 1 then
    if cam_id == 4 then
      camera.position = {
        cam_pos[1] + cam_dir[1] * 4.0,
        cam_pos[2] + cam_dir[2] * 4.0,
        cam_pos[3] + cam_dir[3] * 4.0
      }
      camera:save()
    end
    if cam_id == 2 or cam_id == 3 then
      cam_pos[3] = cam_pos[3] - 5.0 -- defalut scale factor (for now)
      camera.position = cam_pos
      camera:save()

      if cam_id == 2 then
        x, z = get_terrain_coords(cam_pos[1], cam_pos[3])

        if paint_all == 1 then
          terrain_setVertexHeight(x, z, last_height)
          terrain_setVertexType(x, z, last_type)
          terrain_setVertexColor(x, z, {last_red, last_green, last_blue})
        elseif paint_color == 1 then
          terrain_setVertexColor(x, z, {last_red, last_green, last_blue})
        end
      end
    end
  end

  if krig.test_key_pressed(string.byte("s", 1)) == 1 or
    krig.test_key_pressed(string.byte("S", 1)) == 1 then
    if cam_id == 4 then
      camera.position = {
        cam_pos[1] - cam_dir[1] * 4.0,
        cam_pos[2] - cam_dir[2] * 4.0,
        cam_pos[3] - cam_dir[3] * 4.0
      }
      camera:save()
    end
    if cam_id == 2 or cam_id == 3 then
      cam_pos[3] = cam_pos[3] + 5.0 -- defalut scale factor (for now)
      camera.position = cam_pos
      camera:save()

      if cam_id == 2 then
        x, z = get_terrain_coords(cam_pos[1], cam_pos[3])
        if paint_all == 1 then
          terrain_setVertexHeight(x, z, last_height)
          terrain_setVertexType(x, z, last_type)
          terrain_setVertexColor(x, z, {last_red, last_green, last_blue})
        elseif paint_color == 1 then
          terrain_setVertexColor(x, z, {last_red, last_green, last_blue})
        end
      end
    end
  end

  if krig.test_key_pressed(string.byte("a", 1)) == 1 or
    krig.test_key_pressed(string.byte("A", 1)) == 1 then
    if cam_id == 4 then
      rot_axis = krig.vector.cross_product(cam_dir, cam_up)
      rot_axis = krig.vector.normalize(rot_axis)

      camera.position = {
        cam_pos[1] - rot_axis[1] * 4.0,
        cam_pos[2] - rot_axis[2] * 4.0,
        cam_pos[3] - rot_axis[3] * 4.0
      }
      camera:save()
    end
    if cam_id == 2 or cam_id == 3 then
      cam_pos[1] = cam_pos[1] - 5.0 -- defalut scale factor (for now)
      camera.position = cam_pos
      camera:save()

      if cam_id == 2 then
        x, z = get_terrain_coords(cam_pos[1], cam_pos[3])

        if paint_all == 1 then
          terrain_setVertexHeight(x, z, last_height)
          terrain_setVertexType(x, z, last_type)
          terrain_setVertexColor(x, z, {last_red, last_green, last_blue})
        elseif paint_color == 1 then
          terrain_setVertexColor(x, z, {last_red, last_green, last_blue})
        end
      end
    end
  end

  if krig.test_key_pressed(string.byte("d", 1)) == 1 or
    krig.test_key_pressed(string.byte("D", 1)) == 1 then
    if cam_id == 4 then
      rot_axis = krig.vector.cross_product(cam_dir, cam_next)
      rot_axis = krig.vector.normalize(rot_axis)

      camera.position = {
        cam_pos[1] + rot_axis[1] * 4.0,
        cam_pos[2] + rot_axis[2] * 4.0,
        cam_pos[3] + rot_axis[3] * 4.0
      }
      camera:save()
    end
    if cam_id == 2 or cam_id == 3 then
      cam_pos[1] = cam_pos[1] + 5.0 -- defalut scale factor (for now)
      camera.position = cam_pos
      camera:save()

      if cam_id == 2 then
        x, z = get_terrain_coords(cam_pos[1], cam_pos[3])

        if paint_all == 1 then
          terrain_setVertexHeight(x, z, last_height)
          terrain_setVertexType(x, z, last_type)
          terrain_setVertexColor(x, z, {last_red, last_green, last_blue})
        elseif paint_color == 1 then
          terrain_setVertexColor(x, z, {last_red, last_green, last_blue})
        end
      end
    end
  end

  if krig.test_key_pressed(string.byte("r", 1)) == 1 or
    krig.test_key_pressed(string.byte("R", 1)) == 1 then
    if cam_id == 4 then
      camera.position = {
        cam_pos[1] + cam_up[1] * 4.0,
        cam_pos[2] + cam_up[2] * 4.0,
        cam_pos[3] - cam_up[3] * 4.0
      }
      camera:save()
    end
    if cam_id == 2 or cam_id == 3 then
      cam_pos[2] = cam_pos[2] + 1.0 -- default scale (for now)
      camera.position = cam_pos
      camera:save()
    end
  end

  if krig.test_key_pressed(string.byte("f", 1)) == 1 or
    krig.test_key_pressed(string.byte("F", 1)) == 1 then
    if cam_id == 4 then
      camera.position = {
        cam_pos[1] - cam_up[1] * 4.0,
        cam_pos[2] - cam_up[2] * 4.0,
        cam_pos[3] + cam_up[3] * 4.0
      }
      camera:save()
    end
    if cam_id == 2 or cam_id == 3 then
      cam_pos[2] = cam_pos[2] - 1.0 -- default scale (for now)
      camera.position = cam_pos
      camera:save()
    end
  end

  if krig.test_key_pressed(string.byte("z", 1)) == 1 or
    krig.test_key_pressed(string.byte("Z", 1)) == 1 then
    if cam_id == 4 then
      camera.position = {0.0, 0.0, -1.0}
      camera.rotation = krig.rotation.from_euler({0.0, 0.0, 0.0})
      camera:save()
    end
  end

  if krig.test_key_pressed(string.byte("u", 1)) == 1 then
    x, z    = get_terrain_coords(cam_pos[1], cam_pos[3])
    terrain_setVertexColor(x, z, {last_red, last_green, last_blue})
  end

  if krig.test_key_pressed(string.byte("U", 1)) == 1 then
    x, z    = get_terrain_coords(cam_pos[1], cam_pos[3])
    terrain_setVertexHeight(x, z, last_height)
    terrain_setVertexType(x, z, last_type)
    terrain_setVertexColor(x, z, {last_red, last_green, last_blue})
  end

  if krig.test_key_pressed(string.byte("p", 1)) == 1 then
    paint_color = paint_color + 1
    if paint_color > 1 then paint_color = 0 end
  end

  if krig.test_key_pressed(string.byte("P", 1)) == 1 then
    paint_all = paint_all + 1
    if paint_all > 1 then paint_all = 0 end
  end

  if krig.test_key_pressed(string.byte("h", 1)) == 1 then
    x, z = get_terrain_coords(cam_pos[1], cam_pos[3])
    height = terrain_getVertexHeight(x, z)
    terrain_setVertexHeight(x, z, height - .05)
  end

  if krig.test_key_pressed(string.byte("H", 1)) == 1 then
    x, z = get_terrain_coords(cam_pos[1], cam_pos[3])
    height = terrain_getVertexHeight(x, z)
    terrain_setVertexHeight(x, z, height + .05)
  end

  if krig.test_key_pressed(string.byte("0", 1)) == 1 then
    x, z = get_terrain_coords(cam_pos[1], cam_pos[3])
    terrain_setVertexHeight(x, z, 0.0)
  end

  if krig.test_key_pressed(string.byte("t", 1)) == 1 or
    krig.test_key_pressed(string.byte("T", 1)) == 1 then
    x, z = get_terrain_coords(cam_pos[1], cam_pos[3])

    type = terrain_getVertexType(x,z)
    type = -type
    terrain_setVertexType(x, z, type)
  end

  light = krig.level.get_light_direction()

  if krig.test_key_pressed(string.byte("[", 1)) == 1 then
    light[1] = light[1] + .1
    light = krig.vector.normalize(light)
  elseif krig.test_key_pressed(string.byte("]", 1)) == 1 then
    light[2] = light[2] + .1
    light = krig.vector.normalize(light)
  elseif krig.test_key_pressed(string.byte("\\", 1)) == 1 then
    light[3] = light[3] + .1
    light = krig.vector.normalize(light)
  elseif krig.test_key_pressed(string.byte("{", 1)) == 1 then
    light[1] = light[1] - .1
    light = krig.vector.normalize(light)
  elseif krig.test_key_pressed(string.byte("}", 1)) == 1 then
    light[2] = light[2] - .1
    light = krig.vector.normalize(light)
  elseif krig.test_key_pressed(string.byte("|", 1)) == 1 then
    light[3] = light[3] - .1
    light = krig.vector.normalize(light)
  end

  krig.level.set_light_direction(light)
end

debug_lib = {
  handle_debug_controls = handle_debug_controls
}

return debug_lib
