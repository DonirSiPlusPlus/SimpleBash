#include "grep.h"

grep_string pars_string = {.opt_i = 0,
                           .opt_v = 0,
                           .opt_c = 0,
                           .opt_l = 0,
                           .opt_n = 0,
                           .opt_h = 0,
                           .opt_s = 0,
                           .opt_f = 0,
                           .opt_o = 0,
                           .count_keys = 0,
                           .count_files = 0,
                           .keywords = NULL,
                           .file_args = NULL};

void s21_grep(int argc, char **argv) {
  short error = 0;
  for (int i = 1; i < argc; i++) {
    if (argv[i][0] == '-') {
      switch (argv[i][1]) {
        case 'e':
          if (i + 1 >= argc) {
            fprintf(stderr, "grep: option requires an argument -- e");
            error = 1;
            break;
          }
          pars_string.opt_e++;
          break;
        case 'i':
          pars_string.opt_i = 1;
          break;
        case 'v':
          pars_string.opt_v = 1;
          break;
        case 'c':
          pars_string.opt_c = 1;
          break;
        case 'l':
          pars_string.opt_l = 1;
          break;
        case 'n':
          pars_string.opt_n = 1;
          break;
        case 'h':
          pars_string.opt_h = 1;
          break;
        case 's':
          pars_string.opt_s = 1;
          break;
        case 'f':
          if (i + 1 >= argc) {
            fprintf(stderr, "grep: option requires an argument -- f");
            error = 1;
            break;
          }
          pars_string.opt_f = 1;
          break;
        case 'o':
          pars_string.opt_o = 1;
          break;
        case '?':
        default:
          fprintf(stderr, "grep: option requires an argument -- %c\n",
                  argv[i][1]);
          error = 1;
          break;
      }
      if (error) break;
    }
  }
  if (!error && argc > 1) {
    short first_word = 0;
    mem_alloc(argc);
    int f_pattern = 0;
    for (int i = 1; i < argc; i++) {
      if (pars_string.opt_e > 0 || pars_string.opt_f > 0) {
        if (argv[i][0] == '-' && i + 1 < argc) {
          if (argv[i][1] == 'e') {
            strcpy(pars_string.keywords[pars_string.count_keys], argv[++i]);
            pars_string.count_keys++;
          } else if (argv[i][1] == 'f') {
            f_pattern += file_patterns(argv, argc, ++i);
          }
        } else if (argv[i][0] != '-') {
          strcpy(pars_string.file_args[pars_string.count_files], argv[i]);
          pars_string.count_files++;
        }
      } else {
        if (argv[i][0] != '-') {
          if (!first_word) {
            strcpy(pars_string.keywords[0], argv[i]);
            first_word = 1;
            pars_string.count_keys++;
          } else {
            strcpy(pars_string.file_args[pars_string.count_files], argv[i]);
            pars_string.count_files++;
          }
        }
      }
    }
    file_search(argc, f_pattern);
  }
}

