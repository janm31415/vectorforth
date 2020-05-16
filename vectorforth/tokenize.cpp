#include "tokenize.h"


#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <algorithm>
#include <sstream>
#include <set>

VF_BEGIN

float to_float(const char* value)
  {
  return (float)atof(value);
  }

int is_number(int* is_real, int* is_scientific, const char* value)
  {
  if (value[0] == '\0')
    return 0;
  int i = 0;
  if (value[0] == 'e' || value[0] == 'E')
    return 0;
  if (value[0] == '-' || value[0] == '+')
    {
    ++i;
    if (value[1] == '\0')
      return 0;
    }
  *is_real = 0;
  *is_scientific = 0;
  const char* s = value + i;
  while (*s != '\0')
    {
    if (isdigit((unsigned char)(*s)) == 0)
      {
      if ((*s == '.') && (*is_real == 0) && (*is_scientific == 0))
        *is_real = 1;
      else if ((*s == 'e' || *s == 'E') && (*is_scientific == 0))
        {
        *is_scientific = 1;
        *is_real = 1;
        if (*(s + 1) == '\0')
          return 0;
        if (*(s + 1) == '-' || *(s + 1) == '+')
          {
          ++s;
          }
        if (*(s + 1) == '\0')
          return 0;
        }
      else
        return 0;
      }
    ++s;
    }
  return 1;
  }

namespace
  {
  bool hex_to_uint64_t(uint64_t& hexvalue, const std::string& h)
    {
    hexvalue = 0;
    if (h.empty())
      return hexvalue;
    int i = (int)h.size() - 1;
    int j = 0;
    for (; i >= 0; --i)
      {
      int v = h[i] >= 'A' ? (h[i] >= 'a' ? ((int)h[i] - 87) : ((int)h[i] - 55)) : ((int)h[i] - 48);
      if (v > 15 || v < 0)
        false;
      hexvalue |= (((uint64_t)v) << (4 * j));
      ++j;
      }

    return true;
    }

  bool ignore_character(const char& ch)
    {
    return (ch == ' ' || ch == '\n' || ch == '\t');
    }

  void _treat_buffer(std::string& buff, std::vector<token>& tokens, int line_nr, int column_nr)
    {
    if (!buff.empty() && buff[0] != '\0')
      {
      int is_real;
      int is_scientific;
      switch (buff[0])
        {
        case '#': // check whether it is an integer
        {
        std::string remainder = buff.substr(1);
        if (is_number(&is_real, &is_scientific, remainder.c_str()) && !is_real)
          tokens.emplace_back(token::T_INTEGER, remainder, line_nr, column_nr - (int)buff.length());
        else
          {
          if (buff.size() > 2 && buff[1] == 'x') // perhaps writtin in hex notation?
            {
            uint64_t hexvalue;
            remainder = buff.substr(2);
            if (hex_to_uint64_t(hexvalue, remainder))
              {
              std::stringstream ss;
              ss << hexvalue;
              tokens.emplace_back(token::T_INTEGER, ss.str(), line_nr, column_nr - (int)buff.length());
              }
            else
              tokens.emplace_back(token::T_WORD, buff, line_nr, column_nr - (int)buff.length());
            }
          else
            tokens.emplace_back(token::T_WORD, buff, line_nr, column_nr - (int)buff.length());
          }
        break;
        }
        case 'v':
        {
        if (buff == "v8")
          tokens.emplace_back(token::T_VECTOR8, buff, line_nr, column_nr - (int)buff.length());
        else
          tokens.emplace_back(token::T_WORD, buff, line_nr, column_nr - (int)buff.length());
        break;
        }
        default:
        {
        if (is_number(&is_real, &is_scientific, buff.c_str()))
          tokens.emplace_back(token::T_FLOAT, buff, line_nr, column_nr - (int)buff.length());        
        else
          tokens.emplace_back(token::T_WORD, buff, line_nr, column_nr - (int)buff.length());
        break;
        }
        }      
      }
    buff.clear();
    }

  }

std::vector<token> tokenize(const std::string& str)
  {
  std::vector<token> tokens;
  std::string buff;

  const char* s = str.c_str();
  const char* s_end = str.c_str() + str.length();

  int line_nr = 1;
  int column_nr = 1;

  while (s < s_end)
    {

    if (ignore_character(*s))
      {
      _treat_buffer(buff, tokens, line_nr, column_nr);

      while (ignore_character(*s))
        {
        if (*s == '\n')
          {
          ++line_nr;
          column_nr = 0;
          }
        ++s;
        ++column_nr;
        }
      }

    const char* s_copy = s;

    switch (*s)
      {

      case '(': // treat as multiline comment
      {
      _treat_buffer(buff, tokens, line_nr, column_nr);
      int nesting = 1;
      ++s; ++column_nr;
      while (*s && nesting > 0)
        {
        if (*s == '\n')
          {
          ++line_nr;
          column_nr = 0;
          }
        if (*s == '(')
          ++nesting;
        if (*s == ')')
          --nesting;
        ++s;
        ++column_nr;
        }
      if (*s)
        {
        ++s;
        ++column_nr;
        }
      break;
      }
      case ':':
      {
      _treat_buffer(buff, tokens, line_nr, column_nr);
      tokens.emplace_back(token::T_COLON, ":", line_nr, column_nr);
      ++s;
      ++column_nr;
      break;
      }
      case ';':
      {
      _treat_buffer(buff, tokens, line_nr, column_nr);
      tokens.emplace_back(token::T_SEMICOLON, ";", line_nr, column_nr);
      ++s;
      ++column_nr;
      break;
      }      
      case '\\':
      {
      _treat_buffer(buff, tokens, line_nr, column_nr);
      while (*s && *s != '\n') // comment, so skip till end of the line
        ++s;
      ++s;
      ++line_nr;
      column_nr = 1;
      break;
      }
      }

    if (s_copy == s)
      {
      buff += *s;
      ++s;
      ++column_nr;
      }

    } // while (s < s_end)
  _treat_buffer(buff, tokens, line_nr, column_nr);

  return tokens;
  }

VF_END
