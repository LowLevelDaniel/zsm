#pragma once
// #include "linked_list.hpp"
#include "aux.hpp"
#include <cstring>

namespace __std {
  // struct
    struct PreProcessor {
      // LinkedList<char*> macro_idents;
      // LinkedList<char*> macro_defenitions;
      // size_t macros_len = 0;
      PreProcessor() = default;
      ~PreProcessor() = default;
    }; 
  // define && ifdef && ifndef && else && endif
    void parse_macro_define(FILE* stream) {}
    void parse_macro_ifdef(FILE* stream) {}
    void parse_macro_ifndef(FILE* stream) {}
    void parse_macro_else(FILE* stream) {}
    void parse_macro_endif(FILE* stream) {}
  // include  // (LEARNING MORE ABOUT HOW IT WORKS WITHOUT)
  // link     // add file to link targets (searchs link paths)
  void parse_macro(FILE* stream, PreProcessor& macro_parser) {
    char input = push(stream); // get rid of '#'
    char buffer[10];
    unsigned vals_len = 0;
    while (input != ' ') {
      if (vals_len == 10) err_lexing("garbage after #, did you forget a space? \n");
      buffer[vals_len] = input;
      ++vals_len;
      input = push(stream);
    }
    buffer[vals_len] = '\0';

    if (strcmp(buffer, "if") == 0) {
      err_notimpl("#if is not implemented yet");
    } else if (strcmp(buffer, "elif") == 0) {
      err_notimpl("#if is not implemented yet");
    } else if (strcmp(buffer, "else") == 0) {
      parse_macro_else(stream);
    } else if (strcmp(buffer, "ifdef") == 0) {
      parse_macro_ifdef(stream);
    } else if (strcmp(buffer, "ifndef") == 0) {
      parse_macro_ifndef(stream);
    } else if (strcmp(buffer, "endif") == 0) {
      parse_macro_endif(stream);
    } else if (strcmp(buffer, "define") == 0) {
      parse_macro_define(stream);
    } else {
      err_lexing("Invalid Macro: %s \n", buffer);
    }
    while (input != '\n') {
      if (input == '\\') { push(stream); push(stream); } // get rid of '\' and '\n'
      push(stream);
    }
  }
  void parse_macro_deref(FILE* stream, PreProcessor& macro_parser) {
    char input = push(stream);
    // deref by $MACRO or ${MACRO}
    if (input != '{') {
      while (push(stream) != '}');
    } else {
      while (push(stream) != ' ');
    }
  }

};