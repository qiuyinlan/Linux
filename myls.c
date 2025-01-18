#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>

void ls(const char *path, int show_all, int show_details, int recursive, int sort_time, int reverse, int show_inode, int show_size);
void print_details(const char *path);
void print_inode(const char *path);
void recursive_ls(const char *path, int show_all, int show_details, int show_inode, int show_size);

struct file_info {
    char name[256];
    time_t mtime;
};

int compare_time(const void *a, const void *b) {
    struct file_info *file_a = (struct file_info *)a;
    struct file_info *file_b = (struct file_info *)b;
    return (file_a->mtime - file_b->mtime);
}

int compare_time_reverse(const void *a, const void *b) {
    struct file_info *file_a = (struct file_info *)a;
    struct file_info *file_b = (struct file_info *)b;
    return (file_b->mtime - file_a->mtime);
}

int main(int argc, char *argv[]) {
    int show_all = 0, show_details = 0, recursive = 0, sort_time = 0, reverse = 0, show_inode = 0, show_size = 0;
    const char *path = ".";

    // 解析命令行参数
    for (int i = 1; i < argc; i++) {
        if (argv[i][0] == '-') {  // 处理选项
            for (int j = 1; argv[i][j] != '\0'; j++) {
                switch (argv[i][j]) {
                    case 'a': show_all = 1; break;
                    case 'l': show_details = 1; break;
                    case 'R': recursive = 1; break;
                    case 't': sort_time = 1; break;
                    case 'r': reverse = 1; break;
                    case 'i': show_inode = 1; break;
                    case 's': show_size = 1; break;
                    default: printf("Unknown option: %c\n", argv[i][j]); break;
                }
            }
        } else {  // 如果是路径
            path = argv[i];
        }
    }

    ls(path, show_all, show_details, recursive, sort_time, reverse, show_inode, show_size);
    return 0;
}

void ls(const char *path, int show_all, int show_details, int recursive, int sort_time, int reverse, int show_inode, int show_size) {
    DIR *dir;
    struct dirent *entry;
    struct stat file_stat;
    struct file_info files[1024];
    int file_count = 0;

    // 打开目录
    if ((dir = opendir(path)) == NULL) {
        perror("opendir");
        return;
    }

    // 读取目录内容
    while ((entry = readdir(dir)) != NULL) {
        if (!show_all && entry->d_name[0] == '.') continue;  // 如果不显示隐藏文件，跳过以 '.' 开头的文件

        // 获取文件信息
        if (stat(entry->d_name, &file_stat) == -1) {
            perror("stat");
            continue;
        }

        // 保存文件信息
        strncpy(files[file_count].name, entry->d_name, sizeof(files[file_count].name));
        files[file_count].mtime = file_stat.st_mtime;
        file_count++;
    }

    if (sort_time) {
        // 按时间排序
        if (reverse) {
            qsort(files, file_count, sizeof(struct file_info), compare_time_reverse);
        } else {
            qsort(files, file_count, sizeof(struct file_info), compare_time);
        }
    }

    // 输出文件信息
    for (int i = 0; i < file_count; i++) {
        if (show_details) {
            print_details(files[i].name);  // 显示文件详细信息
        }
        if (show_inode) {
            print_inode(files[i].name);  // 显示文件的 inode
        }
        if (show_size) {
            printf("Size: %ld bytes\n", file_stat.st_size);  // 显示文件大小
        }
        printf("%s\n", files[i].name);  // 打印文件名
    }

    closedir(dir);  // 关闭目录
}

void print_details(const char *path) {
    struct stat file_stat;

    if (stat(path, &file_stat) == -1) {
        perror("stat");
        return;
    }

    // 显示文件权限
    printf("Permissions: ");
    printf((S_ISDIR(file_stat.st_mode)) ? "d" : "-");
    printf((file_stat.st_mode & S_IRUSR) ? "r" : "-");
    printf((file_stat.st_mode & S_IWUSR) ? "w" : "-");
    printf((file_stat.st_mode & S_IXUSR) ? "x" : "-");
    printf((file_stat.st_mode & S_IRGRP) ? "r" : "-");
    printf((file_stat.st_mode & S_IWGRP) ? "w" : "-");
    printf((file_stat.st_mode & S_IXGRP) ? "x" : "-");
    printf((file_stat.st_mode & S_IROTH) ? "r" : "-");
    printf((file_stat.st_mode & S_IWOTH) ? "w" : "-");
    printf((file_stat.st_mode & S_IXOTH) ? "x" : "-");

    // 显示文件大小
    printf(" Size: %ld bytes\n", file_stat.st_size);
}

void print_inode(const char *path) {
    struct stat file_stat;

    if (stat(path, &file_stat) == -1) {
        perror("stat");
        return;
    }

    // 打印 inode
    printf("Inode: %ld\n", file_stat.st_ino);
}

void recursive_ls(const char *path, int show_all, int show_details, int show_inode, int show_size) {
    DIR *dir;
    struct dirent *entry;
    struct stat file_stat;

    if ((dir = opendir(path)) == NULL) {
        perror("opendir");
        return;
    }

    while ((entry = readdir(dir)) != NULL) {
        if (!show_all && entry->d_name[0] == '.') continue;

        char new_path[1024];
        snprintf(new_path, sizeof(new_path), "%s/%s", path, entry->d_name);

        if (stat(new_path, &file_stat) == -1) {
            perror("stat");
            continue;
        }

        if (S_ISDIR(file_stat.st_mode)) {
            printf("\nDirectory: %s\n", new_path);
            recursive_ls(new_path, show_all, show_details, show_inode, show_size);  // 递归调用
        } else {
            printf("%s\n", new_path);  // 不是目录时，直接打印文件名
        }
    }

    closedir(dir);
}
