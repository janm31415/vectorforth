#include "view.h"

#include <SDL_syswm.h>

#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl2.h"
#include "imgui_stdlib.h"
#include "imguifilesystem.h"

#include <stdexcept>
#include <chrono>
#include <string>
#include <fstream>
#include <streambuf>
#include <ctime>

#include "shader_program.h"
#include "logging.h"

namespace
  {
  void gl_check_error(const char* txt)
    {
    unsigned int err = glGetError();
    if (err)
      {
      std::stringstream str;
      str << "GL error " << err << ": " << txt;
      throw std::runtime_error(str.str());
      }
    }

  uint32_t get_next_power_of_two(uint32_t v)
    {
    --v;
    v |= v >> 1;
    v |= v >> 2;
    v |= v >> 4;
    v |= v >> 8;
    v |= v >> 16;
    ++v;
    return v;
    }

  void clear_screen(image<uint32_t>& screen)
    {
    for (auto& v : screen)
      //v = 0xff000000;
      v = 0xff000000 | (uint32_t(49) << 16) | (uint32_t(49) << 8) | uint32_t(49);
    }
  }

view::view() : _w(1600), _h(900), 
_viewport_w(800), _viewport_h(450), _viewport_pos_x(50), _viewport_pos_y(50), 
_quit(false), _pause(false), _line_nr(1), _col_nr(1), _program(_viewport_w, _viewport_h),
_optimize(true)
  {
  SDL_DisplayMode DM;
  SDL_GetCurrentDisplayMode(0, &DM);
  _w_max = DM.w;
  _h_max = DM.h;

  if (_w > _w_max)
    _w = _w_max;
  if (_h > _h_max)
    _h = _h_max;

  // Setup window
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
  SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
  SDL_DisplayMode current;
  SDL_GetCurrentDisplayMode(0, &current);

  _window = SDL_CreateWindow("ShaderForth",
    SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
    _w, _h,
    SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN);
  if (!_window)
    throw std::runtime_error("SDL can't create a window");

  SDL_GLContext gl_context = SDL_GL_CreateContext(_window);
  //SDL_GL_SetSwapInterval(1); // Enable vsync


  IMGUI_CHECKVERSION();
  ImGui::CreateContext();

  // Setup Platform/Renderer bindings
  ImGui_ImplSDL2_InitForOpenGL(_window, gl_context);
  ImGui_ImplOpenGL2_Init();

  // Setup Style
  ImGui::StyleColorsDark();
  ImGui::GetStyle().Colors[ImGuiCol_TitleBg] = ImGui::GetStyle().Colors[ImGuiCol_TitleBgActive];

  SDL_GL_MakeCurrent(_window, gl_context);

  glEnable(GL_TEXTURE_2D);
  glGenTextures(1, &_gl_texture);
  glBindTexture(GL_TEXTURE_2D, _gl_texture);

  _gl_texture_w = get_next_power_of_two(_w_max);
  _gl_texture_h = get_next_power_of_two(_h_max);

  GLint max_texture_size;
  glGetIntegerv(GL_MAX_TEXTURE_SIZE, &max_texture_size);

  if (_gl_texture_w > (uint32_t)max_texture_size)
    _gl_texture_w = (uint32_t)max_texture_size;
  if (_gl_texture_h > (uint32_t)max_texture_size)
    _gl_texture_h = (uint32_t)max_texture_size;

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _gl_texture_w, _gl_texture_h, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
  gl_check_error("glTexImage2D in view.cpp");

  _settings = read_settings("shaderforth.cfg");

  _screen = image<uint32_t>(_viewport_w, _viewport_h);
  clear_screen(_screen);
  _init_shader_input();
  _reset_time();
  }


view::~view()
  {
  write_settings(_settings, "shaderforth.cfg");
  glDeleteTextures(1, &_gl_texture);
  gl_check_error("glDeleteTextures in view.cpp");

  ImGui_ImplOpenGL2_Shutdown();
  ImGui_ImplSDL2_Shutdown();
  ImGui::DestroyContext();
  SDL_DestroyWindow(_window);
  }

void view::_reset_time()
  {
  _start = std::chrono::high_resolution_clock::now();
  _shader_input.time = 0;
  }

void view::set_shader_code(const std::string& shader)
  {
  if (compiled_shader_code != shader)
    {
    _reset_time();
    if (!_program.compile(shader, _optimize))
      {
      return;
      }
    }
  current_shader_code = shader;
  }


