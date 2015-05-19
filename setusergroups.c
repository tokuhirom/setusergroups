#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <grp.h>

static void set_supp_groups(const char* username) {
  int ngroups=0;
  int capacity=100;
  gid_t* gidset = malloc(sizeof(gid_t)*capacity);
  if (gidset == NULL) {
      printf("Out of memory\n");
      exit(1);
  }

  while (1) {
    struct group* grent = getgrent();
    if (grent == NULL) {
      break;
    }

    char **gr_mem = grent->gr_mem;
    while (*gr_mem) {
      if (strcmp(*gr_mem, username) == 0) {
        if (ngroups+1 > capacity) {
          capacity = capacity * 2;

          gidset = realloc(gidset, sizeof(gid_t)*capacity);
          if (gidset == NULL) {
              printf("Out of memory\n");
              exit(1);
          }
        }

        gidset[ngroups++] = grent->gr_gid;
        break;
      }
      gr_mem++;
    }
  }

  if (setgroups(ngroups, gidset) != 0) {
    perror("setgroups failed");
    exit(1);
  }

  free(gidset);
}

int main(int argc, char** argv) {
  if (argc <= 2) {
    printf("Usage: %s account child\n", argv[0]);
    return 1;
  }

  char* name = argv[1];

  struct passwd *pwnam = getpwnam(name);
  if (pwnam == NULL) {
    perror("unknown user");
    return 1;
  }

  if (setgid(pwnam->pw_gid) != 0) {
    perror("setgid failed");
    return 1;
  }

  set_supp_groups(name);

  if (setuid(pwnam->pw_uid) != 0) {
    perror("setuid failed");
    return 1;
  }

  execvp(argv[2], &(argv[2]));
}
