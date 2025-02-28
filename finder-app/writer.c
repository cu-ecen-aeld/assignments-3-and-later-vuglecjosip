#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>

int main(int argc, char *argv[]) {
    // Open syslog
    openlog("writer", LOG_PID | LOG_CONS, LOG_USER);

    if (argc != 3) {
        syslog(LOG_ERR, "Error: You must provide both the file path and text string.");
        fprintf(stderr, "Error: You must provide both the file path and text string.\n");
        closelog();
        return 1;
    }

    char *writefile = argv[1];
    char *writestr = argv[2];

    // Write string to file
    FILE *file = fopen(writefile, "w");
    if (file == NULL) {
        syslog(LOG_ERR, "Error: Failed to write to %s.", writefile);
        fprintf(stderr, "Error: Failed to write to %s.\n", writefile);
        closelog();
        return 1;
    }

    fprintf(file, "%s", writestr);
    syslog(LOG_DEBUG, "Writing '%s' to '%s'", writestr, writefile);
    fclose(file);

    printf("File %s has been created/overwritten with the provided content.\n", writefile);

    // Close syslog
    closelog();

    return 0;
}
