-- Overridden Engine Callbacks
function on_load(this)
  setModel(this, "GreenShot.mdl")
  setTypeId(this, 20)
  enableAlwaysLit(this)

  playSound(this, "laser.wav")
  setScale(this, 4.0, 4.0, 4.0)
end

function on_update(this, elapsedTime)
  in_view = getInView(this)

  if in_view == 0 then
    removeObject(this)
  end
end

function on_collision(this, temp)
  typeId = getTypeId(temp)

  if typeId ~= 1 and typeId ~= 3 and typeId ~= 2 then
    removeObject(this)
  end
end
