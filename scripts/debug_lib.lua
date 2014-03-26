paint_all = 0
paint_color = 0

last_x      = 0
last_z      = 0
last_type   = 0
last_red    = 0.0
last_green  = 0.0
last_blue   = 0.0
last_height = 0.0

mouse_x = nil
mouse_y = nil

function handle_debug_controls(elapsedTime)
    -- handle global user control
    cam_id = getCameraId()
    camera = getCamera()
    player = getPlayer()
    cam_dir = getDirection(camera)

    if cam_id == 3 or cam_id == 4 then
        mouse_coords = engine_getMouseCoordinates()

        mouse_coords[1] = mouse_coords[1] - 400
        mouse_coords[2] = mouse_coords[2] - 300

        if mouse_x == nil then mouse_x = mouse_coords[1] end
        if mouse_y == nil then mouse_y = mouse_coords[2] end

        if mouse_coords[2] ~= mouse_y then
            if math.abs(mouse_y - mouse_coords[2]) < 30.0 then
--                camera_addRotation(camera, ((mouse_coords[2] - mouse_y) / 400.0 * .75 ) * -cam_orth[1], 0.0, ((mouse_coords[2] - mouse_y) / 400.0 * .75)* -cam_orth[3] )
                camera_addRotation(camera, ((mouse_coords[2] - mouse_y) / 400.0 ) , 0.0, 0.0)
                --addRotation(camera, mouse_coords[2] / 300.0 * .0075, 0.0, 0.0)
            end
            mouse_y = mouse_coords[2]
        end

        if mouse_coords[1] ~= mouse_x then
            if math.abs(mouse_x - mouse_coords[1]) < 30.0 then
                --addRotation(camera, 0.0, (mouse_coords[1] - mouse_x) / 300.0 * .75, 0.0)
                addRotation(camera, 0.0, (mouse_coords[1] - mouse_x) / 300.0, 0.0)
                --addRotation(camera, 0.0, mouse_coords[1] / 400.0 * 0.0075, 0.0)
            end
            mouse_x = mouse_coords[1]
        end
    end

    cam_orth = getOrthogonal(camera)

    if engine_testKeyPressed(string.byte("*", 1)) == 1 then
        swapLevel()
    end

    if engine_testKeyPressed(string.byte("q", 1)) == 1 or 
       engine_testKeyPressed(string.byte("Q", 1)) == 1 then 
       shutdown()
    end

    if engine_testKeyPressed(string.byte("k", 1)) == 1 or 
       engine_testKeyPressed(string.byte("K", 1)) == 1 then
        loadLevel(levels[levelNum])
    end

    if engine_testKeyPressed(string.byte("w", 1)) == 1 or 
       engine_testKeyPressed(string.byte("W", 1)) == 1 then
        if cam_id == 4 then
            cam_pos = getPosition(camera)
            cam_dir = getDirection(camera)
            
            setPosition(camera, cam_pos[1] + cam_dir[1] * 4.0,
                        cam_pos[2] + cam_dir[2] * 4.0,
                        cam_pos[3] + cam_dir[3] * 4.0)
        end
        if cam_id == 2 or cam_id == 3 then
            cam_pos = getPosition(camera)
            cam_pos[3] = cam_pos[3] - 5.0 -- defalut scale factor (for now)
            setPositionv(camera, cam_pos)

            if cam_id == 2 then
                --c3->position.x = c2->position.x;
                --c3->position.z = c2->position.z;
                x, z = get_terrain_coords(cam_pos[1], cam_pos[3])

                if paint_all == 1 then
                    terrain_setVertexHeight(x, z, last_height)
                    terrain_setVertexType(x, z, last_type)
                    terrain_setVertexColor(x, z, {last_red, last_green, last_blue})
                elseif paint_color == 1 then
                    terrain_setVertexColor(x, z, {last_red, last_green, last_blue})
                end
            else
                --c2->position.x = c3->position.x;
                --c2->position.z = c3->position.z;
            end
        end
    end

    if engine_testKeyPressed(string.byte("s", 1)) == 1 or 
       engine_testKeyPressed(string.byte("S", 1)) == 1 then
        if cam_id == 4 then
            cam_pos = getPosition(camera)
            cam_dir = getDirection(camera)
            
            setPosition(camera, cam_pos[1] - cam_dir[1] * 4.0,
                        cam_pos[2] - cam_dir[2] * 4.0,
                        cam_pos[3] - cam_dir[3] * 4.0)
        end
        if cam_id == 2 or cam_id == 3 then
            cam_pos = getPosition(camera)
            cam_pos[3] = cam_pos[3] + 5.0 -- defalut scale factor (for now)
            setPositionv(camera, cam_pos)

            if cam_id == 2 then
                --c3->position.x = c2->position.x;
                --c3->position.z = c2->position.z;

                x, z = get_terrain_coords(cam_pos[1], cam_pos[3])
                if paint_all == 1 then
                    terrain_setVertexHeight(x, z, last_height)
                    terrain_setVertexType(x, z, last_type)
                    terrain_setVertexColor(x, z, {last_red, last_green, last_blue})
                elseif paint_color == 1 then
                    terrain_setVertexColor(x, z, {last_red, last_green, last_blue})
                end
            else
                --c2->position.x = c3->position.x;
                --c2->position.z = c3->position.z;
            end
        end
    end

    if engine_testKeyPressed(string.byte("a", 1)) == 1 or 
       engine_testKeyPressed(string.byte("A", 1)) == 1 then
        if cam_id == 4 then
            cam_up = getUp(camera)
            cam_dir = getDirection(camera)

            rot_axis = vector_crossProduct(cam_up, cam_dir)
            cam_pos = getPosition(camera)
            cam_dir = getDirection(camera)
            rot_axis = vector_normalize(rot_axis)
            
            setPosition(camera, cam_pos[1] - rot_axis[1] * 4.0,
                cam_pos[2] - rot_axis[2] * 4.0,
                cam_pos[3] - rot_axis[3] * 4.0)
        end
        if cam_id == 2 or cam_id == 3 then
            cam_pos = getPosition(camera)
            cam_pos[1] = cam_pos[1] - 5.0 -- defalut scale factor (for now)
            setPositionv(camera, cam_pos)

            if cam_id == 2 then
                --c3->position.x = c2->position.x;
                --c3->position.z = c2->position.z;

                x, z = get_terrain_coords(cam_pos[1], cam_pos[3])

                if paint_all == 1 then
                    terrain_setVertexHeight(x, z, last_height)
                    terrain_setVertexType(x, z, last_type)
                    terrain_setVertexColor(x, z, {last_red, last_green, last_blue})
                elseif paint_color == 1 then
                    terrain_setVertexColor(x, z, {last_red, last_green, last_blue})
                end
            else
                --c2->position.x = c3->position.x;
                --c2->position.z = c3->position.z;
            end
        end
    end

    if engine_testKeyPressed(string.byte("d", 1)) == 1 or 
       engine_testKeyPressed(string.byte("D", 1)) == 1 then
        if cam_id == 4 then
            cam_up = getUp(camera)
            cam_dir = getDirection(camera)

            rot_axis = vector_crossProduct(cam_up, cam_dir)
            cam_pos = getPosition(camera)
            cam_dir = getDirection(camera)
            rot_axis = vector_normalize(rot_axis)
            
            setPosition(camera, cam_pos[1] + rot_axis[1] * 4.0,
                cam_pos[2] + rot_axis[2] * 4.0,
                cam_pos[3] + rot_axis[3] * 4.0)
        end
        if cam_id == 2 or cam_id == 3 then
            cam_pos = getPosition(camera)
            cam_pos[1] = cam_pos[1] + 5.0 -- defalut scale factor (for now)
            setPositionv(camera, cam_pos)

            if cam_id == 2 then
                --c3->position.x = c2->position.x;
                --c3->position.z = c2->position.z;

                x, z = get_terrain_coords(cam_pos[1], cam_pos[3])

                if paint_all == 1 then
                    terrain_setVertexHeight(x, z, last_height)
                    terrain_setVertexType(x, z, last_type)
                    terrain_setVertexColor(x, z, {last_red, last_green, last_blue})
                elseif paint_color == 1 then
                    terrain_setVertexColor(x, z, {last_red, last_green, last_blue})
                end
            else
                --c2->position.x = c3->position.x;
                --c2->position.z = c3->position.z;
            end
        end
    end

    if engine_testKeyPressed(string.byte("r", 1)) == 1 or 
       engine_testKeyPressed(string.byte("R", 1)) == 1 then
        if cam_id == 4 then
            cam_pos = getPosition(camera)
            cam_up = getUp(camera)
            
            setPosition(camera, cam_pos[1] + cam_up[1] * 4.0,
                        cam_pos[2] + cam_up[2] * 4.0,
                        cam_pos[3] - cam_up[3] * 4.0)
        end
        if cam_id == 2 or cam_id == 3 then
            cam_pos = getPosition(camera)
            cam_pos[2] = cam_pos[2] + 1.0 -- default scale (for now)
            setPositionv(camera, cam_pos)
        end
    end

    if engine_testKeyPressed(string.byte("f", 1)) == 1 or 
       engine_testKeyPressed(string.byte("F", 1)) == 1 then
        if cam_id == 4 then
            cam_pos = getPosition(camera)
            cam_up = getUp(camera)

            setPosition(camera, cam_pos[1] - cam_up[1] * 4.0,
                        cam_pos[2] - cam_up[2] * 4.0,
                        cam_pos[3] + cam_up[3] * 4.0)
        end
        if cam_id == 2 or cam_id == 3 then
            cam_pos = getPosition(camera)
            cam_pos[2] = cam_pos[2] - 1.0 -- default scale (for now)
            setPositionv(camera, cam_pos)
        end
    end

    if engine_testKeyPressed(string.byte("z", 1)) == 1 or 
       engine_testKeyPressed(string.byte("Z", 1)) == 1 then
        if cam_id == 4 then
            setPosition(camera, 0.0, 0.0, -1.0)
            --setUp(camera, 0.0, 1.0, 0.0        
            setRotation(camera, 0.0, 0.0, 0.0)
        end
    end

    if engine_testKeyPressed(string.byte("u", 1)) == 1 then
        cam_pos = getPosition(camera)
        x, z = get_terrain_coords(cam_pos[1], cam_pos[3])
        terrain_setVertexColor(x, z, {last_red, last_green, last_blue})
    end

    if engine_testKeyPressed(string.byte("U", 1)) == 1 then
        cam_pos = getPosition(camera)
        x, z = get_terrain_coords(cam_pos[1], cam_pos[3])
        terrain_setVertexHeight(x, z, last_height)
        terrain_setVertexType(x, z, last_type)
        terrain_setVertexColor(x, z, {last_red, last_green, last_blue})
    end

    if engine_testKeyPressed(string.byte("p", 1)) == 1 then
        paint_color = paint_color + 1
        if paint_color > 1 then paint_color = 0 end
    end

    if engine_testKeyPressed(string.byte("P", 1)) == 1 then
        paint_all = paint_all + 1
        if paint_all > 1 then paint_all = 0 end
    end

    if engine_testKeyPressed(string.byte("h", 1)) == 1 then
        cam_pos = getPosition(camera)
        x, z = get_terrain_coords(cam_pos[1], cam_pos[3])
        height = terrain_getVertexHeight(x, z)
        terrain_setVertexHeight(x, z, height - .05) 
    end

    if engine_testKeyPressed(string.byte("H", 1)) == 1 then
        cam_pos = getPosition(camera)
        x, z = get_terrain_coords(cam_pos[1], cam_pos[3])
        height = terrain_getVertexHeight(x, z)
        terrain_setVertexHeight(x, z, height + .05)
    end

    if engine_testKeyPressed(string.byte("0", 1)) == 1 then
        cam_pos = getPosition(camera)
        x, z = get_terrain_coords(cam_pos[1], cam_pos[3])
        terrain_setVertexHeight(x, z, 0.0)
    end

    if engine_testKeyPressed(string.byte("t", 1)) == 1 or 
       engine_testKeyPressed(string.byte("T", 1)) == 1 then
        cam_pos = getPosition(camera)
        x, z = get_terrain_coords(cam_pos[1], cam_pos[3])

        type = terrain_getVertexType(x,z)
        type = -type
        terrain_setVertexType(x, z, type)
    end

    light = getLightDirection() 

    if engine_testKeyPressed(string.byte("[", 1)) == 1 then 
        light[1] = light[1] + .1 
        light = vector_normalize(light) 
    elseif engine_testKeyPressed(string.byte("]", 1)) == 1 then 
        light[2] = light[2] + .1 
        light = vector_normalize(light) 
    elseif engine_testKeyPressed(string.byte("\\", 1)) == 1 then 
        light[3] = light[3] + .1 
        light = vector_normalize(light) 
    elseif engine_testKeyPressed(string.byte("{", 1)) == 1 then 
        light[1] = light[1] - .1 
        light = vector_normalize(light) 
    elseif engine_testKeyPressed(string.byte("}", 1)) == 1 then 
        light[2] = light[2] - .1 
        light = vector_normalize(light) 
    elseif engine_testKeyPressed(string.byte("|", 1)) == 1 then 
        light[3] = light[3] - .1 
        light = vector_normalize(light) 
    end 

    setLightDirectionv(light) 

    return
end

function get_terrain_coords(x,z)
    x = math.floor(x / 5.0)
    z = -math.floor(z / 5.0)

    return x,z    
end
