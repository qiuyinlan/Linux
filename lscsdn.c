#include<stdio.h>
#include<time.h>                    
#include<unistd.h>
#include<sys/stat.h>
#include<sys/types.h>        //不完全，缺少容错性，及链接显示问题和执行权限x
#include<stdlib.h>
#include<grp.h>              
#include<pwd.h>
#include<string.h>
#include<dirent.h>
#include<stdlib.h>

//a    显示隐藏文件
//l    显示详细信息
//R    显示详细目录
//i    显示节点索引
//n    将用户名和组名用uid,gid表示
//f    不排序
//r    逆排序



void mls(char *name,int x,int y,int z,int u,int r,int mlsf,int mlsr)  //(a,l,R,i,n,mlsf，mlsr) 
{
    DIR *dir;
    struct stat buf;   //文件信息
    struct passwd *p;        //获取文件所有者的名字
    struct group *g;   //获取所有者所在组的名字
    struct dirent *w;  //目录信息

    int i=0,j,t,f,C=0,k=0,sum=0,m;
    char a[1000][200],e[200],c[1000][200],b[50],d[1000][200];

    if(z)
        printf("\n%s:\n",name);

    if((dir=opendir(name))==NULL)
    {   
        printf("没有那个文件或目录\n");
        return ;
    }

    while((w=readdir(dir))!=NULL)
    {   
        if(x)
        {
            if( (t=strlen(strcpy(a[C],w->d_name))) > k )
                k=t;
            strcpy(c[C],w->d_name);
            strcpy(a[C],name);
            if(strcmp(name,"/"))
                strcat(a[C],"/");
            strcat(a[C++],w->d_name);
        }
        else
        {
            if(w->d_name[0]!='.')
            {   
                if( (t=strlen(strcpy(a[C],w->d_name))) > k )
                    k=t;
                strcpy(c[C],w->d_name);
                strcpy(a[C],name);
                if(strcmp(name,"/"))
                    strcat(a[C],"/");
                strcat(a[C++],w->d_name);
            }       
        }
    }
    //文件名排序
    if(!mlsf)
    {
        if(!mlsr)
        {
            for(i=0;i<C-1;i++)
                for(j=0;j<C-1-i;j++)
                    if(strcmp(a[j],a[j+1])>0)
                    {
                        strcpy(e,a[j]);
                        strcpy(a[j],a[j+1]);       //a全路径  c相对路径
                        strcpy(a[j+1],e);

                        strcpy(e,c[j]);
                        strcpy(c[j],c[j+1]);
                        strcpy(c[j+1],e);   
                    }

        }
        else
        {
            for(i=0;i<C-1;i++)
                for(j=0;j<C-1-i;j++)
                    if(strcmp(a[j],a[j+1])<0)
                        {
                            strcpy(e,a[j]);
                            strcpy(a[j],a[j+1]);       //a全路径  c相对路径
                            strcpy(a[j+1],e);

                            strcpy(e,c[j]);
                            strcpy(c[j],c[j+1]);
                            strcpy(c[j+1],e);   
                        }
        }
    }

    if(y)           
    {
        for(i=0;i<C;i++)
        {   
            stat(a[i],&buf);
            sum+=buf.st_size;
        }
        printf("文件总大小%d(KB)  文件总数%d\n",sum/1024,C);
    }

    for(i=0,m=0;i<C;i++)
    {   
        if(stat(a[i],&buf))      //它可以自动识别相对路径下的文件
            printf("获取文件属性失败");

        if(u)
            printf("%7ld  ",buf.st_ino);
        if(y)
        {   
             //文件类型
        if( S_ISLNK(buf.st_mode) )
            printf("l");
        if( S_ISREG(buf.st_mode) )
            printf("-");
        if( S_ISDIR(buf.st_mode) )
            printf("d");
        if( S_ISCHR(buf.st_mode) )
            printf("c");
        if( S_ISBLK(buf.st_mode) )
            printf("b");
        if( S_ISFIFO(buf.st_mode) )
            printf("f");
        if( S_ISSOCK(buf.st_mode) )
            printf("s");

        //文件权限/所有者
        if( buf.st_mode & S_IRUSR)
            printf("r");    
        else
            printf("-");
        if( buf.st_mode & S_IWUSR)
            printf("w");
        else
            printf("-");
        if( buf.st_mode & S_IXUSR)
            printf("x");
        else
            printf("-");

        //所属的用户组
        if( buf.st_mode & S_IRGRP)
            printf("r");
        else    
            printf("-");
        if( buf.st_mode & S_IWGRP)
            printf("w");
        else
            printf("-");
        if( buf.st_mode & S_IXGRP)
            printf("x");
        else
            printf("-");

        //其他人
        if( buf.st_mode & S_IROTH)
            printf("r");
        else
            printf("-");
        if( buf.st_mode & S_IWOTH)
            printf("w");
        else
            printf("-");
        if( buf.st_mode & S_IXOTH)
            printf("x");
        else
            printf("-");

        printf("  ");

        //文件个数
        printf("%3ld",buf.st_nlink);

        printf("  ");

        //文件所有者
        p=getpwuid(buf.st_uid);
        if(r)
            printf("%5d",buf.st_uid);
        else
            printf("%6s",p->pw_name);

        printf("  ");

        //文件所有者所在组
        g=getgrgid(buf.st_gid);
        if(r)
            printf("%5d",buf.st_gid);
        else
            printf("%6s",g->gr_name);

        printf("  ");

        //文件大小
        printf("%8ld",buf.st_size);

        printf("  ");

        //文件最后一次被修改的时间
        strcpy(b,ctime(&buf.st_mtime));
        b[strlen(b)-1]='\0';                 //×去掉换行符×
        printf("%s",b);

        printf("  ");

        //文件名
        printf("%s\n",c[i]); 

        }

        if(z)
            if( S_ISDIR(buf.st_mode) )
                if( strcmp(c[i],".") && strcmp(c[i],"..") )
                    strcpy(d[m++],a[i]);
        if(!y)
            if( x || z || (!x&&!y&&!z || w))
            {
                if( (t=strlen(c[i])) < k)
                {   
                    for(f=t;f<k;f++)
                        c[i][f]=' ';
                    c[i][f]='\0';   
                }
                printf("%s  ",c[i]);

                if((i+1)%5==0)
                    if(i!=C-1)
                        printf("\n");
            }
    }
    printf("\n");

    if(z)
        for(i=0;i<m;i++)
            mls(d[i],x,y,z,u,r,mlsf,mlsr);

    closedir(dir);
}

