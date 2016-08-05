#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>
#include <time.h>

typedef struct Mkdisk
{
    int size;
    char unit;
    char path[100];
    char name[100];
}Mkdisk;

typedef struct Fdisk
{
    int size;
    char unit; //default = k
    char path[100];
    char type; //default = p
    char fit[2]; //default = wf
    char borrar[4];
    char name[100];
    char add; //lo mismo en unit
}Fdisk;

typedef struct PARTITON
{
    char part_status;
    char part_type;
    char part_fit;
    int part_start;
    int part_size;
    char part_name[16];

}PARTITON;

typedef struct MBR
{
    int mbr_tamanio;
    time_t mbr_fecha_creacion;
    int mbr_disk_signature;
    struct PARTITON particiones[4];

}MBR;


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
        fgets(Linea,150,stdin);
        tokens = str_split(Linea, ' ');
        if (tokens)
        {
            //AQUI VERIFICA QUE COMANDO
            int i;
            for (i = 0; *(tokens + i); i++)
            {
                //printf("token=[%s]\n", *(tokens + i));

                //SI ENCUENTRA UN COMANDO RECORRE LA LISTA DESDE ESE PUNTO
                if(strcasecmp((*(tokens + i)), "mkdisk") ==0 )
                {
                    Mkdisk * cmdDisk = malloc(sizeof(Mkdisk));
                    cmdDisk->unit = 'm';
                    int r;
                    char * PathGlobal, * NameGlobal;
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
                            PathGlobal = SplitComandos;
                            strcpy(cmdDisk->path, SplitComandos);

                        }
                        else if (strncasecmp((*(tokens + r)), "-name", 5) == 0)
                        {
                            //printf("si entra al name");

                            char * SplitComandos;
                            SplitComandos = strtok((*(tokens + r)),"::");
                            SplitComandos = strtok(NULL, "::");
                            //SplitComandos = strtok(NULL, "\"");
                            NameGlobal = SplitComandos;
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

                   /* printf("size -> %d \n", cmdDisk->size);
                    printf("unit -> %s \n", &cmdDisk->unit);
                    printf("path -> %s\n", cmdDisk->path);
                    printf("name -> %s\n", cmdDisk->name);*/

                    printf("********VALIDANDO EL MKDISK********* \n");

                    if (cmdDisk->size > 0 && strcmp(cmdDisk->path, "") != 0 && strcmp(cmdDisk->name, "") != 0)
                    {
                        //mkdisk -size::66 -path::hola +unit::m -name::adios
                        //mkdisk -size::66 -path::hola -name::adios.dsk +unit::m
                        //VALIDANDO UNIT
                        if ( cmdDisk->unit == 'k' || cmdDisk->unit == 'm' || cmdDisk->unit == 'K' || cmdDisk->unit == 'M')
                        {
                            char ext [4] = {'.', 'd', 's', 'k'};
                            //VALIDANDO NAME
                            if (ValidarNombre(cmdDisk->name, ext))
                            {
                               //mkdisk -size::66 -name::"adios.dsk" +unit::m -path::"/home/jjenano/Documentos/Discos/"
                               //mkdisk -size::66 -name::prueba.dsk -path::/home/jjenano/Documentos/Discos/
                               printf("************SI TIENE EXTENSION************ \n");


                               char * completo = strcat(PathGlobal, NameGlobal);

                               FILE* archivo = fopen(completo, "wb+");
                               if (archivo != NULL)
                               {

                                   int tamanio;
                                   if (cmdDisk->unit == 'k' || cmdDisk->unit == 'K')
                                   {
                                       tamanio = 1024 * cmdDisk->size;
                                   }
                                   else
                                   {
                                       tamanio = 1024 * 1024 * cmdDisk->size;
                                   }
                                   char tamaArchivo[tamanio];
                                   memset(tamaArchivo, 0, sizeof(tamaArchivo));

                                   fwrite(tamaArchivo, sizeof(tamaArchivo), 1, archivo);


                                   //AQUI SE CREA EN MBR DEL ARCHIVO


                                   int signature = rand()% 1 + 63;

                                   MBR * cmdMBR = malloc(sizeof(MBR));
                                   cmdMBR->mbr_tamanio = tamanio;
                                   cmdMBR->mbr_fecha_creacion = time(NULL);
                                   cmdMBR->mbr_disk_signature = signature;

                                   int part;
                                   for (part = 0; part < 4 ; part++)
                                   {
                                        cmdMBR->particiones[part].part_status = '\0';
                                        cmdMBR->particiones[part].part_type = '\0';
                                        cmdMBR->particiones[part].part_fit = '\0';
                                        cmdMBR->particiones[part].part_start = 0;
                                        cmdMBR->particiones[part].part_size = 0;

                                   }

                                   fseek(archivo, 0, SEEK_SET);
                                   fwrite(&cmdMBR, sizeof(MBR), 1, archivo);

                                   fclose(archivo);


                                   /*FILE * probando = fopen (completo, "rb+");

                                   MBR * temp = malloc(sizeof(MBR));
                                   fseek(probando, 0, SEEK_SET);
                                   fread(&temp, sizeof(MBR), 1, probando);

                                   printf("hola :)))))))))))))))))))))))))))))) %d" , temp->mbr_tamanio);

                                   //fclose(probando);*/

                               }

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
                else if (strcasecmp((*(tokens + i)), "rmdisk") ==0 )
                {
                    Mkdisk * cmdRDisk = malloc(sizeof(Mkdisk));
                    int r;
                    for (r = (i+1); *(tokens + r); r++)
                    {
                        if (strncasecmp((*(tokens + r)), "-path", 5) == 0)
                        {

                            char * SplitComandos;
                            SplitComandos = strtok((*(tokens + r)),"::");
                            SplitComandos = strtok(NULL, "::");
                            //SplitComandos = strtok(NULL, "\"");

                            strcpy(cmdRDisk->path, SplitComandos);

                        }
                    }

                    if (strcmp(cmdRDisk->path, "") != 0)
                    {
                        int opcion;
                        printf("***********¿Seguro que desea borrar el archivo? ************** \n 1. Si \n 2. No \n \n");
                        scanf("%d", &opcion);

                        if (opcion == 1)
                        {
                            //AIIIIIIIIIIIIIIIIIUUUUUUUUUUUUUUUUDA
                        }
                        else
                        {
                            printf("*************NO SE ELMINARA EL ARCHIVO*************");
                        }
                    }
                    else
                    {
                        printf("***********ERROR NO SE ESPECIFICO EL PATH**************");
                    }
                }
                else if (strcasecmp((*(tokens + i)), "fdisk") ==0 )
                {
                    Fdisk * cmdFdisk = malloc(sizeof(Fdisk));
                    cmdFdisk->unit = 'k';
                    cmdFdisk->type = 'p';
                    cmdFdisk->fit[0] = 'w';
                    cmdFdisk->fit[1] = 'f';
                    cmdFdisk->add = 'k';

                    int r;
                    char * PathGlobal, * NameGlobal;

                    for (r = (i+1); *(tokens + r); r++)
                    {
                        if (strncasecmp((*(tokens + r)), "-size", 5) == 0)
                        {
                            cmdFdisk->size = DevolverValorInt(*(tokens+r));
                        }
                        else if (strncasecmp((*(tokens + r)), "+unit", 5) == 0)
                        {
                            char * SplitComandos;
                            SplitComandos = strtok((*(tokens + r)),"::");
                            SplitComandos = strtok(NULL, "::");
                            cmdFdisk->unit = *SplitComandos;
                        }
                        else if (strncasecmp((*(tokens + r)), "-path", 5) == 0)
                        {
                            char * SplitComandos;
                            SplitComandos = strtok((*(tokens + r)),"::");
                            SplitComandos = strtok(NULL, "::");
                            //SplitComandos = strtok(NULL, "\"");
                            PathGlobal = SplitComandos;
                            strcpy(cmdFdisk->path, SplitComandos);

                        }
                        else if (strncasecmp((*(tokens + r)), "+type", 5) == 0)
                        {
                            char * SplitComandos;
                            SplitComandos = strtok((*(tokens + r)),"::");
                            SplitComandos = strtok(NULL, "::");
                            cmdFdisk->type = *SplitComandos;
                        }
                        else if (strncasecmp((*(tokens + r)), "+fit", 4) == 0)
                        {
                            char * SplitComandos;
                            SplitComandos = strtok((*(tokens + r)),"::");
                            SplitComandos = strtok(NULL, "::");
                            //SplitComandos = strtok(NULL, "\"");
                            PathGlobal = SplitComandos;
                            strcpy(cmdFdisk->fit, SplitComandos);

                        }
                        else if (strncasecmp((*(tokens + r)), "+delete", 7) == 0)
                        {
                            char * SplitComandos;
                            SplitComandos = strtok((*(tokens + r)),"::");
                            SplitComandos = strtok(NULL, "::");
                            //SplitComandos = strtok(NULL, "\"");
                            PathGlobal = SplitComandos;
                            strcpy(cmdFdisk->borrar, SplitComandos);

                        }
                        else if (strncasecmp((*(tokens + r)), "-name", 5) == 0)
                        {
                            //printf("si entra al name");

                            char * SplitComandos;
                            SplitComandos = strtok((*(tokens + r)),"::");
                            SplitComandos = strtok(NULL, "::");
                            //SplitComandos = strtok(NULL, "\"");
                            NameGlobal = SplitComandos;
                            strcpy(cmdFdisk->name, SplitComandos);
                        }
                        else if (strncasecmp((*(tokens + r)), "+add", 4) == 0)
                        {
                            char * SplitComandos;
                            SplitComandos = strtok((*(tokens + r)),"::");
                            SplitComandos = strtok(NULL, "::");
                            cmdFdisk->add = *SplitComandos;
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

                    printf("********VALIDANDO EL FDISK********* \n");

                    if (cmdFdisk->size > 0 && strcmp(cmdFdisk->path, "") != 0 && strcmp(cmdFdisk->name, "") != 0)
                    {
                        //VALIDANDO UNIT
                        if ( cmdFdisk->unit == 'k' || cmdFdisk->unit == 'm' || cmdFdisk->unit == 'b' || cmdFdisk->unit == 'K' || cmdFdisk->unit == 'M' || cmdFdisk->unit == 'B')
                        {
                            //fdisk -size::10 -path::hola -name::adios +unit::B
                            //printf("SI ENTRA********************** \n");
                            //VALIDANDO TYPE
                            if ( cmdFdisk->type == 'l' || cmdFdisk->type == 'e' || cmdFdisk->type == 'p' || cmdFdisk->type == 'L' || cmdFdisk->type == 'E' || cmdFdisk->type == 'P')
                            {

                                //VALIDANDO FIT
                                if ( strcasecmp(cmdFdisk->fit, "bf") == 0 || strcasecmp(cmdFdisk->fit, "ff") == 0 || strcasecmp(cmdFdisk->fit, "wf") == 0)
                                {
                                    //VALIDANDO ADD
                                    if ( cmdFdisk->add == 'k' || cmdFdisk->add == 'm' || cmdFdisk->add == 'b' || cmdFdisk->add == 'K' || cmdFdisk->add == 'M' || cmdFdisk->add == 'B')
                                    {
                                        printf("%s %s", PathGlobal, NameGlobal);
                                        printf("FIN DE FINES :V");
                                    }
                                    else
                                    {
                                        printf("**************ERROR EN EL COMANDO ADD************** \n");
                                    }
                                }
                                else
                                {
                                    printf("**************ERROR EN EL COMANDO FIT************** \n");
                                }
                            }
                            else
                            {
                                printf("**************ERROR EN EL COMANDO TYPE************** \n");
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
