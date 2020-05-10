#pragma once

#include "namespace.h"
#include "vectorforth_api.h"
#include "tokenize.h"
#include <string>
#include <vector>

VF_BEGIN

struct dictionary_entry
  {
  std::string name;
  std::vector<token> words;
  size_t dictionary_position;
  };

typedef std::vector<dictionary_entry> dictionary;

VECTOR_FORTH_API void push(dictionary& d, const dictionary_entry& e);

VECTOR_FORTH_API bool find(dictionary_entry& e, const dictionary& d, const std::string& name);

void register_definition(dictionary& d, std::vector<token>& words);

VF_END