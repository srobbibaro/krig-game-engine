numObjects = 2
terrain = "level2.txt"

weatherEffect = 1

lightDirection = {0.25, 0.25, 0.5}

bgMusic = "Woodman.ogg"

bgColor = {0.0, 0.0, 0.3,
           0.6, 0.6, 0.6,
	     1.0, 1.0, 1.0}

x_start_camera = 50.0
x_start_player = x_start_camera - 10.0
x_start_boss   = x_start_camera + 10.0

cameraPosition = {x_start_camera, 15.0, 27.5}
cameraRotation = {0.0, 0.0, 0.0}
cameraScript   = "enemy_ship1.lua"

objects = {
      -- Setup the 1st object (always player) 
	{type 	= 0,
       modelKey	= "Ship.mdl",
	 scale      = {2.0, 2.0, 2.0},
       rotation   = {0.0, 1.57, 0.0},
       position   = {x_start_player, 20.0, 0.0},
       script     = "enemy_ship1.lua"},

      -- Setup the 2nd object (always boss)
	{type		= 6,
       modelKey   = "snowboss.mdl",
       scale      = {2.0, 2.0, 2.0},
       rotation   = {0.0, -1.57, 0.0},
       position   = {x_start_boss, 15.0, 0.0},
       script     = "boss2.lua"},
}
