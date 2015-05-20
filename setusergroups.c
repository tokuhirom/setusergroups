#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <grp.h>

int main(int argc, char** argv) {
  if (argc <= 2) {
    printf("Usage: %s account child\n", argv[0]);
    return 111;
  }

  char* account = argv[1];

  struct passwd *pwnam = getpwnam(account);
  if (pwnam == NULL) {
    perror("unknown user");
    return 111;
  }

  if (setgid(pwnam->pw_gid) != 0) {
    perror("setgid failed");
    return 111;
  }

  if (initgroups(account, pwnam->pw_gid) != 0) {
    perror("initgroups failed");
    return 111;
  }

  if (setuid(pwnam->pw_uid) != 0) {
    perror("setuid failed");
    return 111;
  }

  if (execvp(argv[2], &(argv[2])) == -1) {
    perror("exec failed");
    return 111;
  }
}
