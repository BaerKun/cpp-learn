#include <stdio.h>
#include <string.h>

int main(){
    FILE *file = fopen("../GLSL/frgshad1.fs", "r");
    char s[100]={0};
    if(file == NULL){
        printf("JOFSN");
        return 1;
    }
    if(fgets(s, 95, file) != NULL)
        printf("%s", s);
    else
        printf("!@");

    fclose(file);
    return 0;
}
