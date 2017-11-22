/***************************************************************************************************/
/*          Copyright (C) 2015-2017 By Yang Chen (yngccc@gmail.com). All Rights Reserved.          */
/***************************************************************************************************/

#include "platform_windows.cpp"

void skip_non_token(char **at) {
  while (true) {
    if ((*at)[0] == '\0') {
      break;
    }
    else if (isspace((*at)[0])) {
      (*at) += 1;
    }
    else if ((*at)[0] == '/' && (*at)[1] == '/') {
      (*at) += 2;
      while ((*at)[0] != '\0' && (*at)[0] != '\n') {
        (*at) += 1;
      }
    }
    else if ((*at)[0] == '/' && (*at)[1] == '*') {
      (*at) += 2;
      while ((*at)[0] != '\0' && !((*at)[0] == '*' && (*at)[1] == '/')) {
        (*at) += 1;
      }
      if ((*at)[0] == '*') {
        (*at) += 2;
      }
    }
    else {
      break;
    }
  }
}

enum token_type {
  token_lparen,
  token_rparen,
  token_lbrace,
  token_rbrace,
  token_lbracket,
  token_rbracket,
  token_colon,
  token_semicolon,
  token_asterisk,

  token_string,
  token_identifier,

  token_eof,
  token_unknown
};

struct token {
  token_type type;
  char *text;
  uint32 text_len;
};

token get_token(char **at) {
  skip_non_token(at);

  token token = {};
  token.type = token_unknown;
  token.text = *at;
  token.text_len = 1;

  char c = (*at)[0];
  (*at) += 1;
  switch (c) {
    case '(' : {
      token.type = token_lparen;
    } break;
    case ')' : {
      token.type = token_rparen;
    } break;
    case '{' : {
      token.type = token_lbrace;
    } break;
    case '}' : {
      token.type = token_rbrace;
    } break;
    case '[' : {
      token.type = token_lbracket;
    } break;
    case ']' : {
      token.type = token_rbracket;
    } break;
    case ':' : {
      token.type = token_colon;
    } break;
    case ';' : {
      token.type = token_semicolon;
    } break;
    case '*' : {
      token.type = token_asterisk;
    } break;
    case '"' : {
      token.type = token_string;
      token.text = *at;
      while ((*at)[0] != '\0' && (*at)[0] != '"') {
        if ((*at)[0] == '\\' && (*at)[1] != '\0') {
          (*at) += 1;
        }
        (*at) += 1;
      }
      token.text_len = (uint32)(*at - token.text);
      if ((*at)[0] == '"') {
        (*at) += 1;
      }
    } break;
    case '\0' : {
      token.type = token_eof;
      (*at) -= 1;
    } break;
    default : {
      if (isalpha(c)) {
        token.type = token_identifier;
        while (isalnum((*at)[0]) || (*at)[0] == '_') {
          (*at) += 1;
        }
        token.text_len = (uint32)((*at) - token.text);
      }
      else {
        token.type = token_unknown;
      }
    }
  }
  return token;
}

bool token_equal(token token, char *text) {
  if (strlen(text) != token.text_len) {
    return false;
  }
	for (uint32 i = 0; i < token.text_len; i += 1) {
		if (token.text[i] != text[i]) {
			return false;
		}
	}
	return true;
}

struct enum_info {
  string enum_name;
  string member_names[128];
  uint32 member_names_count;
};

struct struct_info {
};

struct parse_result {
  enum_info enum_infos[1024];
  uint32 enum_infos_count;
  struct_info struct_infos[1024];
  uint32 struct_infos_count;
};

void parse_enum(char **code, char *enum_name, uint32 enum_name_len, parse_result *parse_result) {
  m_assert(parse_result->enum_infos_count < m_countof(parse_result->enum_infos));
  enum_info *enum_info = &parse_result->enum_infos[parse_result->enum_infos_count++];
  enum_info->enum_name = {enum_name, enum_name_len, enum_name_len};

  for (;;) {
    token token = get_token(code);
    m_assert(token.type != token_eof);
    if (token.type == token_rbrace) {
      break;
    }
    if (token.type == token_identifier) {
      m_assert(enum_info->member_names_count < m_countof(enum_info->member_names));
      string *member_name = &enum_info->member_names[enum_info->member_names_count++];
      *member_name = {token.text, token.text_len, token.text_len};
    }
  }
}

