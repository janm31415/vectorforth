#include "logging.h"

void AppLog::Clear() { Buf.clear(); LineOffsets.clear(); }

void AppLog::AddLog(const char* fmt, ...)// IM_FMTARGS(2)
  {
  int old_size = Buf.size();
  va_list args;
  va_start(args, fmt);
  Buf.appendfv(fmt, args);
  va_end(args);
  for (int new_size = Buf.size(); old_size < new_size; old_size++)
    if (Buf[old_size] == '\n')
      LineOffsets.push_back(old_size);
  ScrollToBottom = true;
  }

void AppLog::Draw(const char* title, bool* p_open)
  {
  //ImGui::SetNextWindowSize(ImVec2(800, 300), ImGuiCond_Appearing);
  //ImGui::SetNextWindowPos(ImVec2(50, 550), ImGuiCond_Appearing);
  if (!ImGui::Begin(title, p_open))
    {
    ImGui::End();
    return;
    }
  if (ImGui::Button("Clear")) Clear();
  ImGui::SameLine();
  bool copy = ImGui::Button("Copy");
  ImGui::SameLine();
  Filter.Draw("Filter", -100.0f);
  ImGui::Separator();
  ImGui::BeginChild("scrolling", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);
  if (copy) ImGui::LogToClipboard();

  if (Filter.IsActive())
    {
    const char* buf_begin = Buf.begin();
    const char* line = buf_begin;
    for (int line_no = 0; line != NULL; line_no++)
      {
      const char* line_end = (line_no < LineOffsets.Size) ? buf_begin + LineOffsets[line_no] : NULL;
      if (Filter.PassFilter(line, line_end))
        ImGui::TextUnformatted(line, line_end);
      line = line_end && line_end[1] ? line_end + 1 : NULL;
      }
    }
  else
    {
    ImGui::TextUnformatted(Buf.begin());
    }

  if (ScrollToBottom)
    ImGui::SetScrollHereY(1.0f);
  ScrollToBottom = false;
  ImGui::EndChild();
  ImGui::End();
  }

Logging& Logging::GetInstance()
  {
  static Logging instance; // Guaranteed to be destroyed.
                        // Instantiated on first use.
  return instance;
  }

Logging::Logging()
  {
  }

Logging& Logging::operator << (const char* text)
  {
  _os << text;
  return *this;
  }

std::string Logging::pop_messages()
  {
  std::string mess = _os.str();
  _os.clear();
  _os.str("");
  return mess;
  }

Logging& Logging::Info()
  {
  return GetInstance() << "[Info] ";
  }

Logging& Logging::Warning()
  {
  return GetInstance() << "[Warning] ";
  }

Logging& Logging::Error()
  {
  return GetInstance() << "[Error] ";
  }