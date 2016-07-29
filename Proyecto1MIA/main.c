#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Mkdisk
{
    int size;
    char unit;
    char path[100];
    char name[100];
}Mkdisk;


char Linea[60];
char * SplitComandos;
int d=0;

int main()
{
    while (d==0)
    {
        printf("Ingrese los comandos que quiera ejecutar\n");
        fgets(Linea,60,stdin);
        SplitComandos=strtok(Linea," ");

        while (SplitComandos != NULL)
        {
            printf("%s\n",SplitComandos); //IMPRIME POR PARTES :)
            if(strcasecmp(SplitComandos, "-size")==0)
            {

            }
            SplitComandos = strtok(NULL,"::");

        }

    }



    return 0;
}