int main(int argc,char *argv[])
{
    int i,t=-1,x=0,y=0,z=0,w=0,n=0,f=0,r=0;
    char ml[50]; 

    getcwd(ml,50);  //获取当前目录路径

    switch(argc)                   
    {
        case 1:mls(ml,x,y,z,w,n,f,r);break;

        case 2:
                if(argv[1][0]!='-')
                    mls(argv[1],x,y,z,w,n,f,r);
                else        
                {
                    for(i=1;i<strlen(argv[1]);i++)
                        {
                            if(argv[1][i]=='a')                   
                                x=1;
                            if(argv[1][i]=='l')
                                y=1;
                            if(argv[1][i]=='R')
                                z=1;
                            if(argv[1][i]=='i')
                                w=1;                    
                            if(argv[1][i]=='n')
                            {
                                n=1;
                                y=1;
                            }
                            if(argv[1][i]=='f')
                                f=1;
                            if(argv[1][i]=='r')
                                r=1;
                        }
                    mls(ml,x,y,z,w,n,f,r);
                }
                break;
        case 3:
                if(argv[1][0]!='-')
                    return printf("参数输入有误");
                else        
                {
                    for(i=1;i<strlen(argv[1]);i++)
                    {
                        if(argv[1][i]=='a')                         
                            x=1;
                        if(argv[1][i]=='l')
                            y=1;
                        if(argv[1][i]=='R')
                            z=1;
                        if(argv[1][i]=='i')
                            w=1;        
                        if(argv[1][i]=='n')
                        {
                            n=1;
                            y=1;
                        }
                        if(argv[1][i]=='f')
                            f=1;
                        if(argv[1][i]='r')
                            r=1;
                    }
                    mls(argv[2],x,y,z,w,n,f,r);
                }
                break;
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:printf("只接受四个以下的参数，第三个参数时第二个参数为a l R f i n r的组合\n");
    }
}

