#include <stdio.h>
#include <string.h>

int main(){
    char line[100],lhs;
    char rhs[10][50];
    int n=0,lr=0;

    printf("Enter grammar: ");
    fgets(line,100,stdin);

    lhs=line[0];
    char *p=strstr(line,"->")+2;
    char *t=strtok(p,"|\n");

    while(t){
        strcpy(rhs[n++],t);
        t=strtok(NULL,"|\n");
    }

    for(int i=0;i<n;i++){
        if(rhs[i][0]==lhs)
            lr=1;
    }

    printf("Original: %c -> ",lhs);
    for(int i=0;i<n;i++)
        printf("%s%s",rhs[i],i==n-1?"\n":" | ");

    if(!lr){
        printf("No Left Recursion\n");
        return 0;
    }

    printf("After eliminating left recursion:\n");
    printf("%c -> ",lhs);

    for(int i=0;i<n;i++)
        if(rhs[i][0]!=lhs)
            printf("%s%c1",rhs[i],lhs);

    printf("\n%c1 -> ",lhs);

    for(int i=0;i<n;i++)
        if(rhs[i][0]==lhs)
            printf("%s%c1 | ",rhs[i]+1,lhs);

    printf("e\n");

    return 0;
}