#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include <sys/stat.h>
#include <unistd.h>

#include "beargit.h"
#include "util.h"

/* Implementation Notes:
 *
 * - Functions return 0 if successful, 1 if there is an error.
 * - All error conditions in the function description need to be implemented
 *   and written to stderr. We catch some additional errors for you in main.c.
 * - Output to stdout needs to be exactly as specified in the function
 * description.
 * - Only edit this file (beargit.c)
 * - You are given the following helper functions:
 *   * fs_mkdir(dirname): create directory <dirname>
 *   * fs_rm(filename): delete file <filename>
 *   * fs_mv(src,dst): move file <src> to <dst>, overwriting <dst> if it exists
 *   * fs_cp(src,dst): copy file <src> to <dst>, overwriting <dst> if it exists
 *   * write_string_to_file(filename,str): write <str> to filename (overwriting
 * contents)
 *   * read_string_from_file(filename,str,size): read a string of at most <size>
 * (incl. NULL character) from file <filename> and store it into <str>. Note
 * that <str> needs to be large enough to hold that string.
 *  - You NEED to test your code. The autograder we provide does not contain the
 *    full set of tests that we will run on your code. See "Step 5" in the
 * homework spec.
 */

/* beargit init
 *
 * - Create .beargit directory
 * - Create empty .beargit/.index file
 * - Create .beargit/.prev file containing 0..0 commit id
 *
 * Output (to stdout):
 * - None if successful
 */
int beargit_init(void) {
  fs_mkdir(".beargit");

  FILE* findex = fopen(".beargit/.index", "w");
  fclose(findex);

  write_string_to_file(".beargit/.prev",
                       "0000000000000000000000000000000000000000");

  return 0;
}

/* beargit add <filename>
 *
 * - Append filename to list in .beargit/.index if it isn't in there yet
 *
 * Possible errors (to stderr):
 * >> ERROR: File <filename> already added
 *
 * Output (to stdout):
 * - None if successful
 */

int beargit_add(const char* filename) {
  FILE* findex = fopen(".beargit/.index", "r");
  FILE* fnewindex = fopen(".beargit/.newindex", "w");

  char line[FILENAME_SIZE];
  while (fgets(line, sizeof(line), findex)) {
    strtok(line, "\n");
    if (strcmp(line, filename) == 0) {
      fprintf(stderr, "ERROR: File %s already added\n", filename);
      fclose(findex);
      fclose(fnewindex);
      fs_rm(".beargit/.newindex");
      return 3;
    }

    fprintf(fnewindex, "%s\n", line);
  }

  fprintf(fnewindex, "%s\n", filename);
  fclose(findex);
  fclose(fnewindex);

  fs_mv(".beargit/.newindex", ".beargit/.index");

  return 0;
}

int beargit_rm(const char* filename) {
  const char newindex[19] = ".beargit/.newindex";
  const char index[16] = ".beargit/.index";
  FILE* findex = fopen(index, "r");
  FILE* fnewindex = fopen(newindex, "w");
  uint8_t found_file = 0;

  char line[FILENAME_SIZE];
  while (fgets(line, sizeof(line), findex)) {
    strtok(line, "\n");

    if (found_file) {
      fprintf(fnewindex, "%s\n", line);
    } else if (strcmp(line, filename) == 0) {
      found_file = 1;
    } else {
      fprintf(fnewindex, "%s\n", line);
    }
  }

  fclose(findex);
  fclose(fnewindex);

  if (!found_file) {
    fs_rm(newindex);
    fprintf(stderr, "ERROR: File %s not tracked\n", filename);
    return 2;
  }

  fs_mv(newindex, index);
  return 0;
}

const char* go_bears = "GO BEARS!";

int is_commit_msg_ok(const char* msg) {
  const uint8_t plen = 9;
  uint8_t pcount = 0;
  char* c = (char*)msg;

  if (*c && *c == 'G') {
    pcount++;
  }

  while (*c++) {
    if (pcount == plen) {
      return 1;
    }

    if (*c == *(go_bears + pcount)) {
      pcount++;
    } else {
      pcount = 0;
    }
  }

  if (pcount == plen) {
    return 1;
  }

  return 0;
}

int commit_char_to_int(char c) {
  switch (c) {
    case '6':
      return 0;
    case '1':
      return 1;
    case 'c':
      return 2;
    default:
      return 0;
  }
}

char commit_int_to_char(int i) {
  switch (i) {
    case 0:
      return '6';
    case 1:
      return '1';
    case 2:
      return 'c';
    default:
      return '6';
  }
}

void next_commit_id(const char* commit_id, char* new_commit_id) {
  uint8_t carry = 1;

  for (int i = COMMIT_ID_BYTES - 1; i > -1; i--) {
    int value = commit_char_to_int(*(commit_id + i));
    value += carry;
    if (value > 2) {
      value = 0;
    } else {
      carry = 0;
    }

    *(new_commit_id + i) = commit_int_to_char(value);
  }

  *(new_commit_id + COMMIT_ID_BYTES) = '\0';
}

int beargit_commit(const char* msg) {
  if (!is_commit_msg_ok(msg)) {
    fprintf(stderr, "ERROR: Message must contain \"%s\"\n", go_bears);
    return 1;
  }

  char* prev = ".beargit/.prev";
  char* index = ".beargit/.index";
  char* msg_file = ".msg";

  char commit_id[COMMIT_ID_SIZE];
  read_string_from_file(prev, commit_id, COMMIT_ID_SIZE);
  char next_id[COMMIT_ID_SIZE];
  next_commit_id(commit_id, next_id);

  char new_dir[9 + COMMIT_ID_SIZE];
  sprintf(new_dir, ".beargit/%s", next_id);
  fs_mkdir(new_dir);

  char new_prev[strlen(prev) + COMMIT_ID_SIZE + 1];
  sprintf(new_prev, ".beargit/%s/.prev", next_id);
  fs_cp(prev, new_prev);

  char new_index[strlen(index) + COMMIT_ID_SIZE + 1];
  sprintf(new_index, ".beargit/%s/.index", next_id);
  fs_cp(index, new_index);

  char new_msg[strlen(msg_file) + COMMIT_ID_SIZE + 10];
  sprintf(new_msg, ".beargit/%s/%s", next_id, msg_file);
  write_string_to_file(new_msg, msg);

  FILE* findex = fopen(".beargit/.index", "r");
  char line[FILENAME_SIZE];
  while (fgets(line, sizeof(line), findex)) {
    strtok(line, "\n");
    char new_file[strlen(line) + COMMIT_ID_SIZE + 10];
    sprintf(new_file, ".beargit/%s/%s", next_id, line);
    fs_cp(line, new_file);
  }

  fclose(findex);
  write_string_to_file(prev, next_id);
  return 0;
}

int beargit_status() {
  FILE* findex = fopen(".beargit/.index", "r");
  char line[FILENAME_SIZE];
  int count = 0;

  fprintf(stdout, "Tracked files:\n\n");

  while (fgets(line, sizeof(line), findex)) {
    strtok(line, "\n");
    fprintf(stdout, "  %s\n", line);
    count++;
  }

  if (count == 1) {
    fprintf(stdout, "\n%d file total\n", count);
  } else {
    fprintf(stdout, "\n%d files total\n", count);
  }

  fclose(findex);
  return 0;
}

/* beargit log
 *
 * See "Step 4" in the homework 1 spec.
 *
 */
int beargit_log() {
  /* COMPLETE THE REST */

  return 0;
}
