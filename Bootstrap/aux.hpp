#pragma once
#include <cstdio>
#include <cstdlib>

#define LOG(msg, ...) std::printf(msg, __VA_ARGS__)
#define EXIT(code) std::exit(code)

#include <cassert>
#define ASSERT(arg, msg) assert((arg) && msg) 
#define STATIC_ASSERT(arg, msg) static_assert((arg) && msg)

#define VAR_ERR_TMPL template<class...ARGS> void 

namespace __std {
  // Structs
    struct ParsedArgs {
      char* output_name = NULL; // ISO C++ Forbids converting a string constant to char*, https://gcc.gnu.org/onlinedocs/gcc/Warning-Options.html#index-Wwrite-strings 
      FILE** files = NULL; 
      size_t files_len = 0;
      char** link_targets = NULL;
      size_t link_targets_len = 0;
      char** link_paths = NULL;
      size_t link_paths_len = 0;
      char** include_paths = NULL; 
      size_t include_paths_len = 0;
      bool output_intermediate_output = false;
      bool static_library = false;
      bool dynamic_library = false;
      ParsedArgs() = default;
      ParsedArgs(ParsedArgs&&) = default;
      ParsedArgs(const ParsedArgs&) = default;
      ~ParsedArgs() {
        for (int i = 0; this->files[i] != NULL; ++i) std::fclose(files[i]);
        std::free(files);
        std::free(link_targets);  // all the pointers point to a argv
        std::free(link_paths);    // all the pointers point to a argv
        std::free(include_paths); // all the pointers point to a argv
      }
    };
  // Error Handling
    enum STATUS_CODE {
      INVAL_TOK = -4,
      INVAL_FILE = -3,
      INVAL_ARG = -2,
      UNDEF = -1,
      GOOD = 0,
      NOT_IMPL = 1
    };
    VAR_ERR_TMPL err_default(STATUS_CODE exit_code, const char* msg, ARGS... args) { LOG(msg, args...); EXIT(exit_code); }
    void err_notimpl(const char* msg) { err_default(STATUS_CODE::NOT_IMPL, "NotImplementedYet: %s \n", msg); }
    void err_argparse(const char* appended_msg, const char* arg) { err_default(STATUS_CODE::INVAL_ARG,"Error parsing Arguments: %s %s \n", appended_msg, arg); }
    void err_invalfile(const char* file_name) { err_default(STATUS_CODE::INVAL_FILE,"Invalid File: %s \n", file_name); }
    VAR_ERR_TMPL err_lexing(const char* custom, ARGS... args) { err_default(STATUS_CODE::INVAL_TOK, custom, args...); }
  // tokens parsing
    // Auxiliary
      char push(FILE* f) { 
        char x = fgetc(f);  
        if (x == EOF) err_lexing("Unexpected End Of File \n");
        return x;
      }
      char rpush(FILE* f) {
        return fgetc(f);  
      }
      char cpush(FILE* f) {
        char output;
        return (output = fgetc(f)) == EOF ? '\0' : output;  
      }
    // The Z Family (Normal)
      void parse_comment(FILE* f) { // should handle EOF in '//'
        char input = push(f);
        if      (input == '/') while (push(f) != '\n');
        else if (input == '*') while (push(f) != '*' || push(f) != '/');
        else err_lexing("Invalid Token after '/' expecting '/' or '*', got: %c \n", input);
      }
};