#include "pref_file.h"

#include <sstream>
#include <iostream>
#include <fstream>

#include <json.hpp>

struct node_type
  {
  node_type() : j(nullptr) {}
  node_type(nlohmann::json& i_j) : j(&i_j) {}

  nlohmann::json* j;
  };

pref_file_element::pref_file_element(const char* _parent_label, const std::shared_ptr<node_type>& _node) : node(_node), parent_label(_parent_label)
  {}

pref_file_element::~pref_file_element()
  {  
  }

namespace
  {
  std::string _to_string(const nlohmann::json& j)
    {
    std::stringstream str;
    switch (j.type())
      {
      case nlohmann::detail::value_t::string:
      {
      const std::string& s = j.get<std::string>();
      return s;
      break;
      }
      case nlohmann::detail::value_t::number_integer:
      {
      str << j.get<nlohmann::json::number_integer_t>();
      return str.str();
      break;
      }
      case nlohmann::detail::value_t::number_unsigned:
      {
      str << j.get<nlohmann::json::number_unsigned_t>();
      return str.str();
      break;
      }
      case nlohmann::detail::value_t::number_float:
      {
      str << j.get<nlohmann::json::number_float_t>();
      return str.str();
      break;
      }
      case nlohmann::detail::value_t::boolean:
      {
      str << j.get<nlohmann::json::boolean_t>();
      return str.str();
      break;
      }
      }
    return std::string();
    }
  }

std::string pref_file_element::to_string() const
  {
  if (node)
    {
    return _to_string(*(node->j));
    }
  return std::string();
  }

std::vector<std::string> pref_file_element::get_text() const
  {
  std::vector<std::string> out;
  if (node)
    {    
    switch (node->j->type())
      {
      case nlohmann::detail::value_t::array:
      {
      for (nlohmann::json::const_iterator it = node->j->begin(); it != node->j->end(); ++it)
        out.push_back(_to_string(it.value()));
      }
      }
    } 
  std::string s = _to_string(*(node->j));
  if (!s.empty())
    out.push_back(s);
  return out;
  }

bool pref_file_element::valid() const
  {
  return node != nullptr;
  }

pref_file_element pref_file_element::operator[](const char* label) const
  {
  if (node)
    {
    switch (node->j->type())
      {
      case nlohmann::detail::value_t::object:
      {
      for (nlohmann::json::iterator it = node->j->begin(); it != node->j->end(); ++it)
        {
        if (it.key() == std::string(label))
          {
          return pref_file_element(label, std::make_shared<node_type>(it.value()));
          }
        }
      break;
      }
      }
    }
  return pref_file_element(label, nullptr);
  }

pref_file_element pref_file_element::AddChild(const char* label)
  {  
  (*node->j)[label] = nullptr;  
  for (nlohmann::json::iterator it = node->j->begin(); it != node->j->end(); ++it)
    {
    if (it.key() == std::string(label))
      {
      return pref_file_element(label, std::make_shared<node_type>(it.value()));
      }
    }
  return pref_file_element(label, nullptr);
  }

pref_file_element pref_file_element::AddValue(const std::string& value)
  {
  (*node->j) = value;
  return *this;
  }

pref_file_element pref_file_element::AddValue(const char* value)
  {
  (*node->j) = value;
  return *this;
  }

pref_file_element pref_file_element::AddValue(double value)
  {
  (*node->j) = value;
  return *this;
  }

pref_file_element pref_file_element::AddValue(int value)
  {
  (*node->j) = value;
  return *this;
  }

pref_file_element pref_file_element::AddValue(int64_t value)
  {
  (*node->j) = value;
  return *this;
  }

pref_file_element pref_file_element::AddValue(uint64_t value)
  {
  (*node->j) = value;
  return *this;
  }

pref_file_element pref_file_element::AddValue(bool value)
  {
  (*node->j) = value;
  return *this;
  }


pref_file_element pref_file_element::PushValue(const std::string& value)
  {
  (*node->j).push_back(value);
  return *this;
  }

pref_file_element pref_file_element::PushValue(const char* value)
  {
  (*node->j).push_back(value);
  return *this;
  }

pref_file_element pref_file_element::PushValue(double value)
  {
  (*node->j).push_back(value);
  return *this;
  }

pref_file_element pref_file_element::PushValue(int value)
  {
  (*node->j).push_back(value);
  return *this;
  }

pref_file_element pref_file_element::PushValue(int64_t value)
  {
  (*node->j).push_back(value);
  return *this;
  }

pref_file_element pref_file_element::PushValue(uint64_t value)
  {
  (*node->j).push_back(value);
  return *this;
  }

pref_file_element pref_file_element::PushValue(bool value)
  {
  (*node->j).push_back(value);
  return *this;
  }
///////////////////////////////////////////////////////////////////////////////

pref_file::pref_file(const char* _filename, EFileMode _mode) : filename(_filename), mode(_mode),
pref_file_element("root", std::make_shared<node_type>())
  {
  node->j = new nlohmann::json();
  if (mode == READ)
    {    
    std::ifstream i(_filename);
    if (i.is_open())
      {
      try
        {
        i >> *(node->j);
        }
      catch (nlohmann::detail::exception e)
        {
        printf("Error: %s\n", e.what());
        }
      i.close();
      }
    }
  }

pref_file::~pref_file()
  {
  delete node->j;
  }

void pref_file::release()
  {
  if (mode == WRITE)
    {
    std::ofstream o(filename);
    if (o.is_open())
      {
      o << *(node->j);
      o.close();
      }
    }
  }