void parse_struct(char **code, char *struct_name, uint32 struct_name_len, parse_result *parse_result) {
}

void parse(char *code, parse_result *parse_result) {
  bool parsing = true;
  while (parsing) {
    token token = get_token(&code);
    switch (token.type) {
      case token_identifier : {
        if (token_equal(token, "enum")) {
          token = get_token(&code);
          if (token.type == token_identifier) {
            char *enum_name = token.text;
            uint32 enum_name_len = token.text_len;
            token = get_token(&code);
            if (token_equal(token, "{")) {
              parse_enum(&code, enum_name, enum_name_len, parse_result);
            }
          }
        }
        else if (token_equal(token, "struct")) {
          token = get_token(&code);
          if (token.type == token_identifier) {
            char *struct_name = token.text;
            uint32 struct_name_len = token.text_len;
            token = get_token(&code);
            if (token_equal(token, "{")) {
              parse_struct(&code, struct_name, struct_name_len, parse_result);
            }
          }
        }
      } break;
      case token_eof : {
        parsing = false;
      } break;
    }
  }
}

void print_parse_result(parse_result *parse_result) {
	for (uint32 i = 0; i < parse_result->enum_infos_count; i += 1) {
    enum_info *enum_info = &parse_result->enum_infos[i];
    printf("%.*s\n", (int)enum_info->enum_name.len, enum_info->enum_name.buf);
    for (uint32 i = 0; i < enum_info->member_names_count; i += 1) {
      string *name = &enum_info->member_names[i];
      printf("  %.*s\n", (int)name->len, name->buf);
    }
  }
}

void append_parse_result(parse_result *parse_result, string *code_gen_src) {
  for (uint32 i = 0; i < parse_result->enum_infos_count; i += 1) {
    enum_info *enum_info = &parse_result->enum_infos[i];
    string_catf(code_gen_src, "const char* enum_to_str(enum %.*s e) {\n  switch (e) {\n", enum_info->enum_name.len, enum_info->enum_name.buf);
    for (uint32 i = 0; i < enum_info->member_names_count; i += 1) {
      string *name = &enum_info->member_names[i];
      string_catf(code_gen_src, "    case %.*s : return \"%.*s\";\n", name->len, name->buf, name->len, name->buf);
    }
    string_catf(code_gen_src, "    default : return \"INVALID_ENUM_VALUE\";\n");
    string_catf(code_gen_src, "  }\n}\n\n");
  }  
}

int main(int argc, char **argv) {
  set_exe_dir_as_current();

  string code_gen_src = {};
  code_gen_src.capacity = m_megabytes(4);
  code_gen_src.buf = (char *)malloc(code_gen_src.capacity);
  {
    strcpy(code_gen_src.buf, "/*** Generated Code ***/\n\n");
    code_gen_src.len = (uint32)strlen(code_gen_src.buf);
  }

  for (uint32 i = 1; i < (uint32)(argc - 1); i += 1) {
    file_mapping src_file_mapping = {};
    open_file_mapping(argv[i], &src_file_mapping);
    m_assert(src_file_mapping.size < m_megabytes(2));
    char *src = (char *)malloc(m_megabytes(2));
    memcpy(src, src_file_mapping.ptr, src_file_mapping.size);
    src[src_file_mapping.size] = '\0';
    close_file_mapping(src_file_mapping);
    parse_result *parse_result = (struct parse_result *)calloc(sizeof(struct parse_result), 1);
    parse(src, parse_result);
    // print_parse_result(parse_result);
    append_parse_result(parse_result, &code_gen_src);
    free(src);
    free(parse_result);
  }

  const char *codegen_file_name = "..\\src\\codegen.gen.cpp";
  file_mapping code_gen_file_mapping = {};
  create_file_mapping(codegen_file_name, code_gen_src.len, &code_gen_file_mapping);
  memcpy(code_gen_file_mapping.ptr, code_gen_src.buf, code_gen_src.len);
  close_file_mapping(code_gen_file_mapping);
} 