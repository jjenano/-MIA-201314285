#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

typedef struct Mkdisk
{
    int size;
    char unit;
    char path[100];
    char name[100];
}Mkdisk;

char** str_split(char* a_str, const char a_delim)
{
    char** resultado = 0;
    size_t contador = 0;
    char* tmp = a_str;
    char* ultimoPara = 0;
    char delim[2];
    delim[0] = a_delim;
    delim[1] = 0;

    /* se cuenta cuantos elementos hay */
    while (*tmp)
    {
        if (a_delim == *tmp)
        {
            contador++;
            ultimoPara = tmp;
        }
        tmp++;
    }

    /* se añade un espacio al token que se trea*/
    contador += ultimoPara < (a_str + strlen(a_str) - 1);

    /* se añade un espacio para el caracter nulo */
    contador++;

    resultado = malloc(sizeof(char*) * contador);

    if (resultado)
    {
        size_t idx  = 0;
        char* token = strtok(a_str, delim);

        while (token)
        {
            assert(idx < contador);
            *(resultado + idx++) = strdup(token);
            token = strtok(0, delim);
        }
        assert(idx == contador - 1);
        *(resultado + idx) = 0;
    }

    return resultado;
}




//SPLIT DE LOS :: DE CADA COMANDO

int DevolverValorInt (char * palabra)
{
    char * SplitComandos;
    SplitComandos = strtok(palabra,"::");
    SplitComandos = strtok(NULL, "::");
    int numero = atoi(SplitComandos);
    return numero;
}

char DevolverValorChar (char * palabra)
{
    char * SplitComandos;
    SplitComandos = strtok(palabra,"::");
    SplitComandos = strtok(NULL, "::");
    char numero[100];
    strcpy(numero, SplitComandos);
    return *numero;
}

char** tokens;
char Linea[60];
char * SplitComandos;
int d=0;

int main()
{
while (d==0)
    {
        printf("Escriba comandos \n");
        fgets(Linea,60,stdin);
        tokens = str_split(Linea, ' ');
        if (tokens)
        {
            //AQUI VERIFICA QUE COMANDO
            int i;
            for (i = 0; *(tokens + i); i++)
            {
                //printf("token=[%s]\n", *(tokens + i));

                //SI ENCUENTRA MKDISK RECORRE LA LISTA DESDE ESE PUNTO
                if(strcasecmp((*(tokens + i)), "mkdisk") ==0 )
                {
                    Mkdisk * cmdDisk = malloc(sizeof(Mkdisk));
                    cmdDisk->unit = 'm';
                    int r;
                    for (r = (i+1); *(tokens + r); r++)
                    {
                        if (strncasecmp((*(tokens + r)), "-size", 5) == 0)
                        {
                            cmdDisk->size = DevolverValorInt(*(tokens+r));
                        }
                        else if (strncasecmp((*(tokens + r)), "+unit", 5) == 0)
                        {
                            char * SplitComandos;
                            SplitComandos = strtok((*(tokens + r)),"::");
                            SplitComandos = strtok(NULL, "::");
                            cmdDisk->unit = *SplitComandos;
                            //printf ("Splitcomando -> %s", SplitComandos);
                            //printf("unit -> %c \n", cmdDisk->unit);
                        }
                        else if (strncasecmp((*(tokens + r)), "-path", 5) == 0)
                        {
                            int d = strncasecmp((*(tokens + r)), "-path", 5);

                            printf("si entra al path %d", d);
                        }
                        else if (strncasecmp((*(tokens + r)), "-name", 5) == 0)
                        {
                            printf("si entra al name");
                        }

                         // printf("token = [%s] \n", *(tokens + r));
                    }

                    printf("size -> %d \n", cmdDisk->size);
                    printf("unit -> %s \n", &cmdDisk->unit);
                    printf("path -> %s", cmdDisk->path);
                    printf("name -> %s", cmdDisk->name);
                }

                //free(*(tokens + i));
            }
            printf("\n");
            free(tokens);
        }

    }

    return 0;
}
