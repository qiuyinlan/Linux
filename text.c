#include<stdio.h>
int main(){
    int arr[9],arr2[9],count[9];
    int i,found=0,j;
    for(i=0;i<9;i++){
        scanf("%d",&arr[i]);
    }
    arr2[found]=arr[0];
    count[found]=1;
    found++;
    for(i=1;i<9;i++){
        for(j=0;j<found;j++){
            if(arr[i]==arr2[j]){
                count[j]++;
                break;
            }
            if(count[j]>3)
        {
            printf("No");
            return 0;
        }
            
        }
        if(found==j){
            arr2[found]=arr[i];
            count[found]=1;
            found++;
        }
        if(found>3)
        {
            printf("No");
            return 0;
        }
        
        
    }
    for(i=0;i<found;i++){
        if(count[i]!=3){
            printf("No");
            return 0;
        }
        
    }
    if(i==found){
        printf("Yes");
    }

}