#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// 交换数组元素
void jiaoHuan(int *a, int *b) {
    int t = *a;
    *a = *b;
    *b = t;
}

// 检查是否错位排列
int shiCuoWei(int arr[], int n) {
    for (int i = 0; i < n; i++) {
        if (arr[i] == i + 1) {
            return 0;
        }
    }
    return 1;
}

// 生成排列并找错位排列
void shengChengPL(int arr[], int kaiShi, int n, int **jieGuo, int *shuLiang) {
    if (kaiShi == n) {
        if (shiCuoWei(arr, n)) {
            // 存储错位排列
            *jieGuo = (int *)realloc(*jieGuo, ((*shuLiang) + 1) * sizeof(int) * n);
            for (int i = 0; i < n; i++) {
                (*jieGuo)[(*shuLiang) * n + i] = arr[i];
            }
            (*shuLiang)++;
        }
        return;
    }
    for (int i = kaiShi; i < n; i++) {
        jiaoHuan(&arr[kaiShi], &arr[i]);
        shengChengPL(arr, kaiShi + 1, n, jieGuo, shuLiang);
        jiaoHuan(&arr[kaiShi], &arr[i]);
    }
}

// 计算错位排列数量
int jiShuCuoWei(int n) {
    int *arr = (int *)malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) {
        arr[i] = i + 1;
    }
    int *jieGuo = NULL;
    int shuLiang = 0;
    shengChengPL(arr, 0, n, &jieGuo, &shuLiang);
    free(arr);
    return shuLiang;
}

// 计算并展示概率与 1/e 差异
void jiSuanXianShi(int zuiDaShu) {
    double e = exp(1);
    for (int n = 1; n <= zuiDaShu; n++) {
        int cuoWeiShu = jiShuCuoWei(n);
        int jie = 1;
        for (int i = 2; i <= n; i++) {
            jie *= i;
        }
        double gaiLv = (double)cuoWeiShu / jie;
        double cha = fabs(gaiLv - 1 / e);
        printf("n = %d, 错位排列概率 = %lf, 与 1/e 差值 = %lf\n", n, gaiLv, cha);
    }
}

int main() {
    // 示例数据
    int shuZu[] = {1, 2, 3, 4, 5, 6, 7, 8};
    int *cuoWeiPL = NULL;
    int cuoWeiShu = 0;
    shengChengPL(shuZu, 0, 8, &cuoWeiPL, &cuoWeiShu);
    printf("集合 {1, 2, 3, 4, 5, 6, 7, 8} 的错位排列：\n");
    for (int i = 0; i < cuoWeiShu; i++) {
        for (int j = 0; j < 8; j++) {
            printf("%d ", cuoWeiPL[i * 8 + j]);
        }
        printf("\n");
    }
    free(cuoWeiPL);

    // 计算概率分析
    jiSuanXianShi(25);

    return 0;
}