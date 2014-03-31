dofile('./scripts/debug_lib.lua')

levels = {
	"./levels/menu.lua", 
	"./levels/level1.lua", 
	"./levels/level2.lua", 
	"./levels/level3.lua", 
	"./levels/level4.lua",
    "./levels/level5.lua",
	"./levels/credits.lua"
}

levelNum     = 1
totalTime    = 0.0
debugEnabled = false

function on_load()
    loadLevel(levels[levelNum])
    setLevelId(levelNum)

    debugEnabled = engine_testDebugEnabled()
    return
end

function on_update(elapsedTime)
    totalTime = totalTime + elapsedTime

    if debugEnabled == 1 then
        handle_debug_controls(elapsedTime)
    end
    
    -- handle global user control
    if engine_testKeyPressed(27) == 1 then 
        loadLevel(levels[1]) 
        setLevelId(1)
    end
   
    if engine_testKeyPressed(string.byte("*", 1)) == 1 then
        swapLevel()
    end

    if engine_testKeyPressed(string.byte("p", 1)) == 1 then pause() end
    
    if engine_testKeyPressed(string.byte("L", 1)) == 1 then
        levelNum = levelNum + 1
        if levelNum > 7 then levelNum = 1 end
        loadLevel(levels[levelNum])
    end

    if engine_testKeyPressed(string.byte("l", l)) == 1 then
        levelNum = levelNum - 1
        if levelNum < 1 then levelNum = 5 end
        loadLevel(levels[levelNum])
    end

    if engine_testKeyPressed(string.byte("k", 1)) == 1 or 
       engine_testKeyPressed(string.byte("K", 1)) == 1 then
        loadLevel(levels[levelNum])
    end

    levelComplete = testLevelComplete()
	
    if levelComplete == 1 then
        levelId = getLevelId()
       	  
       levelNum = levelNum + 1
       if levelNum > 7 then levelNum = 2 end

       loadLevel(levels[levelNum])
    end

    return
end

function on_unload()
    return
end