void view::_poll_for_events()
  {
  SDL_Event event;
  while (SDL_PollEvent(&event))
    {
    ImGui_ImplSDL2_ProcessEvent(&event);
    switch (event.type)
      {
      case SDL_QUIT:
      {
      _quit = true;
      break;
      }
      case SDL_WINDOWEVENT:
      {
      if (event.window.event == SDL_WINDOWEVENT_RESIZED)
        {
        //_w = event.window.data1;
        //_h = event.window.data2;   
        _w = (uint32_t)event.window.data1 < _w_max ? (uint32_t)event.window.data1 : _w_max;
        _h = (uint32_t)event.window.data2 < _h_max ? (uint32_t)event.window.data2 : _h_max;
        }
      break;
      }
      case SDL_MOUSEMOTION:
      {
      if (_shader_input.mz > 0.f)
        {
        float x = float(event.motion.x);
        float y = float(event.motion.y);

        float width_ratio = (float)_viewport_w / (float)_w;
        float height_ratio = (float)_viewport_h / (float)_h;

        float mx = (x - _viewport_pos_x);
        float my = _viewport_h - (y - _viewport_pos_y);

        if (_settings.fullscreen)
          {
          mx = x * width_ratio;
          my = _viewport_h - y * height_ratio;
          }

        if (mx >= 0.f && my >= 0.f && mx <= _viewport_w && my <= _viewport_h)
          {
          _shader_input.mx = mx;
          _shader_input.my = my;
          }
        }
      break;
      }
      case SDL_MOUSEBUTTONDOWN:
      {
      float x = float(event.motion.x);
      float y = float(event.motion.y);

      float width_ratio = (float)_viewport_w / (float)_w;
      float height_ratio = (float)_viewport_h / (float)_h;

      float mx = (x - _viewport_pos_x);
      float my = _viewport_h - (y - _viewport_pos_y);

      if (_settings.fullscreen)
        {
        mx = x * width_ratio;
        my = _viewport_h - y * height_ratio;
        }
      if (mx >= 0.f && my >= 0.f && mx <= _viewport_w && my <= _viewport_h)
        {
        _shader_input.mx = mx;
        _shader_input.my = my;
        _shader_input.mz = mx;
        _shader_input.mw = my;
        }
      break;
      }
      case SDL_MOUSEBUTTONUP:
      {
      if (_shader_input.mz > 0.f)
        {
        _shader_input.mz = -_shader_input.mz;
        _shader_input.mw = -_shader_input.mw;
        }
      break;
      }
      }
    }
  }

void view::_imgui_ui()
  {
  // Start the Dear ImGui frame
  ImGui_ImplOpenGL2_NewFrame();
  ImGui_ImplSDL2_NewFrame(_window);
  ImGui::NewFrame();

  ImGuiWindowFlags window_flags = 0;
  window_flags |= ImGuiWindowFlags_NoTitleBar;
  window_flags |= ImGuiWindowFlags_NoMove;
  window_flags |= ImGuiWindowFlags_NoCollapse;
  window_flags |= ImGuiWindowFlags_MenuBar;
  window_flags |= ImGuiWindowFlags_NoBackground;
  window_flags |= ImGuiWindowFlags_NoResize;
  window_flags |= ImGuiWindowFlags_NoScrollbar;

  ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
  ImGui::SetNextWindowSize(ImVec2((float)_w, 10), ImGuiCond_Always);
  bool open = true;
  static bool open_channel = false;
  static int channel = 0;
  if (ImGui::Begin("ShaderToy", &open, window_flags))
    {
    if (!open)
      _quit = true;
    if (ImGui::BeginMenuBar())
      {
      if (ImGui::BeginMenu("File"))
        {
        if (ImGui::MenuItem("Exit"))
          {
          _quit = true;
          }
        ImGui::EndMenu();
        }
      if (ImGui::BeginMenu("Window"))
        {
        if (ImGui::MenuItem("Fullscreen", NULL, &_settings.fullscreen))
          {
          Logging::Info() << "Toggle fullscreen " << (_settings.fullscreen ? "on" : "off") << "\n";
          }
        ImGui::MenuItem("Log window", NULL, &_settings.log_window);
        ImGui::MenuItem("Script window", NULL, &_settings.script_window);
        ImGui::EndMenu();
        }
      ImGui::EndMenuBar();
      }
    ImGui::End();
    }

  _buttons_window();

  if (_settings.log_window)
    _log_window();

  if (_settings.script_window)
    _script_window();

  //ImGui::ShowDemoWindow();

  ImGui::Render();
  }

