#include<stdio.h>
int main(){
    int n,i,j,l[100],arr[100];
    scanf("%d",& n);
    int lon;
    for(i=0;i<n;i++){
        scanf("%d",&l[i]);
        lon=l[0];
        scanf("%d",&arr[i]);
        if(l[i]>lon)
            lon=l[i];
        
    }
   int maxshuzu[9],count=0,label=0;

    for(i=0;i<n;i++){
        if(l[i]==lon){
            count++;
             int ex[100];
            int found=arr[0];
            for(j=l[i]-1;j>=0;j--){
                ex[j]=found%10;
                found=ex[j];
            }
            int max=ex[0];
            for(j=1;j<l[i]-1;j++) {
                if(ex[j]>max){
                max=ex[j];
                label=j;
                }
            }
            maxshuzu[i]=max;
            int t;
            



        }
        
    }

}