

char* get_program_command(char* extension) {
  if (strcmp(extension, ".txt") == 0) {
    return "vim $FILE";
  }
  else if (strcmp(extension, ".gz") == 0) {
    return "gunzip $FILE";
  }
  else {
    return NULL;
  }
}