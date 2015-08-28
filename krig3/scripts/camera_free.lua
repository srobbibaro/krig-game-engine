-- Configuration
mouse_x = nil
mouse_y = nil
speed   = 100.0

-- Overridden Engine Callbacks
function on_update(this, elapsedTime)
  camera       = getCamera()
  mouse_coords = engine_getMouseCoordinates()

  mouse_coords[1] = mouse_coords[1] - 400
  mouse_coords[2] = mouse_coords[2] - 300

  if mouse_x == nil then mouse_x = mouse_coords[1] end
  if mouse_y == nil then mouse_y = mouse_coords[2] end

  if mouse_coords[2] ~= mouse_y then
    if math.abs(mouse_y - mouse_coords[2]) < 30.0 then
      camera_addRotation(camera, ((mouse_coords[2] - mouse_y) / 400.0 ) , 0.0, 0.0)
    end
    mouse_y = mouse_coords[2]
  end

  if mouse_coords[1] ~= mouse_x then
    if math.abs(mouse_x - mouse_coords[1]) < 30.0 then
      addRotation(camera, 0.0, (mouse_coords[1] - mouse_x) / 300.0, 0.0)
    end
    mouse_x = mouse_coords[1]
  end

  if engine_testKeyPressed(string.byte("w", 1)) == 1 or
     engine_testKeyPressed(string.byte("W", 1)) == 1 then
    addSpeed(camera, speed, 0.0, 0.0)
  end

  if engine_testKeyReleased(string.byte("w", 1)) == 1 or
     engine_testKeyReleased(string.byte("W", 1)) == 1 then
    addSpeed(camera, -speed, 0.0, 0.0)
  end

  if engine_testKeyPressed(string.byte("s", 1)) == 1 or
     engine_testKeyPressed(string.byte("S", 1)) == 1 then
    addSpeed(camera, -speed, 0.0, 0.0)
  end

  if engine_testKeyReleased(string.byte("s", 1)) == 1 or
     engine_testKeyReleased(string.byte("S", 1)) == 1 then
    addSpeed(camera, speed, 0.0, 0.0)
  end

  if engine_testKeyPressed(string.byte("a", 1)) == 1 or
     engine_testKeyPressed(string.byte("A", 1)) == 1 then
    addSpeed(camera, 0.0, 0.0, speed)
  end

  if engine_testKeyReleased(string.byte("a", 1)) == 1 or
     engine_testKeyReleased(string.byte("A", 1)) == 1 then
    addSpeed(camera, 0.0, 0.0, -speed)
  end

  if engine_testKeyPressed(string.byte("d", 1)) == 1 or
     engine_testKeyPressed(string.byte("D", 1)) == 1 then
    addSpeed(camera, 0.0, 0.0, -speed)
  end

  if engine_testKeyReleased(string.byte("d", 1)) == 1 or
     engine_testKeyReleased(string.byte("D", 1)) == 1 then
    addSpeed(camera, 0.0, 0.0, speed)
  end

  if engine_testKeyPressed(string.byte("r", 1)) == 1 or
     engine_testKeyPressed(string.byte("R", 1)) == 1 then
    addSpeed(camera, 0.0, speed, 0.0)
  end

  if engine_testKeyReleased(string.byte("r", 1)) == 1 or
     engine_testKeyReleased(string.byte("R", 1)) == 1 then
    addSpeed(camera, 0.0, -speed, 0.0)
  end

  if engine_testKeyPressed(string.byte("f", 1)) == 1 or
     engine_testKeyPressed(string.byte("F", 1)) == 1 then
    addSpeed(camera, 0.0, -speed, 0.0)
  end

  if engine_testKeyReleased(string.byte("f", 1)) == 1 or
     engine_testKeyReleased(string.byte("F", 1)) == 1 then
    addSpeed(camera, 0.0, speed, 0.0)
  end
end
