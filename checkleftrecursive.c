#include <stdio.h>
#include <string.h>
#include <ctype.h>

int adj[26][26],n;

int dfs(int start,int cur,int vis[])
{
    if(cur==start) return 1;
    vis[cur]=1;

    for(int i=0;i<26;i++)
        if(adj[cur][i]&&!vis[i])
            if(dfs(start,i,vis))
                return 1;

    return 0;
}

int main()
{
    char line[100],*p,*t;
    char lhs;

    printf("Number of productions: ");
    scanf("%d",&n);
    getchar();

    for(int i=0;i<n;i++)
    {
        fgets(line,100,stdin);
        lhs=line[0];

        p=strstr(line,"->")+2;
        t=strtok(p,"|\n");

        while(t)
        {
            while(*t==' ') t++;

            if(isupper(*t))
                adj[lhs-'A'][*t-'A']=1;

            t=strtok(NULL,"|\n");
        }
    }

    int found=0;

    for(int i=0;i<26;i++)
    {
        if(adj[i][i])
        {
            printf("%c: Direct Left Recursion\n",i+'A');
            found=1;
        }
    }

    for(int i=0;i<26;i++)
    {
        for(int j=0;j<26;j++)
        {
            if(i!=j&&adj[i][j])
            {
                int vis[26]={0};

                if(dfs(i,j,vis))
                {
                    printf("%c: Indirect Left Recursion\n",i+'A');
                    found=1;
                    break;
                }
            }
        }
    }

    if(!found)
        printf("No Left Recursion\n");

    return 0;
}