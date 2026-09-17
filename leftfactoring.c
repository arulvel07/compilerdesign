#include <stdio.h>
#include <string.h>

char rule[20][10][50],name[20][10];
int cnt[20],total=1,id=1;

void factor(int r){
    int best,k,g,changed=1;
    char pre[50],temp[10][50],newname[10];

    while(changed){
        changed=0;
        best=0;

        for(int i=0;i<cnt[r];i++)
            for(int j=i+1;j<cnt[r];j++){
                k=0;
                while(rule[r][i][k]&&rule[r][i][k]==rule[r][j][k]) k++;
                if(k>best){
                    best=k;
                    strncpy(pre,rule[r][i],k);
                    pre[k]='\0';
                }
            }

        if(!best) break;
        changed=1;

        sprintf(newname,"A%d",id++);
        strcpy(name[total],newname);

        g=0;
        for(int i=0;i<cnt[r];i++)
            if(strncmp(rule[r][i],pre,best)==0){
                strcpy(rule[total][g],rule[r][i]+best);
                if(!rule[total][g][0]) strcpy(rule[total][g],"e");
                g++;
            }

        cnt[total]=g;
        total++;

        g=0;
        int added=0;

        for(int i=0;i<cnt[r];i++){
            if(strncmp(rule[r][i],pre,best)==0){
                if(!added){ 
                    strcpy(temp[g],pre);
                    strcat(temp[g],newname);
                    g++;
                    added=1;
                }
            }else
                strcpy(temp[g++],rule[r][i]);
        }

        cnt[r]=g;
        for(int i=0;i<g;i++)
            strcpy(rule[r][i],temp[i]);
    }

    printf("%s -> ",name[r]);
    for(int i=0;i<cnt[r];i++)
        printf("%s%s",rule[r][i],i==cnt[r]-1?"\n":" | ");
}

int main(){
    char input[500],*p,*t,*arrow;

    fgets(input,500,stdin);
    input[strcspn(input,"\n")]=0;

    arrow=strstr(input,"->");
    strncpy(name[0],input,arrow-input);
    name[0][arrow-input]=0;

    p=strtok(arrow+2,"|");
    while(p){
        while(*p==' ') p++;
        strcpy(rule[0][cnt[0]++],p);
        p=strtok(NULL,"|");
    }

    printf("Original grammar rule: %s -> ",name[0]);
    for(int i=0;i<cnt[0];i++)
        printf("%s%s",rule[0][i],i==cnt[0]-1?"\n":" | ");

    printf("\nLeft-factored grammar rules:\n");

    for(int i=0;i<total;i++)
        factor(i);

    return 0;
}