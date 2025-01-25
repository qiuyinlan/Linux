#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <time.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/ioctl.h>

// 定义一个结构体来保存文件的信息
typedef struct file_info {
    char *name;             // 文件名
    struct stat file_stat; // 文件状态信息
} file_info_t;

// 用于排序的比较函数
int compare_by_name(const void *a, const void *b) {
    return strcmp(((file_info_t*)a)->name, ((file_info_t*)b)->name);
}

int compare_by_mtime(const void *a, const void *b) {
    return ((file_info_t*)b)->file_stat.st_mtime - ((file_info_t*)a)->file_stat.st_mtime;
}

// 打印文件的详细信息（长格式）
void print_file_details(file_info_t *file_info, int show_size, int show_inode) {
    if (show_inode) {
        printf("%ld ", file_info->file_stat.st_ino);
    }

    printf("%c%c%c%c%c%c%c%c%c ", 
           (S_ISDIR(file_info->file_stat.st_mode)) ? 'd' : '-',
           (file_info->file_stat.st_mode & S_IRUSR) ? 'r' : '-',
           (file_info->file_stat.st_mode & S_IWUSR) ? 'w' : '-',
           (file_info->file_stat.st_mode & S_IXUSR) ? 'x' : '-',
           (file_info->file_stat.st_mode & S_IRGRP) ? 'r' : '-',
           (file_info->file_stat.st_mode & S_IWGRP) ? 'w' : '-',
           (file_info->file_stat.st_mode & S_IXGRP) ? 'x' : '-',
           (file_info->file_stat.st_mode & S_IROTH) ? 'r' : '-',
           (file_info->file_stat.st_mode & S_IWOTH) ? 'w' : '-');

    printf("%d ", (int)file_info->file_stat.st_nlink);
    printf("%s ", getpwuid(file_info->file_stat.st_uid)->pw_name);
    printf("%s ", getgrgid(file_info->file_stat.st_gid)->gr_name);
    printf("%lld ", (long long)file_info->file_stat.st_size);
    printf("%s ", ctime(&file_info->file_stat.st_mtime));
    printf("%s\n", file_info->name);
}

// 列出目录的内容
void list_directory(const char *path, int show_all, int show_long, int show_inode, int sort_by_time) {
    DIR *dir;
    struct dirent *entry;
    file_info_t *files = NULL;
    int num_files = 0;

    dir = opendir(path);
    if (!dir) {
        perror("opendir");
        return;
    }

    // 读取目录中的每一项
    while ((entry = readdir(dir)) != NULL) {
        if (!show_all && entry->d_name[0] == '.') {
            continue; // 跳过隐藏文件（以'.'开头的文件）
        }

        // 统计文件数量
        num_files++;
        files = realloc(files, num_files * sizeof(file_info_t));
        files[num_files - 1].name = strdup(entry->d_name);

        // 获取文件状态
        char file_path[1024];
        snprintf(file_path, sizeof(file_path), "%s/%s", path, entry->d_name);
        if (stat(file_path, &files[num_files - 1].file_stat) == -1) {
            perror("stat");
            continue;
        }
    }

    // 根据时间或名称排序
    if (sort_by_time) {
        qsort(files, num_files, sizeof(file_info_t), compare_by_mtime);
    } else {
        qsort(files, num_files, sizeof(file_info_t), compare_by_name);
    }

    // 打印文件信息
    for (int i = 0; i < num_files; i++) {
        if (show_long) {
            print_file_details(&files[i], 1, show_inode);
        } else {
            printf("%s  ", files[i].name);
        }
        free(files[i].name);
    }
    printf("\n");

    closedir(dir);
    free(files);
}

int main(int argc, char *argv[]) {
    int show_all = 0, show_long = 0, show_inode = 0, sort_by_time = 0;
    int opt;

    // 处理命令行参数
    while ((opt = getopt(argc, argv, "alrti")) != -1) {
        switch (opt) {
            case 'a': show_all = 1; break;
            case 'l': show_long = 1; break;
            case 'r': break; // 反向排序功能可以在排序时加入
            case 't': sort_by_time = 1; break;
            case 'i': show_inode = 1; break;
            default: break;
        }
    }

    // 如果没有指定目录，默认列出当前目录
    if (optind == argc) {
        list_directory(".", show_all, show_long, show_inode, sort_by_time);
    } else {
        // 列出每个指定目录的内容
        for (int i = optind; i < argc; i++) {
            printf("\n%s:\n", argv[i]);
            list_directory(argv[i], show_all, show_long, show_inode, sort_by_time);
        }
    }

    return 0;
}
