/*
 * ls.c - Vedant
 * List directory contents for the kvtd shell.
 * Dispatched via ops.c -> execvp("./ls", args)
 *
 * Flags:
 *   -l   detailed view (permissions, links, owner, group, size, timestamp)
 *   -a   include hidden dot-entries
 *   -v   verbose mode (prints which directory is being listed)
 *
 * Usage: ls [-l] [-a] [-v] [path ...]
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>

/* ------------------------------------------------------------------ */
/* Config struct — mirrors the style used in touch.c / mkdir.c        */
/* ------------------------------------------------------------------ */
typedef struct {
    int show_hidden;   /* -a */
    int detailed;      /* -l */
    int verbosity;     /* -v */
} LSConfig;

/* ------------------------------------------------------------------ */
/* Build 10-char permission string into caller-supplied buffer         */
/* ------------------------------------------------------------------ */
static void build_mode_str(mode_t m, char buf[11])
{
    buf[0] = S_ISDIR(m) ? 'd' : S_ISLNK(m) ? 'l' : '-';

    buf[1] = (m & S_IRUSR) ? 'r' : '-';
    buf[2] = (m & S_IWUSR) ? 'w' : '-';
    buf[3] = (m & S_IXUSR) ? 'x' : '-';

    buf[4] = (m & S_IRGRP) ? 'r' : '-';
    buf[5] = (m & S_IWGRP) ? 'w' : '-';
    buf[6] = (m & S_IXGRP) ? 'x' : '-';

    buf[7] = (m & S_IROTH) ? 'r' : '-';
    buf[8] = (m & S_IWOTH) ? 'w' : '-';
    buf[9] = (m & S_IXOTH) ? 'x' : '-';

    buf[10] = '\0';
}

/* ------------------------------------------------------------------ */
/* uid -> username, fallback to numeric                                */
/* ------------------------------------------------------------------ */
static void uid_to_name(uid_t uid, char *out, size_t outlen)
{
    struct passwd *record = getpwuid(uid);
    if (record != NULL)
        snprintf(out, outlen, "%s", record->pw_name);
    else
        snprintf(out, outlen, "%d", (int)uid);
}

/* ------------------------------------------------------------------ */
/* gid -> group name, fallback to numeric                              */
/* ------------------------------------------------------------------ */
static void gid_to_name(gid_t gid, char *out, size_t outlen)
{
    struct group *record = getgrgid(gid);
    if (record != NULL)
        snprintf(out, outlen, "%s", record->gr_name);
    else
        snprintf(out, outlen, "%d", (int)gid);
}

/* ------------------------------------------------------------------ */
/* Print one entry in -l (detailed) format                             */
/* ------------------------------------------------------------------ */
static void show_detailed(const char *base_path, const char *entry_name)
{
    char full[1024];
    snprintf(full, sizeof(full), "%s/%s", base_path, entry_name);

    struct stat info;
    if (lstat(full, &info) != 0) {
        fprintf(stderr, "ls: cannot stat '%s'\n", entry_name);
        return;
    }

    char mode_str[11];
    build_mode_str(info.st_mode, mode_str);

    char owner[64], grp[64];
    uid_to_name(info.st_uid, owner, sizeof(owner));
    gid_to_name(info.st_gid, grp,   sizeof(grp));

    char when[20];
    struct tm *t = localtime(&info.st_mtime);
    strftime(when, sizeof(when), "%b %e %H:%M", t);

    printf("%s %2ld %-8s %-8s %6lld %s %s\n",
           mode_str,
           (long)info.st_nlink,
           owner, grp,
           (long long)info.st_size,
           when,
           entry_name);
}

/* ------------------------------------------------------------------ */
/* Returns 1 if entry should be skipped                                */
/* ------------------------------------------------------------------ */
static int should_skip(const char *name, int show_hidden)
{
    return (!show_hidden && name[0] == '.');
}

/* ------------------------------------------------------------------ */
/* List a single directory                                             */
/* ------------------------------------------------------------------ */
static int list_directory(const char *path, const LSConfig *cfg)
{
    DIR *folder = opendir(path);
    if (folder == NULL) {
        fprintf(stderr, "ls: cannot access '%s': ", path);
        perror("");
        return EXIT_FAILURE;
    }

    if (cfg->verbosity)
        printf("ls: listing '%s'\n", path);

    struct dirent *item;
    while ((item = readdir(folder)) != NULL) {
        if (should_skip(item->d_name, cfg->show_hidden))
            continue;

        if (cfg->detailed)
            show_detailed(path, item->d_name);
        else
            puts(item->d_name);
    }

    closedir(folder);
    return EXIT_SUCCESS;
}

/* ------------------------------------------------------------------ */
/* Entry point                                                         */
/* ------------------------------------------------------------------ */
int main(int argc, char *argv[])
{
    LSConfig cfg = { 0, 0, 0 };

    int ch;
    while ((ch = getopt(argc, argv, "lav")) != -1) {
        switch (ch) {
            case 'l': cfg.detailed    = 1; break;
            case 'a': cfg.show_hidden = 1; break;
            case 'v': cfg.verbosity   = 1; break;
            default:
                fprintf(stderr, "Usage: %s [-l] [-a] [-v] [path ...]\n", argv[0]);
                return EXIT_FAILURE;
        }
    }

    /* No path given — default to current directory */
    if (optind >= argc)
        return list_directory(".", &cfg);

    /* Support multiple paths: ls dir1 dir2 */
    int exit_status = EXIT_SUCCESS;
    for (int i = optind; i < argc; i++) {
        if (list_directory(argv[i], &cfg) != EXIT_SUCCESS)
            exit_status = EXIT_FAILURE;
    }

    return exit_status;
}
