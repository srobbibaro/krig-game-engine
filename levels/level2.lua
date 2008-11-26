numObjects = 2
terrain = "level2.txt"

terrainColor = {0.6, 0.4, 0.0,
                0.0, 1.0, 0.0,
                0.0, 0.0, 0.0}

weatherEffect = 0

songNum = 1

bgColor = {0.0, 0.0, 0.3,
           0.4, 0.4, 1.0,
	     0.8, 0.8, 1.0}

x_start_camera = 50.0
x_start_player = x_start_camera - 20.0

cameraPosition = {x_start_camera, 15.0, 27.5}
cameraRotation = {0.0, 0.0, 0.0}
cameraScript   = "camera1.lua"

objects = {
      -- Setup the 1st object (always player) 
	{type 	= 0,
       modelKey	= "Ship.mdl",
	 scale      = {2.0, 2.0, 2.0},
       rotation   = {0.0, 1.57, 0.0},
       position   = {x_start_player, 20.0, 0.0},
       script     = "camera1.lua"},

      -- Setup the 2nd object (always boss)
	{type		= 6,
       modelKey   = "Boss.mdl",
       scale      = {4.0, 4.0, 4.0},
       rotation   = {0.0, -1.57, 0.0},
       position   = {860.0, 15.0, 50.0},
       script     = "boss1.lua"},
}