void file_search(int argc, int f_pattern) {
  char filename[max_size] = "";
  short compil_err = 0;
  for (int i = 0; i < pars_string.count_files; i++) {
    FILE *my_file = NULL;
    if ((my_file = fopen(pars_string.file_args[i], "r")) == NULL) {
      fprintf(stderr, "grep: %s: No such file or directory\n",
              pars_string.file_args[i]);
    } else {
      if (!pars_string.opt_h) ifFewFile(filename, i);
      int count_strings = 0, current_str = 0;
      while (!feof(my_file)) {
        char strFile[max_size] = "";
        fgets(strFile, max_size, my_file);
        current_str++;
        short was_break = 0, l_break = 0, v_find = 0;
        for (int j = 0; j < pars_string.count_keys; j++) {
          regex_t preg;
          if (!regcomp(&preg, pars_string.keywords[j],
                       REG_EXTENDED | REG_NEWLINE)) {
            regmatch_t pm;
            if (pars_string.opt_i) {
              regfree(&preg);
              int compil = regcomp(&preg, pars_string.keywords[j],
                                   REG_EXTENDED | REG_NEWLINE | REG_ICASE);
              int find = regexec(&preg, strFile, 1, &pm, 0);
              regfree(&preg);
              if (!compil && !find) {
                printf("%s%s", filename, strFile);
                was_break = 1;
                break;
              }
              continue;
            }
            if (strlen(strFile) != 0) {
              int result = regexec(&preg, strFile, 1, &pm, 0);
              regfree(&preg);
              if (result) {
                if (pars_string.opt_v) {
                  v_find++;
                  if (j == pars_string.count_keys - 1 &&
                      v_find == pars_string.count_keys) {
                    printf("%s%s", filename, strFile);
                    if (strFile[strlen(strFile) - 1] != '\n') puts("");
                  }
                }
              } else if (!result && !pars_string.opt_v) {
                if (pars_string.opt_c) {
                  count_strings++;
                  was_break = 1;
                  break;
                }
                if (pars_string.opt_l) {
                  printf("%s\n", pars_string.file_args[i]);
                  l_break = 1;
                  break;
                }
                if (pars_string.opt_n) {
                  printf("%s%d:%s", filename, current_str, strFile);
                  was_break = 1;
                  break;
                }
                if (pars_string.opt_o) {
                  printf("%s%s\n", filename, pars_string.keywords[j]);
                  char *str = strFile + pm.rm_eo;
                  regfree(&preg);
                  if (!regcomp(&preg, pars_string.keywords[j],
                               REG_EXTENDED | REG_NEWLINE)) {
                    while (!regexec(&preg, str, 1, &pm, 0)) {
                      printf("%s\n", pars_string.keywords[j]);
                      str += pm.rm_eo;
                      regfree(&preg);
                      if (regcomp(&preg, pars_string.keywords[j],
                                  REG_EXTENDED | REG_NEWLINE))
                        break;
                    }
                  }
                  regfree(&preg);
                  was_break = 1;
                  break;
                }
                printf("%s%s", filename, strFile);
                was_break = 1;
                break;
              }
            } else {
              regfree(&preg);
            }
          } else {
            regfree(&preg);
            compil_err = 1;
            fprintf(stderr, "Regular expression compilation error");
            break;
          }
        }
        if (was_break || l_break || compil_err) {
          if (strFile[strlen(strFile) - 1] != '\n') puts("");
          if (l_break || compil_err) {
            break;
          } else {
            continue;
          }
        }
      }
      fclose(my_file);
      if (compil_err) break;
      if (pars_string.opt_c) printf("%s%d\n", filename, count_strings);
    }
  }
  free_strings(argc, f_pattern);
}
/*
вставка имени файла в вывод, если указано несколько файлов
*/
void ifFewFile(char *filename, int currentArg) {
  if (pars_string.count_files > 1) {
    strcpy(filename, pars_string.file_args[currentArg]);
    strcat(filename, ":");
  }
}
/*
обработка флага -f и его файла
*/
int file_patterns(char **argv, int argc, int file) {
  FILE *patterns = NULL;
  int counter = 0;
  if ((patterns = (fopen(argv[file], "r"))) == NULL) {
    if (!pars_string.opt_s)
      fprintf(stderr, "grep: %s: No such file or directory\n", argv[file]);
  } else {
    char pattern[max_size] = "";
    while (fgets(pattern, max_size, patterns)) {
      if (pars_string.count_keys >= argc) {
        pars_string.keywords = (char **)realloc(
            pars_string.keywords, sizeof(char *) * (argc + counter + 1));
        pars_string.keywords[argc + counter++] =
            (char *)malloc(max_size * sizeof(char));
      }
      int len = strlen(pattern);
      if (len > 0) {
        if (pattern[len - 1] == '\n') {
          pattern[len - 1] = '\0';
          strcpy(pars_string.keywords[pars_string.count_keys++], pattern);
        } else {
          strcpy(pars_string.keywords[pars_string.count_keys++], pattern);
        }
      }
    }
    fclose(patterns);
  }
  return counter;
}

void mem_alloc(int argc) {
  pars_string.keywords = (char **)malloc(argc * sizeof(char *));
  pars_string.file_args = (char **)malloc(argc * sizeof(char *));
  for (int i = 0; i < argc; i++) {
    pars_string.keywords[i] = (char *)malloc(max_size * sizeof(char));
    pars_string.file_args[i] = (char *)malloc(max_size * sizeof(char));
  }
}

void free_strings(int argc, int f_pattern) {
  for (int i = 0; i < argc + f_pattern; i++) free(pars_string.keywords[i]);
  for (int i = 0; i < argc; i++) free(pars_string.file_args[i]);
  free(pars_string.keywords);
  pars_string.keywords = NULL;
  free(pars_string.file_args);
  pars_string.file_args = NULL;
}
