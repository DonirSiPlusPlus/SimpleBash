#ifndef SRC_GREP_GREP_H_  //  SRC_GREP_GREP_H_
#define SRC_GREP_GREP_H_  //  SRC_GREP_GREP_H_
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define max_size 1024

void s21_grep(int argc, char **argv);
void file_search(int argc, int f_pattern);
void ifFewFile(char *filename, int currentArg);
int file_patterns(char **argv, int argc, int file);
void mem_alloc(int argc);
void free_strings(int argc, int f_pattern);

typedef struct grep_string {
  int opt_e, count_keys, count_files;
  short opt_i, opt_v, opt_c, opt_l, opt_n, opt_h, opt_s, opt_f, opt_o;
  char **keywords, **file_args;
} grep_string;

#endif  // SRC_GREP_GREP_H_
