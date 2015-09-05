local debug_lib = require 'scripts/debug_lib'

-- Configuration
levels = {
  "./levels/menu.lua",
  "./levels/level1.lua",
  "./levels/level2.lua",
  "./levels/level3.lua",
  "./levels/level5.lua",
  "./levels/credits.lua"
}

levelNum     = 1
level1       = 2
prevLevelNum = level1
numLevels    = table.getn(levels)

totalTime    = 0.0
debugEnabled = false

-- Overridden Engine Callbacks
function on_load()
  krig.level.load(levels[levelNum])
  krig.level.set_id(levelNum)

  debugEnabled = krig.test_debug_enabled()
end

function on_update(elapsedTime)
  totalTime = totalTime + elapsedTime

  if debugEnabled == 1 then
    debug_lib.handle_debug_controls(elapsedTime)
  end

  -- handle global user control
  if krig.test_key_pressed(27) == 1 then
    krig.level.load(levels[1])
    krig.level.set_id(levelNum)
    prevLevelNum = levelNum
    levelNum = 1
  end

  if krig.test_key_pressed(string.byte("*", 1)) == 1 then
    krig.level.swap()
  end

  if krig.test_key_pressed(string.byte("p", 1)) == 1 then krig.level.pause() end

  if krig.test_key_pressed(string.byte("L", 1)) == 1 then
    inc_level(1)
    krig.level.load(levels[levelNum])
  end

  if krig.test_key_pressed(string.byte("l", l)) == 1 then
    inc_level(-1)
    krig.level.load(levels[levelNum])
  end

  if krig.test_key_pressed(string.byte("k", 1)) == 1 or
    krig.test_key_pressed(string.byte("K", 1)) == 1 then
    krig.level.load(levels[levelNum])
  end

  if krig.level.get_complete() == 1 then
    levelId = krig.level.get_id()

    if levelNum == 1 then
      levelNum = prevLevelNum
    else
      inc_level(1)
    end

    krig.level.load(levels[levelNum])
  end
end

function inc_level(inc)
  levelNum = levelNum + inc

  if levelNum > numLevels then
    levelNum = 1
    prevLevelNum = level1
  elseif levelNum < 1 then
    levelNum = numLevels
    prevLevelNum = level1
  end
end
