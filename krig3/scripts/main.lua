dofile('./scripts/debug_lib.lua')

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
  loadLevel(levels[levelNum])
  setLevelId(levelNum)

  debugEnabled = engine_testDebugEnabled()
end

function on_update(elapsedTime)
  totalTime = totalTime + elapsedTime

  if debugEnabled == 1 then
    handle_debug_controls(elapsedTime)
  end

  -- handle global user control
  if engine_testKeyPressed(27) == 1 then
    loadLevel(levels[1])
    setLevelId(levelNum)
    prevLevelNum = levelNum
    levelNum = 1
  end

  if engine_testKeyPressed(string.byte("*", 1)) == 1 then
    swapLevel()
  end

  if engine_testKeyPressed(string.byte("p", 1)) == 1 then pause() end

  if engine_testKeyPressed(string.byte("L", 1)) == 1 then
    inc_level(1)
    loadLevel(levels[levelNum])
  end

  if engine_testKeyPressed(string.byte("l", l)) == 1 then
    inc_level(-1)
    loadLevel(levels[levelNum])
  end

  if engine_testKeyPressed(string.byte("k", 1)) == 1 or
    engine_testKeyPressed(string.byte("K", 1)) == 1 then
    loadLevel(levels[levelNum])
  end

  if testLevelComplete() == 1 then
    levelId = getLevelId()

    if levelNum == 1 then
      levelNum = prevLevelNum
    else
      inc_level(1)
    end

    loadLevel(levels[levelNum])
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