void view::_init_shader_input()
  {
  _shader_input.resolution_x = (float)_viewport_w;
  _shader_input.resolution_y = (float)_viewport_h;
  _shader_input.time = 0.f;
  _shader_input.mx = 0.f;
  _shader_input.my = 0.f;
  _shader_input.mz = -1.f;
  _shader_input.mw = -1.f;
  _shader_input.frame = 0;
  _shader_input.time_delta = 0.f;
  }

void view::_bind_shader_input()
  {
  _program.set_shader_input(_shader_input);
  }


void view::_buttons_window()
  {
  if (_settings.fullscreen)
    return;

  ImGui::SetNextWindowSize(ImVec2((float)_viewport_w, (float)(_viewport_pos_y)), ImGuiCond_Always);
  ImGui::SetNextWindowPos(ImVec2((float)_viewport_pos_x, (float)(_viewport_pos_y + _viewport_h)), ImGuiCond_Always);

  ImGuiWindowFlags flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBackground;
  if (!ImGui::Begin("Buttons window", nullptr, flags))
    {
    ImGui::End();
    return;
    }

  ImGui::PushItemWidth(100);
  if (_pause)
    {
    if (ImGui::Button("Play"))
      _pause = false;
    }
  else
    {
    if (ImGui::Button(" || "))
      _pause = true;
    }
  ImGui::SameLine();
  if (ImGui::Button(" << "))
    {
    _reset_time();
    }
  ImGui::SameLine();

  ImGui::Text("%.2f FPS", 1.f/_shader_input.time_delta);
  ImGui::End();
  }

void view::_log_window()
  {
  static AppLog log;

  auto log_messages = Logging::GetInstance().pop_messages();

  if (!log_messages.empty())
    log.AddLog(log_messages.c_str());

  ImGui::SetNextWindowSize(ImVec2((float)_viewport_w, (float)(_h - 3 * _viewport_pos_y - _viewport_h)), ImGuiCond_Always);
  ImGui::SetNextWindowPos(ImVec2((float)_viewport_pos_x, (float)(2 * _viewport_pos_y + _viewport_h)), ImGuiCond_Always);

  log.Draw("Log window", &_settings.log_window);
  }

int view::script_window_callback(ImGuiInputTextCallbackData* data)
  {
  int cur = data->CursorPos;
  if (cur >= current_shader_code.size())
    cur = (int)current_shader_code.size() - 1;
  int line_nr = 3;
  int col_nr = 1;
  for (int i = 0; i < cur; ++i)
    {
    if (current_shader_code[i] == '\n')
      {
      ++line_nr;
      col_nr = 1;
      }
    else
      ++col_nr;
    }
  _line_nr = line_nr;
  _col_nr = col_nr;
  return 0;
  }

namespace
  {
  int _script_window_callback(ImGuiInputTextCallbackData *data)
    {
    return ((view*)data->UserData)->script_window_callback(data);
    }
  }

