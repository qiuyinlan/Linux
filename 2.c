#include <stdio.h>
int main(){
    int n;
    scanf("%d",&n);
    int i,j;
    typedef struct s{
        char kemu[100];
        int num;
    }s;
    s s1[n];
    for(i=0;i<n;i++){
        scanf("%s %d",s1[i].kemu,&s1[i].num);
    }
    for(i=0;i<n-1;i++)
    for(j=0;j<n-i-1;j++){
        if(s1[j].num>s1[j+1].num){
             s temp;
            temp=s1[j];
            s1[j]=s1[j+1];
            s1[j+1]=temp;
        }
    }
    for(i=0;i<n;i++){
        printf("%s %d\n",s1[i].kemu,s1[i].num);
    }
}

