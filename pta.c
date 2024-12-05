#include<stdio.h>
#include<string.h>
int main(){
    char c[40];
    gets(c);
    int i,zimu=0,shuzi=0,kongge=0,m=0;
    for(i=0;i<40;i++){
        if((c[i]<='Z'&&c[i]>='A')||(c[i]<='z'&&c[i]>='a'))
            zimu++;
        else if(c[i]<='9'&&c[i]>='0')
            shuzi++;
        else if(c[i]==' ')
            kongge++;
        else 
            m++;
    }
    printf("zimu=%d\nshuzi=%d\nkongge=%d\nqita=%d\n",zimu,shuzi,kongge,m);
    puts(c);
}