#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>

#define MAX_LINE   256
#define MAX_NAME   64
#define MAX_PERM   10
#define USERS_DB   "users.db"
#define PERMS_DB   "file_perms.db"
#define AUDIT_LOG  "audit.log"

typedef struct
{
    char username[MAX_NAME];
    char group[MAX_NAME];
} Session;

// SECTION a: AUDIT LOGGING 

void audit_log(const char *username, const char *action, const char *target)
{
    FILE *fp = fopen(AUDIT_LOG, "a");

    if (fp == NULL)
    {
        perror("audit_log");
        return;
    }

    time_t now = time(NULL);
    struct tm *current = localtime(&now);

    if (current == NULL)
    {
        fclose(fp);
        return;
    }

    fprintf(fp,
            "%04d-%02d-%02d %02d:%02d:%02d | User: %-10s | Action: %-15s | Target: %s\n",
            current->tm_year + 1900,
            current->tm_mon + 1,
            current->tm_mday,
            current->tm_hour,
            current->tm_min,
            current->tm_sec,
            username,
            action,
            target);

    fclose(fp);
}

// SECTION b: PASSWORD HASHING (djb2)

unsigned long djb2_hash(const char *str)
{
    unsigned long hash = 5381;
    int c;

    while ((c = *str++))
    {
        hash = ((hash << 5) + hash) + (unsigned long)c;
    }

    return hash;
}
// SECTION C: USER AUTHENTICATION


void ensure_default_users(void)
{
    FILE *fp = fopen(USERS_DB, "r");

    if (fp != NULL)
    {
        fclose(fp);
        return;
    }

    fp = fopen(USERS_DB, "w");

    if (fp == NULL)
    {
        perror("ensure_default_users");
        return;
    }

    fprintf(fp, "admin:%lu:staff\n", djb2_hash("admin123"));
    fprintf(fp, "guest:%lu:guest\n", djb2_hash("guest123"));

    fclose(fp);

    printf("[INFO] Default accounts created -> admin/admin123 (group: staff), "
           "guest/guest123 (group: guest)\n");
}

/* Returns 1 and fills session->group on success, 0 on failure. */
int authenticate(const char *username, const char *password, Session *session)
{
    FILE *fp = fopen(USERS_DB, "r");

    if (fp == NULL)
    {
        perror("authenticate");
        return 0;
    }

    char line[MAX_LINE];
    unsigned long entered_hash = djb2_hash(password);

    while (fgets(line, sizeof(line), fp) != NULL)
    {
        char db_user[MAX_NAME];
        unsigned long db_hash;
        char db_group[MAX_NAME];

        line[strcspn(line, "\n")] = '\0';

        if (sscanf(line, "%63[^:]:%lu:%63[^:\n]", db_user, &db_hash, db_group) != 3)
        {
            continue;
        }

        if (strcmp(db_user, username) == 0 && db_hash == entered_hash)
        {
            strncpy(session->username, username, MAX_NAME - 1);
            strncpy(session->group, db_group, MAX_NAME - 1);
            fclose(fp);
            audit_log(username, "LOGIN_SUCCESS", "-");
            return 1;
        }
    }

    fclose(fp);
    audit_log(username, "LOGIN_FAILED", "-");
    return 0;
}

int main(void)
{
    printf("================================================\n");
    printf(" Task 3 - Secure File Management System\n");
    printf("================================================\n\n");

    ensure_default_users();

    audit_log("system", "PROGRAM_START", "task3");

    printf("User authentication module implemented.\n");

    return 0;
}
