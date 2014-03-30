cursor      = -0.2
alph        = 0.1
alphaChange = 0.01
updown      = 0

function on_load()
    playBgMusic("./music/menu.ogg", 0)
    return
end

function on_unload()
    return
end

function on_update(this)
    if engine_testSpecialKeyPressed(101) == 1 then 
        updown = updown + 1 
        playSound(this, "menu.wav")
    end
    if engine_testSpecialKeyPressed(103) == 1 then 
        updown = updown - 1 
        playSound(this, "menu.wav")
    end

    if updown > 1 then
        updown = 0
    end

    if updown < 0 then
        updown = 1
    end

    if updown == 1 then
        cursor = 0.0
    else
        cursor = -0.2
    end
    
    if engine_testKeyPressed(13) == 1 then
        stopBgMusic()

        if cursor == 0.0 then
            setComplete(1)
        else
            shutdown()
        end
    end
    
    return
end

function on_draw()
    return
end

function on_draw_screen(elapsedTime)
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
    displayText("KRIG", -0.69, 0.5, -0.01, 0.005, 0.003) 

    gl.LineWidth(3.0)
    gl.Color(0.0, 0.0, 1.0)
    displayText("KRIG", -0.69, 0.5, -0.01, 0.005, 0.003)
    gl.PushMatrix()
        gl.Rotate(50.0, 0.0, 1.0, 0.0)
        gl.Translate (-0.9, -0.2, 0.0)
        
        gl.LineWidth(2.0)
        gl.Color(1.0, 0.0, 0.0)
        displayText("Start Game", 0.0, 0.0, 0.0, 0.001, 0.001)
        displayText("Quit", 0.0, -0.2, 0.0, 0.001, 0.001)
        gl.LineWidth(1.0)

        gl.PushMatrix() --Draws Blue translucent rectangle. Cursor for Menu
            gl.Translate(0.0, cursor, 0.0)
            for i=1,2 do  --1 for lines, 2 for solid. Makes borders
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
   
    gl.PushMatrix()     --Displays raster text for the game instructions
        gl.Translate(0.0, -0.4, 0.0)
        gl.Color(1.0, 0.0, 0.0)
        renderText("Controls:", 0.0, 0.2)
        --render_string(GLUT_BITMAP_9_BY_15, str);

        gl.Color(0.0, 0.0, 0.0)
        renderText("Arrow Keys      : Move Ship/Menu", 0.1, 0.15)
        renderText("Enter           : Choose Option", 0.1, 0.1)
        renderText("Spacebar        : Fire Weapon", 0.1, 0.05)
        renderText("Escape          : Pause Menu", 0.1, 0.0)
        renderText("F1, F2, F3      : Lower Detail Level", 0.1, -0.05)

        gl.Color(1.0, 0.0, 0.0)
        renderText("Game Description:", 0.0, -0.1)

        gl.Color(0.0, 0.0, 0.0)
        renderText("An alien army, the Krig, has come", 0.1, -0.15)
        renderText("to destroy the planet! Shoot down", 0.1, -0.2)
        renderText("their fighters and avoid asteroids", 0.1, -0.25)
        renderText("to stop the full scale invasion", 0.1, -0.3)
        renderText("of the Krig!!!", 0.1, -0.35)
        
        renderText("v 3.0", 0.6, .87)
        --renderText(GLUT_BITMAP_HELVETICA_18, str)
        
        gl.Color(1.0, 0.0, 0.0)
        renderText("HighScore: 1000250", -0.25, .8)
        
    gl.PopMatrix()
    
    if alph >= 0.5 then
        alphaChange = -0.01
    elseif alph <= 0.1 then
        alphaChange = 0.01
    end
    alph = alph + alphaChange

    gl.LineWidth(3.0)
    
    return
end
