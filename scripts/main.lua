totalTime = 0.0

levels = {
	"./levels/menu.lua", 
	"./levels/level1.lua", 
	"./levels/level2.lua", 
	"./levels/level4.lua",
	"./levels/credits.lua"}
levelNum = 1

function on_load()
    loadLevel(levels[levelNum])
    return
end

function on_update(elapsedTime)
    totalTime = totalTime + elapsedTime
    
    -- handle global user control
    if engine_testKeyPressed(27) == 1 then pause() end
    
    if engine_testKeyPressed(string.byte("q", 1)) == 1 or 
       engine_testKeyPressed(string.byte("Q", 1)) == 1 then 
        shutdown()
    end
    
    if engine_testKeyPressed(string.byte("L", 1)) == 1 then
        levelNum = levelNum + 1
        if levelNum > 5 then levelNum = 1 end
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

    return
end

function on_unload()
    return
end

