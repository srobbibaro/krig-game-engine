function move()
      mx, my, mz = getPosition()
      cx, cy, cz = getCameraPosition()

	tx = cx + 30

      if tx >= mx then
          setVelocity(0.0, -5.0, 0.0)
          setRotationVelocity(3.0, 0.0, 3.0)
	end 

	return
end

