#ifndef _BUFFER_H_
#define _BUFFER_H_
const char *intro_level_script_buffer =
  "local alpha = 0.0\n"

  "function on_draw_screen(this, elapsedTime)\n"
    "alpha = alpha + (elapsedTime / 2.0)\n"
    "if alpha >= 2.0 then\n"
      "krig.level.set_complete(1)\n"
    "end\n"

    "gl.Translate (0.0, 0.0, -2.0)\n"
    "gl.PushMatrix()\n"
    "gl.LineWidth(3.0)\n"
    "gl.Color(0.8, 0.8, 0.8, alpha)\n"

    "krig.display_text(\"Krig Game Engine\", -0.6, 0.1, -0.01, 0.001, 0.001)\n"
    "krig.display_text(\"2007-2016\", -0.2, -0.1, -0.01, 0.0005, 0.001)\n"

    "gl.PopMatrix()\n"
    "gl.LineWidth(3.0)\n"
  "end\n";
#endif
