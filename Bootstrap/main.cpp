#include "aux.hpp"
#include "macros.hpp"
#include <string>

// anything after '__' is specific to the compiler (in this case it shows this this is the compiler std) 
namespace __std {
  // Binary Construction
  // Tokens
    enum TokenID {
      VOID = 0
    };
    struct TokenRoot { 
      static constexpr TokenID id = TokenID::VOID; 
    };
  // token parsing 
    // Normal
      void parse_label(FILE* stream, std::string& ident) {
        while (push(stream) != ')');
        while (push(stream) != '{'); // allow optional spacing i.e label () {}
        while (push(stream) != '}');
        ident = "";
      }
    // END
    TokenRoot* token_parse_single(FILE* stream, PreProcessor& macro_parser) {
      std::string left_over;
      char input;
      while ((input = cpush(stream)) != '\0') {
        switch (input) {
          case ' ':
          case ';':
          case '\n': break;
          case '/': parse_comment(stream); break;
          case '#': parse_macro(stream, macro_parser); break;
          case '$': parse_macro_deref(stream, macro_parser); break;
          case '(': parse_label(stream, left_over); break;
          default:
            left_over += input;
        }
      }
      return NULL;
    }
    TokenRoot** token_parse(ParsedArgs& pargs, PreProcessor& macro_parser) { // parse into tokens (no actual assembling takes place (this function can be used by other compilers))
      TokenRoot** output = (TokenRoot**)malloc(sizeof(TokenRoot*) * pargs.files_len);
      for (unsigned i = 0; i < pargs.files_len; ++i) 
        output[i] = token_parse_single(pargs.files[i], macro_parser);
      return output;
    }
  // argument parsing
    ParsedArgs arg_parse(int argc, char** argv) {
      ParsedArgs pargs;
      int files = 0;
      int link_targets = 0;
      int link_paths = 0;
      int include_paths = 0;
      for (int i = 1; i < argc; ++i) {
        if (argv[i][0] == '-') {
          int next_index = 1;
          if (argv[i][1] == '-') { next_index = 2; }
          switch (argv[i][next_index]) {
            case 'h':
              LOG(
                "Welcome To The ZSM Help Menu, the current compiler options are \n"
                "  -c  (output intermediate object files) \n"
                "  -l* (add a link target) \n"
                "  -L* (add a link path) \n"
                "  -I* (add a include path) \n"
                "  -s  (create a static library) \n"
                "  -d  (create a dynamic library) \n", NULL
              );
            case 'c': pargs.output_intermediate_output = true; break;
            case 'l': ++link_targets;break;
            case 'L': ++link_paths;break;
            case 'I': ++include_paths;break;
            case 's': pargs.static_library=true;break;
            case 'd': pargs.dynamic_library=true;break;
            default: err_argparse("Invalid Argument:", argv[i]);
          }
        } else ++files;
      }
      if (pargs.static_library && pargs.dynamic_library) err_argparse("target can't be both static library and dynamic library", "");
      pargs.files = (FILE**)malloc(sizeof(FILE*) * files);
      if (link_targets  != 0) pargs.link_targets  = (char**)malloc(sizeof(char*) * link_targets );
      if (link_paths    != 0) pargs.link_paths    = (char**)malloc(sizeof(char*) * link_paths   );
      if (include_paths != 0) pargs.include_paths = (char**)malloc(sizeof(char*) * include_paths);
      
      pargs.files_len = files;
      pargs.link_targets_len = link_targets;
      pargs.link_paths_len = link_paths;
      pargs.include_paths_len = include_paths;

      for (int i = 1; i < argc; ++i) {
        if (argv[i][0] == '-') {
          switch(argv[i][1]) {
            case 'l': // link
              --link_targets;
              pargs.link_targets[link_targets] = argv[i];
              break;
            case 'L': // Add Link Path
              --link_paths;
              pargs.link_paths[link_paths] = argv[i];
              break;
            case 'I': // Add Include Path
              --include_paths;
              pargs.include_paths[include_paths] = argv[i];
              break;
          }
        } else {
          --files;
          FILE* f = std::fopen(argv[i], "r");
          if (f==NULL) err_invalfile(argv[i]);
          pargs.files[files] = f;
        }
      }

      return pargs;
    }
  // END
  void zembel(int argc, char** argv) {
    auto args   = arg_parse  (argc, argv);
    PreProcessor pre_processor;
    auto tokens = token_parse(args,pre_processor);
  }
};

int main(int argc, char** argv) {
  __std::zembel(argc, argv);
} 