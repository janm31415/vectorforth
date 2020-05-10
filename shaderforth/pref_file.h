#pragma once

#include <string>
#include <sstream>
#include <vector>
#include <iomanip>
#include <memory>
#include <stdint.h>

struct node_type;

class pref_file_element
  {
  template <class T>
  friend pref_file_element operator<< (pref_file_element in, T val);  

  template <class T>
  friend pref_file_element operator<< (pref_file_element in, const std::vector<T>& val);

  public:
    pref_file_element(const char* _parent_label, const std::shared_ptr<node_type>& _node);

    virtual ~pref_file_element();
     
    pref_file_element operator [] (const char* label) const;

    std::string to_string() const;    

    std::vector<std::string> get_text() const;

    bool valid() const;

  private:
    pref_file_element AddChild(const char* label);
    pref_file_element AddValue(const std::string& value);
    pref_file_element AddValue(const char* value);
    pref_file_element AddValue(double value);
    pref_file_element AddValue(int value);
    pref_file_element AddValue(int64_t value);
    pref_file_element AddValue(uint64_t value);
    pref_file_element AddValue(bool value);
    pref_file_element PushValue(const std::string& value);
    pref_file_element PushValue(const char* value);
    pref_file_element PushValue(double value);
    pref_file_element PushValue(int value);
    pref_file_element PushValue(int64_t value);
    pref_file_element PushValue(uint64_t value);
    pref_file_element PushValue(bool value);

  protected:
    std::shared_ptr<node_type> node;
    std::string parent_label;
  };

template <class T>
inline pref_file_element operator>> (const pref_file_element &in, T& val)
  {
  if (in.valid())
    {    
    std::stringstream str;
    str << in.to_string();
    str >> val;
    }
  return in;
  }

template <>
inline pref_file_element operator >> (const pref_file_element &in, std::string& val)
  {
  if (in.valid())
    {
    std::stringstream str;
    str << in.to_string();
    val = str.str();
    }
  return in;
  }

template <class T>
inline pref_file_element operator>> (const pref_file_element &in, std::vector<T>& val)
  {
  if (in.valid())
    {
    auto lst = in.get_text();
    for (auto c : lst)
      {      
      if (!c.empty())
        {
        std::stringstream str;
        str << c;
        T m;
        str >> m;
        val.push_back(m);
        }
      }
    }
  return in;
  }


template <>
inline pref_file_element operator >> (const pref_file_element &in, std::vector<std::string>& val)
  {
  if (in.valid())
    {
    auto lst = in.get_text();
    for (auto c : lst)
      {
      if (!c.empty())
        {
        std::stringstream str;
        str << c;        
        val.push_back(str.str());
        }
      }
    }
  return in;
  }

template <class T>
inline pref_file_element operator<< (pref_file_element in, T val)
  {
  if (in.valid())
    {
    return in.AddValue(val);
    }
  else 
    return in;
  }

template <class T>
inline pref_file_element operator<< (pref_file_element in, const std::vector<T>& val)
  {
  for (auto v : val)      
    in.PushValue(v);      
  return in;
  }

template <>
inline pref_file_element operator<< (pref_file_element in, const char* val)
  {
  if (in.valid())
    {
    return in.AddChild(val);
    }
  else
    return in;
  }


class pref_file : public pref_file_element
  {
  public:
    enum EFileMode
      {
      READ,
      WRITE
      };

    pref_file(const char* _filename, EFileMode _mode);

    virtual ~pref_file();    

    void release();

  private:
    std::string filename;
    EFileMode mode;    
  };