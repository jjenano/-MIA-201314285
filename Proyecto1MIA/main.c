#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>

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

bool ValidarNombre(char cadena[100], char ext[4])
{
    int i;

    for (i = 0; i < 97; i++)
    {
        if (cadena[i] == ext[0] && cadena[i+1] == ext[1] && cadena[i+2] == ext[2] && cadena[i+3] == ext[3] )
        {
            return true;
        }
    }

    return false;
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
                        }
                        else if (strncasecmp((*(tokens + r)), "-path", 5) == 0)
                        {
                            char * SplitComandos;
                            SplitComandos = strtok((*(tokens + r)),"::");
                            SplitComandos = strtok(NULL, "::");
                            //SplitComandos = strtok(NULL, "\"");

                            strcpy(cmdDisk->path, SplitComandos);

                        }
                        else if (strncasecmp((*(tokens + r)), "-name", 5) == 0)
                        {
                            //printf("si entra al name");

                            char * SplitComandos;
                            SplitComandos = strtok((*(tokens + r)),"::");
                            SplitComandos = strtok(NULL, "::");
                            //SplitComandos = strtok(NULL, "\"");

                            strcpy(cmdDisk->name, SplitComandos);
                        }
                        else
                        {
                            //SI VIENE UNA DIAGONAL QUE SIGA ESCRIBIENDO EL MISMO MKDISK
                            printf("Escriba comandos \n");
                            fgets(Linea,60,stdin);
                            tokens = str_split(Linea, ' ');
                        }

                         // printf("token = [%s] \n", *(tokens + r));
                    }

                    /*printf("size -> %d \n", cmdDisk->size);
                    printf("unit -> %s \n", &cmdDisk->unit);
                    printf("path -> %s\n", cmdDisk->path);
                    printf("name -> %s\n", cmdDisk->name);*/

                    printf("********VALIDANDO EL MKDISK********* \n");

                    if (cmdDisk->size > 0 && strcmp(cmdDisk->path, "") != 0 && strcmp(cmdDisk->name, "") != 0)
                    {
                        //mkdisk -size::66 -path::hola +unit::m -name::adios
                        //mkdisk -size::66 -path::hola -name::adios.dsk +unit::m
                        //VALIDANDO UNIT
                        if ( cmdDisk->unit == 'k' || cmdDisk->unit == 'm')
                        {
                            char ext [4] = {'.', 'd', 's', 'k'};
                            //VALIDANDO NAME
                            if (ValidarNombre(cmdDisk->name, ext))
                            {
                               printf("************SI TIENE EXTENSION************");
                            }
                            else
                            {
                                printf("**************ERROR EN EL COMANDO NAME************** \n");
                            }
                        }
                        else
                        {
                            printf("**************ERROR EN EL COMANDO UNIT************** \n");
                        }
                    }
                    else
                    {
                        printf("**************ERROR ALGUNO DE LOS PARAMETROS ESTA VACIO************** \n");
                    }


                }

                //free(*(tokens + i));
            }
            printf("\n");
            free(tokens);
        }

    }

    return 0;
}
