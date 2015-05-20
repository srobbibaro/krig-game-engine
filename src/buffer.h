#ifndef _BUFFER_H_
#define _BUFFER_H_
const char *intro_script_buffer =
  "local alpha = 0.0\n"

  "function on_load()\n"
    "return\n"
  "end\n"

  "function on_unload()\n"
    "return\n"
  "end\n"

  "function on_update(this)\n"
    "return\n"
  "end\n"

  "function on_draw()\n"
    "return\n"
  "end\n"

  "function on_draw_screen(this, elapsedTime)\n"
    "alpha = alpha + (elapsedTime / 2.0)\n"
    "if alpha >= 2.0 then\n"
      "setComplete(1)\n"
    "end\n"

    "gl.Translate (0.0, 0.0, -2.0)\n"
    "gl.PushMatrix()\n"
    "gl.LineWidth(3.0)\n"
    "gl.Color(0.8, 0.8, 0.8, alpha)\n"

    "displayText(\"Krig Game Engine\", -0.6, 0.1, -0.01, 0.001, 0.001)\n"
    "displayText(\"2007-2015\", -0.2, -0.1, -0.01, 0.0005, 0.001)\n"

    "gl.PopMatrix()\n"
    "gl.LineWidth(3.0)\n"
    "return\n"
  "end\n";
#endif
