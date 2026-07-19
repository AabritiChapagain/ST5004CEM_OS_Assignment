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

Session currentUser;
void createDefaultUsers();
int login();
void createFile();
void writeFile();
void readFile();
void deleteFile();
//authentication
void createDefaultUsers()
{
    FILE *fp = fopen("users.db", "r");

    if (fp != NULL)
    {
        fclose(fp);
        return;
    }

    fp = fopen("users.db", "w");

    if (fp == NULL)
    {
        printf("Unable to create users database.\n");
        return;
    }

    fprintf(fp, "admin,admin123,staff\n");
    fprintf(fp, "aabriti,aabriti123,staff\n");
    fprintf(fp, "billa,billa123,guest\n");

    fclose(fp);

    printf("Default users created.\n");
}
int login()
{
    char username[MAX_NAME];
    char password[MAX_NAME];

    printf("\nLogin\n");

    printf("Username: ");
    scanf("%63s", username);

    printf("Password: ");
    scanf("%63s", password);

    FILE *fp = fopen("users.db", "r");

    if (fp == NULL)
    {
        printf("Cannot open users database.\n");
        return 0;
    }

    char fileUser[MAX_NAME];
    char filePass[MAX_NAME];
    char fileGroup[MAX_NAME];
while (fscanf(fp,
              " %63[^,],%63[^,],%63[^\n]",
              fileUser,
              filePass,
              fileGroup) == 3)
{
    if (strcmp(username, fileUser) == 0 &&
        strcmp(password, filePass) == 0)
    {
        strcpy(currentUser.username, fileUser);
        strcpy(currentUser.group, fileGroup);

        fclose(fp);

        printf("\nLogin Successful!\n");
        printf("Welcome %s\n", currentUser.username);

        return 1;
    }
}
    fclose(fp);

    printf("\nInvalid username or password.\n");

    return 0;
}
void createFile()
{
    char filename[100];

    printf("\nEnter file name: ");
    scanf("%99s", filename);

    FILE *fp = fopen(filename, "w");

    if (fp == NULL)
    {
        printf("Unable to create file.\n");
        return;
    }

    fclose(fp);

    printf("File created successfully.\n");
}
void writeFile()
{
    char filename[100];
    char text[500];

    printf("\nEnter file name: ");
    scanf("%99s", filename);

    getchar();

    printf("Enter text: ");
    fgets(text, sizeof(text), stdin);

    FILE *fp = fopen(filename, "a");

    if (fp == NULL)
    {
        printf("Unable to open file.\n");
        return;
    }

    fprintf(fp, "%s", text);

    fclose(fp);

    printf("Data written successfully.\n");
}
void readFile()
{
    char filename[100];
    char line[500];

    printf("\nEnter file name: ");
    scanf("%99s", filename);

    FILE *fp = fopen(filename, "r");

    if (fp == NULL)
    {
        printf("Unable to open file.\n");
        return;
    }

    printf("\n----- File Content -----\n");

    while (fgets(line, sizeof(line), fp) != NULL)
    {
        printf("%s", line);
    }

    fclose(fp);
}
void deleteFile()
{
    char filename[100];

    printf("\nEnter file name: ");
    scanf("%99s", filename);

    if (remove(filename) == 0)
    {
        printf("File deleted successfully.\n");
    }
    else
    {
        printf("Unable to delete file.\n");
    }
}

int main()
{
    printf("__________________________________\n");
    printf(" Secure File Management System\n");
    printf("__________________________________\n\n");

    printf("Project initialized successfully.\n");

 createDefaultUsers();

if (!login())
{
    return 1;
}
printf("\nAuthentication module loaded successfully.\n");

createFile();
writeFile();
readFile();
deleteFile();
return 0;
}
