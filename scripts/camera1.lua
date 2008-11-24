totalTime = 0
stopped = 0

function on_load(this)
    setVelocity(this, 10.0, 0.0, 0.0)
    return
end

function on_update(this, elapsedTime)
     -- player = getPlayer()
--	prx, pry, prz = getRotation(player)
--	setRotation(this, prx, pry, prz)
--	px, py, pz = getPosition(player)
--	setPosition(this, px, py, pz - 15.0)
--totalTime = totalTime + elapsedTime
--vx = math.sin(totalTime) * 100.0
--setVelocity(this, vx, 0.0, 0.0)

	--if 1 == 0 then 
      --    x,y,z = getPosition(this, elapsedTime);

--          if x >= 950.0 then
--	        stopped = 1
--              setVelocity(this, 0.0, 0.0, 0.0)
--          end
--	end

	return
end

function on_unload(this)
    return
end
