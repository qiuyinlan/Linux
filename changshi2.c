#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>

#define MAX_PATH 1024
#define MAX_FILES 1024

// Option flags
int show_all = 0, show_long = 0, recursive = 0;
int reverse_order = 0, sort_by_time = 0, show_inode = 0, show_blocks = 0;

// Struct to store file info
typedef struct {
    struct dirent dir_entry;
    struct stat file_stat;
} FileInfo;

// Function prototypes
void list_directory(const char *path);
void display_files(FileInfo *files, int count);
int compare_by_name(const void *a, const void *b);
int compare_by_time(const void *a, const void *b);
void print_file_info(FileInfo *file);
void parse_options(int argc, char **argv);

int main(int argc, char **argv) {
    parse_options(argc, argv);

    if (optind == argc) {
        // If no directory is specified, use the current directory
        list_directory(".");
    } else {
        // Process each specified directory or file
        for (int i = optind; i < argc; i++) {
            printf("%s:\n", argv[i]);
            list_directory(argv[i]);
            if (i < argc - 1) {
                printf("\n");
            }
        }
    }

    return 0;
}

void parse_options(int argc, char **argv) {
    int opt;
    while ((opt = getopt(argc, argv, "alRtris")) != -1) {
        switch (opt) {
            case 'a': show_all = 1; break;
            case 'l': show_long = 1; break;
            case 'R': recursive = 1; break;
            case 'r': reverse_order = 1; break;
            case 't': sort_by_time = 1; break;
            case 'i': show_inode = 1; break;
            case 's': show_blocks = 1; break;
            default:
                fprintf(stderr, "Usage: %s [-alRtris] [file...]\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }
}

void list_directory(const char *path) {
    DIR *dir = opendir(path);
    if (!dir) {
        perror("opendir");
        return;
    }

    FileInfo files[MAX_FILES];
    int file_count = 0;

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (!show_all && entry->d_name[0] == '.') {
            continue; // Skip hidden files unless -a is specified
        }

        FileInfo *file = &files[file_count++];
        file->dir_entry = *entry;

        char full_path[MAX_PATH];
        snprintf(full_path, MAX_PATH, "%s/%s", path, entry->d_name);
        if (stat(full_path, &file->file_stat) == -1) {
            perror("stat");
            file_count--;
        }

        if (file_count >= MAX_FILES) {
            fprintf(stderr, "Too many files in directory: %s\n", path);
            break;
        }
    }

    closedir(dir);

    // Sort files
    qsort(files, file_count, sizeof(FileInfo),sort_by_time ? compare_by_time : compare_by_name);
    if (reverse_order) {
        for (int i = 0; i < file_count / 2; i++) {
            FileInfo temp = files[i];
            files[i] = files[file_count - 1 - i];
            files[file_count - 1 - i] = temp;
        }
    }

    // Display files
    display_files(files, file_count);

    // Handle recursion
    if (recursive) {
        for (int i = 0; i < file_count; i++) {
            if (S_ISDIR(files[i].file_stat.st_mode) &&
                strcmp(files[i].dir_entry.d_name, ".") != 0 &&
                strcmp(files[i].dir_entry.d_name, "..") != 0) {

                char next_path[MAX_PATH];
                snprintf(next_path, MAX_PATH, "%s/%s", path, files[i].dir_entry.d_name);
                printf("\n%s:\n", next_path);
                list_directory(next_path);
            }
        }
    }
}

void display_files(FileInfo *files, int count) {
    for (int i = 0; i < count; i++) {
        print_file_info(&files[i]);
    }
}

int compare_by_name(const void *a, const void *b) {
    FileInfo *file1 = (FileInfo *)a;
    FileInfo *file2 = (FileInfo *)b;
    return strcmp(file1->dir_entry.d_name, file2->dir_entry.d_name);
}

int compare_by_time(const void *a, const void *b) {
    FileInfo *file1 = (FileInfo *)a;
    FileInfo *file2 = (FileInfo *)b;
    return file2->file_stat.st_mtime - file1->file_stat.st_mtime;
}

void print_file_info(FileInfo *file) {
    if (show_inode) {
        printf("%lu ", file->dir_entry.d_ino);
    }

    if (show_blocks) {
        printf("%lu ", file->file_stat.st_blocks / 2);
    }

    if (show_long) {
        printf((S_ISDIR(file->file_stat.st_mode)) ? "d" : "-");
        printf((file->file_stat.st_mode & S_IRUSR) ? "r" : "-");
        printf((file->file_stat.st_mode & S_IWUSR) ? "w" : "-");
        printf((file->file_stat.st_mode & S_IXUSR) ? "x" : "-");
        printf((file->file_stat.st_mode & S_IRGRP) ? "r" : "-");
        printf((file->file_stat.st_mode & S_IWGRP) ? "w" : "-");
        printf((file->file_stat.st_mode & S_IXGRP) ? "x" : "-");
        printf((file->file_stat.st_mode & S_IROTH) ? "r" : "-");
        printf((file->file_stat.st_mode & S_IWOTH) ? "w" : "-");
        printf((file->file_stat.st_mode & S_IXOTH) ? "x" : "-");

        printf(" %lu", file->file_stat.st_nlink);
        printf(" %lu", file->file_stat.st_size);

        char time_str[20];
        strftime(time_str, sizeof(time_str), "%b %d %H:%M", localtime(&file->file_stat.st_mtime));
        printf(" %s", time_str);
    }

    printf("      %s", file->dir_entry.d_name);
}
