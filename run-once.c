#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <getopt.h>
#include <stdio.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char **argv) {
    // Parse opts
    while (1) {
        static struct option long_options[] = {
            {0, 0, 0, 0}
        };
        int c = getopt_long(argc, argv, "", long_options, NULL);

        if (c == -1) {
            break;
        }

        switch (c) {
            default:
                exit(2);
        }
    }

    // Check positional arg count
    if (argc - optind < 1) {
        fprintf(stderr, "Too few arguments\n");
        exit(2);
    }
    if (argc - optind > 1) {
        fprintf(stderr, "Too many arguments\n");
        exit(2);
    }

    char *exec_path = realpath(argv[optind], NULL);

    if (!exec_path) {
        fprintf(stderr, "Error resolving %s\n", argv[optind]);
        exit(2);
    }

    chdir(getenv("HOME"));

    if (access(exec_path, X_OK)) {
        fprintf(stderr, "Error finding or cannot execute %s\n", exec_path);
        exit(2);
    }

    int fd = open(exec_path, O_RDONLY);

    if (fd == -1) {
        fprintf(stderr, "Error opening %s\n", exec_path);
        exit(2);
    }

    if(flock(fd, LOCK_EX | LOCK_NB)){
        if (errno == EWOULDBLOCK) {
            printf("Lock already held. Program already running?");
            exit(1);
        } else {
            fprintf(stderr, "Error locking %s\n", exec_path);
            exit(2);
        }
    }

    execl(exec_path, exec_path, (char *)NULL);
    fprintf(stderr, "Error running %s\n", exec_path);
    exit(2);
}

