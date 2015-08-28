local shooting_object = require 'scripts/shooting_object'

-- Configuration
progress        = 0
life            = 10
lives           = 3
numMissiles     = 0
nextMissileShot = 0.0

leftDown        = 0
rightDown       = 0
upDown          = 0
downDown        = 0

invul            = 0.0
collisionRecover = 0
interpTime       = 0.0

-- Overridden Engine Callbacks
function on_load(this)
  setModel(this, "Ship.mdl")

  setRotation(this, 0.0, 1.57, 0.0)
  setScale(this, 2.0, 2.0, 2.0)
  setVelocity(this, 40.0, 0.0, 0.0)
  setRotationVelocity(this, -10.0, 0.0, 0.0)
  setTypeId(this, 0)

  suspend(this, 0.25)

  shooting_object.setupShots(this, "./scripts/player_shot.lua", 0.25)
end

function on_update(this, elapsedTime)
  shooting_object.update_shots(elapsedTime)

  if nextMissileShot > 0.0 then nextMissileShot = nextMissileShot - elapsedTime end
  if invul > 0.0 then invul = invul - elapsedTime end

  -- handle invul --
  if collisionRecover == 1 then
    isDrawn = getDrawEnabled(this)

    if isDrawn == 0 then
      enableDraw(this)
    else
      disableDraw(this)
    end

    if invul <= 0.0 then
      enableDraw(this)
      collisionRecover = 0
    end
  end

  -- handle individual states
  if progress == 0 then
    camera = getCamera()
    this_position = getPosition(this)
    camera_position = getPosition(camera)
    tx = camera_position[1] - 20.0
    if this_position[1] >= tx then
      setVelocity(this, 8.0, 0.0, 0.0)
      progress = 1
      suspend(this, 1.0)
    else
      suspend(this, 0.25)
    end
  elseif progress == 1 then
    setVelocity(this, 10.0, 0.0, 0.0)
    setRotationVelocity(this, 0.0, 0.0, 0.0)
    progress = 2
  elseif progress == 2 then
    this_rotation = getRotation(this)
    setInterpolationVariableBeginValue(this, 0.0)
    setInterpolationVariableEndValue(this, 0.5)
    setInterpolationVariableCurrentValue(this, 0.0)
    setInterpolationRotationStartv(this, this_rotation)
    setInterpolationRotationEnd(this, 0.0, 1.57, 0.0)
    setInterpolationEnable(this, 1)

    progress = 3
  elseif progress == 3 then
    interpTime = interpTime + elapsedTime
    setInterpolationVariableCurrentValue(this, interpTime)

    if interpTime > 0.5 then
      setInterpolationEnable(this, 0)
      progress = 4
    end
  elseif progress == 4 then
    this_position   = getPosition(this)
    this_velocity   = getVelocity(this)
    camera          = getCamera()
    camera_velocity = getVelocity(camera)

    this_velocity[1] = camera_velocity[1]
    this_velocity[2] = camera_velocity[2]

    if engine_testSpecialKeyPressed(101) == 1 then upDown = 1 end
    if engine_testSpecialKeyPressed(103) == 1 then downDown = 1 end
    if engine_testSpecialKeyPressed(100) == 1 then leftDown = 1 end
    if engine_testSpecialKeyPressed(102) == 1 then rightDown = 1 end

    if engine_testSpecialKeyReleased(101) == 1 then upDown = 0 end
    if engine_testSpecialKeyReleased(103) == 1 then downDown = 0 end
    if engine_testSpecialKeyReleased(100) == 1 then leftDown = 0 end
    if engine_testSpecialKeyReleased(102) == 1 then rightDown = 0 end

    if upDown == 1 then this_velocity[2] = this_velocity[2] + 10 end
    if downDown == 1 then this_velocity[2] = this_velocity[2] - 10 end
    if leftDown == 1 then this_velocity[1] = this_velocity[1] - 10 end
    if rightDown == 1 then this_velocity[1] = this_velocity[1] + 10 end

    -- Ax + By + Cz + D = 0
    this_pos = getPosition(this)
    plane    = camera_getFrustumPlane(1)
    x        = -(((plane[3] * this_pos[3]) + plane[4]) / plane[1])

    if this_pos[1] > x - getBoundingSphereRadius(this) then
      this_pos[1] = x - getBoundingSphereRadius(this)
    end

    plane = camera_getFrustumPlane(0)
    x     = -(((plane[3] * this_pos[3]) + plane[4]) / plane[1])

    if this_pos[1] < x + getBoundingSphereRadius(this) then
      this_pos[1] = x + getBoundingSphereRadius(this)
    end

    plane = camera_getFrustumPlane(2)
    y     = -(((plane[3] * this_pos[3]) + plane[4]) / plane[2])

    if this_pos[2] < y + getBoundingSphereRadius(this) then
      this_pos[2] = y + getBoundingSphereRadius(this)
    end

    plane = camera_getFrustumPlane(3)
    y     = -(((plane[3] * this_pos[3]) + plane[4]) / plane[2])

    if this_pos[2] > y - getBoundingSphereRadius(this) then
      this_pos[2] = y - getBoundingSphereRadius(this)
    end

    setVelocityv(this, this_velocity)
    setPositionv(this, this_pos)

    if engine_testKeyPressed(32) == 1 then
      shooting_object.attemptShot(this, (getBoundingSphereRadius(this) - 1.0))
    end

    if engine_testKeyPressed(string.byte("m", 1)) == 1 and
       nextMissileShot <= 0.0 and
       numMissiles > 0 then
      obj = shooting_object.setShot(this, "./scripts/player_missile.lua")
      nextMissileShot = .75
      numMissiles = numMissiles - 1
    end
  end
end

function on_collision(this,temp)
  tempId = getTypeId(temp)

  if collisionRecover == 0 and (tempId == 1 or tempId == 10 or tempId == 4) then
    life             = life - 1
    collisionRecover = 1
    invul            = 1.0

    if life <= 0 then
      life = 10
      if lives > 0 then lives = lives - 1 end
    end
  elseif collisionRecover == 0 and tempId == 20 then
    life             = life - 2
    collisionRecover = 1
    invul            = 1.0
    if life <= 0 then
      life = 10
      if lives > 0 then lives = lives - 1 end
    end
  elseif tempId == 5 then
    numMissiles = numMissiles + 5
    if numMissiles > 30 then numMissiles = 30 end
  elseif tempId == 6 then
    life = life + 5
    if life > 10 then life = 10 end
  elseif tempId == 7 then
    lives = lives + 1
    if lives > 99 then lives = 99 end
  end
end
