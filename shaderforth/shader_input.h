#pragma once

struct shader_input
  {
  float resolution_x; // viewport x resolution (in pixels)
  float resolution_y; // viewport y resolution (in pixels)
  float time; // shader playback time (in seconds)
  float time_delta; // render time (in seconds)
  int frame; // shader playback frame  
  float mx, my, mz, mw; // mouse pixel coords.
  float global_time;
  };