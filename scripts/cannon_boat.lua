dofile('./scripts/sail_boat.lua')

function on_load(this)
    setModel(this, "BoatCannon.mdl")
    setRotation(this, 0.0, -1.57, 0.0)     
    disableCollisionDetection(this)
    setScale(this, 10.0, 10.0, 10.0)
--    setVelocity(this, 10.0, 0.0, 0.0)
    return
end

function on_update(this, elapsedTime)
    orientOnTerrain(this, 0.0, 0.0, 0.0)
    return
end
