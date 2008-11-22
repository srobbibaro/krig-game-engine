stopped = 0

function on_load(this)
    setVelocity(this, 10.0, 0.0, 0.0)
    return
end

function on_update(this)
      if stopped == 0 then 
          x,y,z = getPosition(this);

          if x >= 850.0 then
	        stopped = 1
              setVelocity(this, 0.0, 0.0, 0.0)
          end
	end

	return
end

function on_unload(this)
    return
end
