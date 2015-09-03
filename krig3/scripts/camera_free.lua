-- Configuration
mouse_x = nil
mouse_y = nil
speed   = 100.0

-- Overridden Engine Callbacks
function on_update(this, elapsedTime)
  camera       = krig.get_camera()
  mouse_coords = krig.get_mouse_coordinates()

  mouse_coords[1] = mouse_coords[1] - 400
  mouse_coords[2] = mouse_coords[2] - 300

  if mouse_x == nil then mouse_x = mouse_coords[1] end
  if mouse_y == nil then mouse_y = mouse_coords[2] end

  if mouse_coords[2] ~= mouse_y then
    if math.abs(mouse_y - mouse_coords[2]) < 30.0 then
      krig.camera.add_rotation(camera, ((mouse_coords[2] - mouse_y) / 400.0 ) , 0.0, 0.0)
    end
    mouse_y = mouse_coords[2]
  end

  if mouse_coords[1] ~= mouse_x then
    if math.abs(mouse_x - mouse_coords[1]) < 30.0 then
      krig.object.add_rotation(camera, 0.0, (mouse_coords[1] - mouse_x) / 300.0, 0.0)
    end
    mouse_x = mouse_coords[1]
  end

  if krig.test_key_pressed(string.byte("w", 1)) == 1 or
     krig.test_key_pressed(string.byte("W", 1)) == 1 then
    krig.object.add_speed(camera, speed, 0.0, 0.0)
  end

  if krig.test_key_released(string.byte("w", 1)) == 1 or
     krig.test_key_released(string.byte("W", 1)) == 1 then
    krig.object.add_speed(camera, -speed, 0.0, 0.0)
  end

  if krig.test_key_pressed(string.byte("s", 1)) == 1 or
     krig.test_key_pressed(string.byte("S", 1)) == 1 then
    krig.object.add_speed(camera, -speed, 0.0, 0.0)
  end

  if krig.test_key_released(string.byte("s", 1)) == 1 or
     krig.test_key_released(string.byte("S", 1)) == 1 then
    krig.object.add_speed(camera, speed, 0.0, 0.0)
  end

  if krig.test_key_pressed(string.byte("a", 1)) == 1 or
     krig.test_key_pressed(string.byte("A", 1)) == 1 then
    krig.object.add_speed(camera, 0.0, 0.0, speed)
  end

  if krig.test_key_released(string.byte("a", 1)) == 1 or
     krig.test_key_released(string.byte("A", 1)) == 1 then
    krig.object.add_speed(camera, 0.0, 0.0, -speed)
  end

  if krig.test_key_pressed(string.byte("d", 1)) == 1 or
     krig.test_key_pressed(string.byte("D", 1)) == 1 then
    krig.object.add_speed(camera, 0.0, 0.0, -speed)
  end

  if krig.test_key_released(string.byte("d", 1)) == 1 or
     krig.test_key_released(string.byte("D", 1)) == 1 then
    krig.object.add_speed(camera, 0.0, 0.0, speed)
  end

  if krig.test_key_pressed(string.byte("r", 1)) == 1 or
     krig.test_key_pressed(string.byte("R", 1)) == 1 then
    krig.object.add_speed(camera, 0.0, speed, 0.0)
  end

  if krig.test_key_released(string.byte("r", 1)) == 1 or
     krig.test_key_released(string.byte("R", 1)) == 1 then
    krig.object.add_speed(camera, 0.0, -speed, 0.0)
  end

  if krig.test_key_pressed(string.byte("f", 1)) == 1 or
     krig.test_key_pressed(string.byte("F", 1)) == 1 then
    krig.object.add_speed(camera, 0.0, -speed, 0.0)
  end

  if krig.test_key_released(string.byte("f", 1)) == 1 or
     krig.test_key_released(string.byte("F", 1)) == 1 then
    krig.object.add_speed(camera, 0.0, speed, 0.0)
  end
end
