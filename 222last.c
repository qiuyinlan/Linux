#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <dirent.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <stdbool.h>
#include <errno.h>

#define MAX_NAME_LEN 256

// 定义文件信息结构体
typedef struct {
    char name[MAX_NAME_LEN];
    ino_t inode;
    off_t size;
    time_t mtime;
    mode_t mode;
    uid_t uid;
    gid_t gid;
    nlink_t nlink;
    blkcnt_t blocks;
} FileInfo;

// 错误处理函数
void handle_error(const char *message) {
    perror(message);
    exit(EXIT_FAILURE);
}

// 比较文件名
int compare_name(const void *a, const void *b) {
    const FileInfo *fileA = (const FileInfo *)a;
    const FileInfo *fileB = (const FileInfo *)b;
    return strcmp(fileA->name, fileB->name);
}

// 比较文件修改时间
int compare_time(const void *a, const void *b) {
    const FileInfo *fileA = (const FileInfo *)a;
    const FileInfo *fileB = (const FileInfo *)b;
    return (fileA->mtime > fileB->mtime) ? -1 : (fileA->mtime < fileB->mtime) ? 1 : 0;
}

// 反转数组
void reverse_array(FileInfo *array, int count) {
    for (int i = 0; i < count / 2; i++) {
        FileInfo temp = array[i];
        array[i] = array[count - 1 - i];
        array[count - 1 - i] = temp;
    }
}

// 打印文件权限
void print_permissions(mode_t mode) {
    printf((mode & S_IRUSR) ? "r" : "-");
    printf((mode & S_IWUSR) ? "w" : "-");
    printf((mode & S_IXUSR) ? "x" : "-");
    printf((mode & S_IRGRP) ? "r" : "-");
    printf((mode & S_IWGRP) ? "w" : "-");
    printf((mode & S_IXGRP) ? "x" : "-");
    printf((mode & S_IROTH) ? "r" : "-");
    printf((mode & S_IWOTH) ? "w" : "-");
    printf((mode & S_IXOTH) ? "x" : "-");
}

// 打印文件类型
void print_file_type(mode_t mode) {
    if (S_ISDIR(mode)) printf("d");
    else if (S_ISLNK(mode)) printf("l");
    else if (S_ISREG(mode)) printf("-");
    else if (S_ISCHR(mode)) printf("c");
    else if (S_ISBLK(mode)) printf("b");
    else if (S_ISFIFO(mode)) printf("p");
    else printf("?");
}

// 打印文件信息
void print_file_info(const FileInfo *file, bool show_inode, bool show_blocks, bool long_format, int max_link_len, int max_size_len, int max_blocks_len) {
    if (long_format) {
        if (show_inode) printf("%ld ", file->inode);
        if (show_blocks) printf("%*ld ", max_blocks_len, file->blocks);
        print_file_type(file->mode);
        print_permissions(file->mode);
        printf(" %*ld", max_link_len, file->nlink);

        struct passwd *pwd = getpwuid(file->uid);
        struct group *grp = getgrgid(file->gid);
        printf(" %s %s", pwd->pw_name, grp->gr_name);

        printf(" %*ld", max_size_len, file->size);

        char time_buff[30];
        strftime(time_buff, sizeof(time_buff), "%m月 %d %H:%M", localtime(&file->mtime));
        printf(" %s %s\n", time_buff, file->name);
    } else {
        if (show_inode) printf("%ld ", file->inode);
        if (show_blocks) printf("%*ld ", max_blocks_len, file->blocks);
        printf("%s     ", file->name);
    }
}

