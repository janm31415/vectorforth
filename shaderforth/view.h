#pragma once
#include <SDL.h>
#include <SDL_opengl.h>
#include <string>
#include <chrono>
#include <array>

#include "settings.h"
#include "shader_input.h"
#include "shader_program.h"

#include "image.h"

struct ImGuiInputTextCallbackData;

class view
  {
  public:
    view();
    ~view();

    void set_shader_code(const std::string& shader);    

    void loop();
    int script_window_callback(ImGuiInputTextCallbackData* data);

  private:
    void _poll_for_events();
    void _imgui_ui();   
    void _log_window();
    void _buttons_window();
    void _script_window();
    void _init_shader_input();
    void _bind_shader_input();
    void _reset_time();
    void _blit_screen_to_opengl_texture();

  private:
    std::string current_shader_code, compiled_shader_code;
    SDL_Window* _window;    
    uint32_t _w, _h, _w_max, _h_max, _viewport_w, _viewport_h, _viewport_pos_x, _viewport_pos_y;
    GLuint _gl_texture;
    uint32_t _gl_texture_w;
    uint32_t _gl_texture_h;
    image<uint32_t> _screen;
    shader_input _shader_input;
    bool _quit;
    bool _pause;
    settings _settings;
    std::chrono::high_resolution_clock::time_point _start;
    int _line_nr, _col_nr;
    bool _optimize;
    shader_program _program;
  };