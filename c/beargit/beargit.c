#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include <sys/stat.h>
#include <unistd.h>

#include "beargit.h"
#include "util.h"

static const char* INIT_COMMIT = "0000000000000000000000000000000000000000";

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

  write_string_to_file(".beargit/.prev", INIT_COMMIT);

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

int beargit_log() {
  char* msg_file = ".msg";
  char* prev_file = ".prev";
  char last_commit_id[COMMIT_ID_SIZE];
  read_string_from_file(".beargit/.prev", last_commit_id, COMMIT_ID_SIZE);

  if (!strcmp(last_commit_id, INIT_COMMIT)) {
      fprintf(stderr, "ERROR: There are no commits!\n");
      return 1;
  }

  char *current_commit_id = last_commit_id;
  puts("");

  while (strcmp(current_commit_id, INIT_COMMIT)) {
      char current_msg_file[strlen(msg_file) + COMMIT_ID_SIZE + 10];
      sprintf(current_msg_file, ".beargit/%s/%s", current_commit_id, msg_file);

      char current_msg[MSG_SIZE];
      read_string_from_file(current_msg_file, current_msg, MSG_SIZE);
      fprintf(stdout, "commit %s\n    %s\n\n", current_commit_id, current_msg);

      char current_prev_file[strlen(prev_file) + COMMIT_ID_SIZE + 10];
      sprintf(current_prev_file, ".beargit/%s/%s", current_commit_id, prev_file);
      read_string_from_file(current_prev_file, current_commit_id, COMMIT_ID_SIZE);
  }

  return 0;
}