void view::_script_window()
  {
  ImGui::SetNextWindowSize(ImVec2((float)(_w - _viewport_w - 3 * _viewport_pos_x), (float)(_h - 2 * _viewport_pos_y)), ImGuiCond_Always);
  ImGui::SetNextWindowPos(ImVec2((float)(_viewport_pos_x * 2 + _viewport_w), (float)(_viewport_pos_y)), ImGuiCond_Always);

  if (!ImGui::Begin("Script window", &_settings.script_window))
    {
    ImGui::End();
    return;
    }

  ImGuiInputTextFlags flags = ImGuiInputTextFlags_CallbackAlways;
  ImGui::InputTextMultiline("Scripting", &current_shader_code, ImVec2(-1.0f, _h - 2 * _viewport_pos_y - ImGui::GetTextLineHeight() * 6), flags, &_script_window_callback, this);
  if (ImGui::Button("Compile"))
    {
    set_shader_code(current_shader_code);
    _reset_time();
    }
  ImGui::SameLine();
  static bool open_script = false; 
  if (ImGui::Button("Load"))
    {
    open_script = true;
    }
  ImGui::SameLine();
  static bool save_script = false;
  if (ImGui::Button("Save"))
    {
    save_script = true;
    }
  ImGui::SameLine();
  ImGui::Checkbox("Optimize", &_optimize);
  ImGui::SameLine();
  if (ImGui::Button("Asm"))
    {
    _program.log_assembly(current_shader_code, _optimize);
    }

  static ImGuiFs::Dialog open_script_dlg(false, true, true);
  const char* openScriptChosenPath = open_script_dlg.chooseFileDialog(open_script, _settings.file_open_folder.c_str(), ".4th", "Open forth shader", ImVec2(-1, -1), ImVec2(50, 50));
  open_script = false;
  if (strlen(openScriptChosenPath) > 0)
    {
    _settings.file_open_folder = open_script_dlg.getLastDirectory();
    std::ifstream t(openScriptChosenPath);
    std::string str((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
    current_shader_code = str;
    }

  static ImGuiFs::Dialog save_script_dlg(false, true, true);
  const char* saveScriptChosenPath = save_script_dlg.saveFileDialog(save_script, _settings.file_open_folder.c_str(), 0, ".4th", "Save forth shader", ImVec2(-1, -1), ImVec2(50, 50));
  save_script = false;
  if (strlen(saveScriptChosenPath) > 0)
    {
    _settings.file_open_folder = save_script_dlg.getLastDirectory();
    std::ofstream t(saveScriptChosenPath);
    t << current_shader_code;
    t.close();
    }

  ImGui::Text("Ln %d\tCol %d", _line_nr, _col_nr);
  ImGui::End();
  }

void view::_blit_screen_to_opengl_texture()
  {
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  if (_settings.fullscreen)
    {
    glOrtho(0, _viewport_w, 0, _viewport_h, 0, 1);
    glMatrixMode(GL_MODELVIEW);
    glViewport(0, 0, _w, _h);
    glClear(GL_COLOR_BUFFER_BIT);

    }
  else
    {
    glOrtho(0, _w, 0, _h, 0, 1);
    glMatrixMode(GL_MODELVIEW);
    glViewport(0, 0, _w, _h);
    glClear(GL_COLOR_BUFFER_BIT);
    }

  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, _gl_texture);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);


  const uint32_t step_block_x = _viewport_w / _gl_texture_w + 1;
  const uint32_t step_block_y = _viewport_h / _gl_texture_h + 1;

  for (uint32_t block_y = 0; block_y < step_block_y; ++block_y)
    {
    for (uint32_t block_x = 0; block_x < step_block_x; ++block_x)
      {
      const uint32_t x_offset = block_x * _gl_texture_w;
      const uint32_t y_offset = block_y * _gl_texture_h;
      uint32_t wi = std::min<uint32_t>(_gl_texture_w, _viewport_w - x_offset);
      uint32_t he = std::min<uint32_t>(_gl_texture_h, _viewport_h - y_offset);

      uint32_t* p_im_start = (uint32_t*)(_screen.row(y_offset) + x_offset);
      glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
      glPixelStorei(GL_UNPACK_ROW_LENGTH, _screen.stride());
      glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, wi, he, GL_RGBA, GL_UNSIGNED_BYTE, (void*)p_im_start);
      gl_check_error("glTexSubImage2D in view.cpp");
      const float tex_w = float(wi) / float(_gl_texture_w);
      const float tex_h = float(he) / float(_gl_texture_h);

      float x0 = float(x_offset);
      float y0 = float(y_offset);

      if (!_settings.fullscreen)
        {
        x0 += (float)_viewport_pos_x;
        y0 += (float)(_h - _viewport_h - _viewport_pos_y);
        }

      const float x1 = x0 + float(wi);
      const float y1 = y0 + float(he);
      glBegin(GL_TRIANGLE_STRIP);
      glTexCoord2f(0.0, 0.0);
      glVertex2f(x0, y0);
      glTexCoord2f(tex_w, 0.0);
      glVertex2f(x1, y0);
      glTexCoord2f(0.0, tex_h);
      glVertex2f(x0, y1);
      glTexCoord2f(tex_w, tex_h);
      glVertex2f(x1, y1);
      glEnd();
      gl_check_error("GL_TRIANGLE_STRIP in view.cpp");
      }
    }
  }

void view::loop()
  {
  auto last_tic = std::chrono::high_resolution_clock::now();
  while (!_quit)
    {
    auto tic = std::chrono::high_resolution_clock::now();
    _shader_input.time_delta = (float)(std::chrono::duration_cast<std::chrono::microseconds>(tic - last_tic).count()) / 1000000.f;
    last_tic = tic;

    _poll_for_events();

    if (!_pause)
      {
      _shader_input.resolution_x = (float)_viewport_w;
      _shader_input.resolution_y = (float)_viewport_h;
      _shader_input.time += _shader_input.time_delta;
      _bind_shader_input();

      _program.run(_screen);

      ++_shader_input.frame;
      }

    _imgui_ui();

    _blit_screen_to_opengl_texture();

    ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());


    SDL_GL_SwapWindow(_window);

    glGetError(); //hack
    }
  }