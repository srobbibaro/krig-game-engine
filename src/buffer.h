#ifndef _BUFFER_H_
#define _BUFFER_H_
const char *intro_script_buffer =
"local alpha = 0.0 \
function on_load() \
return \
end \
function on_unload() \
return \
end \
function on_update(this) \
return \
end \
function on_draw() \
return \
end \
function on_draw_screen(this, elapsedTime) \
alpha = alpha + (elapsedTime / 2.0) \
if alpha >= 2.0 then \
setComplete(1) \
end \
gl.Translate (0.0, 0.0, -2.0) \
gl.PushMatrix() \
gl.LineWidth(3.0) \
gl.Color(0.8, 0.8, 0.8, alpha) \
displayText(\"Krig Game Engine\", -0.6, 0.1, -0.01, 0.001, 0.001) \
displayText(\"2008-2014\", -0.2, -0.1, -0.01, 0.0005, 0.001)  \
gl.PopMatrix() \
gl.LineWidth(3.0) \
return \
end";
#endif
