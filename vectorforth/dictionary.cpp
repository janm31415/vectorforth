#include "dictionary.h"

#include "compile_error.h"

#include <cassert>

VF_BEGIN

void push(dictionary& d, const dictionary_entry& e)
  {  
  size_t pos = d.size();
  d.push_back(e);
  d.back().dictionary_position = pos;
  }

bool find(dictionary_entry& e, const dictionary& d, const std::string& name)
  {
  auto rit = d.rbegin();
  auto rit_end = d.rend();
  for (; rit != rit_end; ++rit)
    {
    if (rit->name == name)
      {
      e = *rit;
      return true;
      }
    }
  return false;
  }


void register_definition(dictionary& d, std::vector<token>& words)
  {
  assert(!words.empty());
  assert(words.front().type == token::T_WORD);
  
  dictionary_entry de;
  de.name = words.front().value;

  auto it_end = words.rend() - 1;
  for (auto it = words.rbegin(); it != it_end; ++it)
    {
    if (it->type == token::T_WORD)
      {
      dictionary_entry e;
      if (find(e, d, it->value))
        {
        de.words.insert(de.words.end(), e.words.begin(), e.words.end());
        }
      else
        de.words.emplace_back(token::T_PRIMITIVE, it->value, it->line_nr, it->column_nr);
      }
    else
      de.words.push_back(*it);
    }
  push(d, de);
  }

VF_END