function move(this)
      x,y,z = getPosition(this);

      if x < 850.0 then
          setVelocity(this, 10.0, 0.0, 0.0)
      else
          setVelocity(this, 0.0, 0.0, 0.0)
      end

	return
end

