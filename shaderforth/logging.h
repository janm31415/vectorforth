#pragma once

#include "imgui.h"

#include <sstream>

class AppLog
  {
  public:
    void Clear();
    void AddLog(const char* fmt, ...) IM_FMTARGS(2);
    void Draw(const char* title, bool* p_open = NULL);

  private:
    ImGuiTextBuffer     Buf;
    ImGuiTextFilter     Filter;
    ImVector<int>       LineOffsets;        // Index to lines offset
    bool                ScrollToBottom;
  };



class Logging
  {
  public:
    static Logging& GetInstance();
    static Logging& Info();
    static Logging& Warning();
    static Logging& Error();
    Logging(Logging const&) = delete;
    void operator=(Logging const&) = delete;

    Logging& operator << (const char* text);

    template <class T>
    Logging& operator << (const T& t)
      {
      _os << t;
      return *this;
      }

    std::string pop_messages();

  private:
    Logging();

  private:
    std::ostringstream _os;
  };