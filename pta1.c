#include <stdio.h>
#include <string.h>

int main() {
    char c[40];
    fgets(c, sizeof(c), stdin);  // 使用fgets代替gets，避免缓冲区溢出

    int i, zimu = 0, shuzi = 0, kongge = 0, m = 0;

    // 获取字符串的有效长度，避免处理多余的字符
    int len = strlen(c);
    
    for (i = 0; i < len; i++) {
        if ((c[i] >= 'A' && c[i] <= 'Z') || (c[i] >= 'a' && c[i] <= 'z')) {
            zimu++;
        } else if (c[i] >= '0' && c[i] <= '9') {
            shuzi++;
        } else if (c[i] == ' ') {
            kongge++;
        } else {
            m++;
        }
    }

    // 输出结果
    printf("zimu=%d\n", zimu);
    printf("shuzi=%d\n", shuzi);
    printf("kongge=%d\n", kongge);
    printf("qita=%d\n", m);

    return 0;
}
