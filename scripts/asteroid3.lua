function move()
      mx, my, mz = getPosition()
      cx, cy, cz = getCameraPosition()

      if cx >= mx then
          setVelocity(3.0, -7.0, 0.0)
          setRotationVelocity(0.0, 3.0, 3.0)
	end 

	return
end

