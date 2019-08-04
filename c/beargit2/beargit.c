#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include <unistd.h>
#include <sys/stat.h>

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

  FILE* fbranches = fopen(".beargit/.branches", "w");
  fprintf(fbranches, "%s\n", "master");
  fclose(fbranches);
   
  write_string_to_file(".beargit/.prev", "0000000000000000000000000000000000000000");
  write_string_to_file(".beargit/.current_branch", "master");

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
  FILE *fnewindex = fopen(".beargit/.newindex", "w");

  char line[FILENAME_SIZE];
  while(fgets(line, sizeof(line), findex)) {
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

void next_commit_id_hw1(char* commit_id, char* new_commit_id) {
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

int beargit_commit_hw1(const char* msg) {
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
  next_commit_id_hw1(commit_id, next_id);

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

// ---------------------------------------
// HOMEWORK 2 
// ---------------------------------------

// This adds a check to beargit_commit that ensures we are on the HEAD of a branch.
int beargit_commit(const char* msg) {
  char current_branch[BRANCHNAME_SIZE];
  read_string_from_file(".beargit/.current_branch", current_branch, BRANCHNAME_SIZE);

  if (strlen(current_branch) == 0) {
    fprintf(stderr, "ERROR: Need to be on HEAD of a branch to commit\n");
    return 1;
  }

  return beargit_commit_hw1(msg);
}

const char* digits = "61c";

void next_commit_id(char* commit_id) {
  char current_branch[BRANCHNAME_SIZE];
  read_string_from_file(".beargit/.current_branch", current_branch, BRANCHNAME_SIZE);

  // The first COMMIT_ID_BRANCH_BYTES=10 characters of the commit ID will
  // be used to encode the current branch number. This is necessary to avoid
  // duplicate IDs in different branches, as they can have the same pre-
  // decessor (so next_commit_id has to depend on something else).
  int n = get_branch_number(current_branch);
  for (int i = 0; i < COMMIT_ID_BRANCH_BYTES; i++) {
    // This translates the branch number into base 3 and substitutes 0 for
    // 6, 1 for 1 and c for 2.
    commit_id[i] = digits[n%3];
    n /= 3;
  }

  // Use next_commit_id to fill in the rest of the commit ID.
  char next_id[COMMIT_ID_SIZE];
  next_commit_id_hw1(commit_id + COMMIT_ID_BRANCH_BYTES, next_id);
  strcpy(commit_id + COMMIT_ID_BRANCH_BYTES, next_id);
}


// This helper function returns the branch number for a specific branch, or
// returns -1 if the branch does not exist.
int get_branch_number(const char* branch_name) {
  FILE* fbranches = fopen(".beargit/.branches", "r");

  int branch_index = -1;
  int counter = 0;
  char line[FILENAME_SIZE];
  while(fgets(line, sizeof(line), fbranches)) {
    strtok(line, "\n");
    if (strcmp(line, branch_name) == 0) {
      branch_index = counter;
    }
    counter++;
  }

  fclose(fbranches);

  return branch_index;
}

/* beargit branch
 *
 * See "Step 6" in the homework 2 spec.
 *
 */

int beargit_branch() {
  /* COMPLETE THE REST */

  return 0;
}

/* beargit checkout
 *
 * See "Step 7" in the homework 2 spec.
 *
 */

int checkout_commit(const char* commit_id) {
  /* COMPLETE THE REST */
  return 0;
}

int is_it_a_commit_id(const char* commit_id) {
  /* COMPLETE THE REST */
  return 1;
}

int beargit_checkout(const char* arg, int new_branch) {
  // Get the current branch
  char current_branch[BRANCHNAME_SIZE];
  read_string_from_file(".beargit/.current_branch", "current_branch", BRANCHNAME_SIZE);

  // If not detached, update the current branch by storing the current HEAD into that branch's file...
  // Even if we cancel later, this is still ok.
  if (strlen(current_branch)) {
    char current_branch_file[BRANCHNAME_SIZE+50];
    sprintf(current_branch_file, ".beargit/.branch_%s", current_branch);
    fs_cp(".beargit/.prev", current_branch_file);
  }

  // Check whether the argument is a commit ID. If yes, we just stay in detached mode
  // without actually having to change into any other branch.
  if (is_it_a_commit_id(arg)) {
    char commit_dir[FILENAME_SIZE] = ".beargit/";
    strcat(commit_dir, arg);
    if (!fs_check_dir_exists(commit_dir)) {
      fprintf(stderr, "ERROR: Commit %s does not exist\n", arg);
      return 1;
    }

    // Set the current branch to none (i.e., detached).
    write_string_to_file(".beargit/.current_branch", "");

    return checkout_commit(arg);
  }

  // Just a better name, since we now know the argument is a branch name.
  const char* branch_name = arg;

  // Read branches file (giving us the HEAD commit id for that branch).
  int branch_exists = (get_branch_number(branch_name) >= 0);

  // Check for errors.
  if (!(!branch_exists || !new_branch)) {
    fprintf(stderr, "ERROR: A branch named %s already exists\n", branch_name);
    return 1;
  } else if (!branch_exists && new_branch) {
    fprintf(stderr, "ERROR: No branch %s exists\n", branch_name);
    return 1;
  }

  // File for the branch we are changing into.
  char* branch_file = ".beargit/.branch_"; 
  strcat(branch_file, branch_name);

  // Update the branch file if new branch is created (now it can't go wrong anymore)
  if (new_branch) {
    FILE* fbranches = fopen(".beargit/.branches", "a");
    fprintf(fbranches, "%s\n", branch_name);
    fclose(fbranches);
    fs_cp(".beargit/.prev", branch_file); 
  }

  write_string_to_file(".beargit/.current_branch", branch_name);

  // Read the head commit ID of this branch.
  char branch_head_commit_id[COMMIT_ID_SIZE];
  read_string_from_file(branch_file, branch_head_commit_id, COMMIT_ID_SIZE);

  // Check out the actual commit.
  return checkout_commit(branch_head_commit_id);
}
