递归可能会爆栈是指，在递归调用的过程中，系统需要在每次递归时保存当前函数的状态（如局部变量、返回地址等）到栈中。当递归的深度过大时，栈的空间可能会被耗尽，导致栈溢出错误。

为了避免递归爆栈，可以使用**循环**来模拟递归的过程，这样可以避免栈空间的耗尽。循环模拟递归意味着你用显式的栈数据结构（如堆栈、队列等）来替代系统栈，从而实现类似递归的行为，但使用的是显式的内存管理。

### 递归 vs 循环模拟

- **递归**：每次函数调用时会将函数调用的上下文（局部变量、返回地址等）压入系统栈中。当递归层级过深时，栈空间耗尽可能导致程序崩溃。
    
- **循环模拟递归**：使用栈（或队列等数据结构）手动管理每次的状态，将每次递归的参数和当前状态存储在栈中，避免直接依赖系统栈。
递归:
// 递归遍历文件夹
void list_directory_recursive(char* path) {
    DIR* dir = opendir(path);
    if (dir == NULL) {
        perror("opendir");
        return;
    }

    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            char full_path[PATH_MAX];
            snprintf(full_path, sizeof(full_path), "%s/%s", path, entry->d_name);
            if (is_directory(full_path)) {
                list_directory_recursive(full_path);  // 递归调用
            }
            // 处理文件
        }
    }

    closedir(dir);
}
用循环模拟递归
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>

typedef struct {
    char* path;
    int index;  // 当前读取到的文件/目录索引
    DIR* dir;
} DirectoryState;

void list_directory_iterative(char* root_path) {
    DirectoryState* stack = malloc(sizeof(DirectoryState) * 100);  // 栈，用于模拟递归
    int stack_size = 0;

    // 初始路径入栈
    stack[stack_size].path = root_path;
    stack[stack_size].dir = opendir(root_path);
    stack[stack_size].index = 0;
    stack_size++;

    while (stack_size > 0) {
        DirectoryState* current_state = &stack[stack_size - 1];

        // 如果目录已经打开
        if (current_state->dir != NULL) {
            struct dirent* entry;
            while ((entry = readdir(current_state->dir)) != NULL) {
                if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
                    // 处理文件或目录
                    printf("%s/%s\n", current_state->path, entry->d_name);

                    char full_path[PATH_MAX];
                    snprintf(full_path, sizeof(full_path), "%s/%s", current_state->path, entry->d_name);

                    if (is_directory(full_path)) {
                        // 如果是目录，将目录信息入栈（模拟递归）
                        stack[stack_size].path = full_path;
                        stack[stack_size].dir = opendir(full_path);
                        stack[stack_size].index = 0;
                        stack_size++;
                    }
                }
            }
            closedir(current_state->dir);
            current_state->dir = NULL;  // 目录已处理完
        }
        
        // 弹出栈顶状态
        stack_size--;
    }

    free(stack);  // 释放栈
}
