function move()
      mx, my, mz = getPosition()
      cx, cy, cz = getCameraPosition()

	tx = cx + 40

      if tx >= mx then
          setVelocity(3.0, -5.0, 0.0)
          setRotationVelocity(3.0, 3.0, 0.0)
	end 

	return
end

