#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void func2(char* input) {
    // To be modified

    char buffer[100];
    int length = strlen(input);
    if (length>sizeof(buffer))
    {
        exit(0);
    }
    else{
        strcpy(buffer, input);
        printf("%s\n", buffer);


 /* code */
    }
    // if(sizeof())
    //         ;
}

void func1(char* input) {
    // To be modified

    //
    // int length = strlen(input);
     //input=(char *)malloc(length*sizeof(input));
     char *longstr = (char *) malloc(1000*sizeof(longstr));
    // char longstr[30];
    char shortstr[20];
    strcpy(longstr, input);
    strcpy(shortstr,"Small String");
    
    func2(longstr);
    func2(shortstr);

    free(longstr);
}

int main() {
    // Do not modify this part
    char inp[1000];
    printf("Input string: ");
    fgets(inp, 1000, stdin);
    while (1) {
        func1(inp);
    }
    return 0;
}