// 列出目录内容
void list_directory(const char *dir_path, bool show_hidden, bool long_format, bool recursive, bool sort_by_time, bool reverse_order, bool show_inode, bool show_blocks) {
    if (!dir_path) return;

    if (recursive) {
        printf("\n%s\n", dir_path);
    }

    DIR *dir = opendir(dir_path);
    if (!dir) {
        if (errno == EACCES) {
            printf("ls: 无法访问 '%s': 权限不够\n", dir_path);
        } else if (errno == ENOENT) {
            printf("ls: 无法访问 '%s': 没有那个文件或目录\n", dir_path);
        } else {
            perror("opendir 失败");
        }
        return;
    }

    int capacity = 1024;
    FileInfo *files = malloc(sizeof(FileInfo) * capacity);
    if (!files) handle_error("malloc error");

    int file_count = 0;
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (file_count >= capacity) {
            capacity *= 2;
            files = realloc(files, sizeof(FileInfo) * capacity);
            if (!files) handle_error("realloc error");
        }

        if (!show_hidden && entry->d_name[0] == '.') continue;

        char file_path[1024];
        snprintf(file_path, sizeof(file_path), "%s/%s", dir_path, entry->d_name);

        struct stat file_stat;
        if (lstat(file_path, &file_stat) == -1) {
            perror("stat error");
            continue;
        }

        strcpy(files[file_count].name, entry->d_name);
        files[file_count].inode = file_stat.st_ino;
        files[file_count].size = file_stat.st_size;
        files[file_count].mtime = file_stat.st_mtime;
        files[file_count].mode = file_stat.st_mode;
        files[file_count].uid = file_stat.st_uid;
        files[file_count].gid = file_stat.st_gid;
        files[file_count].nlink = file_stat.st_nlink;
        files[file_count].blocks = file_stat.st_blocks;
        file_count++;
    }

    qsort(files, file_count, sizeof(FileInfo), sort_by_time ? compare_time : compare_name);
    if (reverse_order) reverse_array(files, file_count);

    int max_link = 0, max_size = 0, max_blocks = 0;
    for (int i = 0; i < file_count; i++) {
        if (files[i].nlink > max_link) max_link = files[i].nlink;
        if (files[i].size > max_size) max_size = files[i].size;
        if (files[i].blocks > max_blocks) max_blocks = files[i].blocks;
    }

    int max_link_len = snprintf(NULL, 0, "%d", max_link);
    int max_size_len = snprintf(NULL, 0, "%d", max_size);
    int max_blocks_len = snprintf(NULL, 0, "%d", max_blocks);

    for (int i = 0; i < file_count; i++) {
        print_file_info(&files[i], show_inode, show_blocks, long_format, max_link_len, max_size_len, max_blocks_len);
    }

    if (!long_format) printf("\n");

    int dir_count = 0;
    for (int i = 0; i < file_count; i++) {
        if (S_ISDIR(files[i].mode) && strcmp(files[i].name, ".") != 0 && strcmp(files[i].name, "..") != 0) dir_count++;
    }

    char **dir_names = malloc(dir_count * sizeof(char *));
    for (int i = 0, j = 0; i < file_count; i++) {
        if (S_ISDIR(files[i].mode) && strcmp(files[i].name, ".") != 0 && strcmp(files[i].name, "..") != 0) {
            dir_names[j] = malloc(MAX_NAME_LEN);
            strcpy(dir_names[j++], files[i].name);
        }
    }

    free(files);

    if (recursive) {
        for (int i = 0; i < dir_count; i++) {
            char new_dir_path[1024];
            snprintf(new_dir_path, sizeof(new_dir_path), "%s/%s", dir_path, dir_names[i]);
            list_directory(new_dir_path, show_hidden, long_format, recursive, sort_by_time, reverse_order, show_inode, show_blocks);
        }
    }

    free(dir_names);
    closedir(dir);
}

int main(int argc, char *argv[]) {
    int opt;
    bool show_hidden = false, long_format = false, recursive = false, sort_by_time = false, reverse_order = false, show_inode = false, show_blocks = false;
    char *path = ".";

    while ((opt = getopt(argc, argv, "alRtris")) != -1) {
        switch (opt) {
            case 'a': show_hidden = true; break;
            case 'l': long_format = true; break;
            case 'R': recursive = true; break;
            case 't': sort_by_time = true; break;
            case 'r': reverse_order = true; break;
            case 'i': show_inode = true; break;
            case 's': show_blocks = true; break;
            default: fprintf(stderr, "Usage: %s [-alRtris] [directory...]\n", argv[0]); exit(EXIT_FAILURE);
        }
    }

    if (optind < argc) {
        path = argv[optind];
        if (!recursive) printf("%s:\n", path);
    }

    list_directory(path, show_hidden, long_format, recursive, sort_by_time, reverse_order, show_inode, show_blocks);

    return 0;
}