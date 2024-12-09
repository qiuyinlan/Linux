#include <stdio.h>

int main() {
    int n, i, j;
    int l[100];      // 存储每个数字的长度
    int arr[100];    // 存储每个数字
    scanf("%d", &n);

    // 输入处理
    for (i = 0; i < n; i++) {
        scanf("%d %d", &l[i], &arr[i]); // 输入长度和数字
    }

    int max_value = 0; // 存储最终的最大值

    // 找到长度最大的数字
    for (i = 0; i < n; i++) {
        if (l[i] == l[0]) { // 只考虑长度等于最大长度的数字
            int current_num = arr[i];
            char digits[10];
            int len = 0;

            // 将数字转化为字符数组
            while (current_num > 0) {
                digits[len++] = current_num % 10 + '0';
                current_num /= 10;
            }

            // 暴力交换两位数字
            for (int p = 0; p < len; p++) {
                for (int q = p; q < len; q++) {
                    // 交换两位
                    char temp = digits[p];
                    digits[p] = digits[q];
                    digits[q] = temp;

                    // 将数组转回数字
                    int swapped_num = 0;
                    for (int k = 0; k < len; k++) {
                        swapped_num = swapped_num * 10 + (digits[k] - '0');
                    }

                    // 更新最大值
                    if (swapped_num > max_value) {
                        max_value = swapped_num;
                    }

                    // 还原交换
                    temp = digits[p];
                    digits[p] = digits[q];
                    digits[q] = temp;
                }
            }
        }
    }

    // 输出最终的最大值
    printf("%d\n", max_value);

    return 0;
}
