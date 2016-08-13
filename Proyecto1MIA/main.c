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

typedef struct Rep
{
    char name[100];
    char path[100];
    char id[100];
    char ruta[100];
}Rep;

typedef struct PARTITON
{
    char part_status;
    char part_type;
    char part_fit[2];
    int part_start;
    int part_size;
    char part_name[16];

}PARTITON;

typedef struct MBR
{
    int mbr_tamanio;
    time_t mbr_fecha_creacion;
    int mbr_disk_signature;
    PARTITON particiones[4];

}MBR;

typedef struct Mount{
    char path[150];
    char name[16];
    char id[4];
    bool disponible;
}Mount;

Mount Montadura[26][150];

char letras[26] = {'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z'};

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
//VALIDA QUE TENGA EXTENSION
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
//USO DEL MAIN
char** tokens;
char Linea[150];
char * SplitComandos;
int d=0;
//USO DEL EXEC
char *line = NULL;
size_t len = 0;
ssize_t read;


void CrearCarpeta(char path[100])
{
    //CREANDO LA CARPETA

    char terminalCMD[500];
    strcpy (terminalCMD, "");
    strcat(terminalCMD, "mkdir -p ");
    strcat(terminalCMD, path);
    printf("****************RUTA A CREAR: %s\n", terminalCMD);
    system(terminalCMD);

}

void BorrarCarpeta (char path[100])
{
    char terminalCMD[500];
    strcpy(terminalCMD, "");
    strcat(terminalCMD, "rmdir ");
    strcat(terminalCMD, path);
    system(terminalCMD);
}

void ejecutarMKDISK (int i)
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
            strcpy(cmdDisk->path, SplitComandos);

        }
        else if (strncasecmp((*(tokens + r)), "-name", 5) == 0)
        {
            //printf("si entra al name");

            char * SplitComandos;
            SplitComandos = strtok((*(tokens + r)),"::");
            SplitComandos = strtok(NULL, "::");
            strcpy(cmdDisk->name, SplitComandos);
        }
        else if (strncasecmp((*(tokens + r)), "\\", 1) == 0)
        {
            //no hago nada jeje
            printf("**");
        }
        else
        {
            strcat(cmdDisk->path, " ");
            strcat(cmdDisk->path, *(tokens + r));
        }

         // printf("token = [%s] \n", *(tokens + r));
    }

    printf("********VALIDANDO EL MKDISK********* \n");

    if (cmdDisk->size > 0 && strcmp(cmdDisk->path, "") != 0 && strcmp(cmdDisk->name, "") != 0)
    {
        //VALIDANDO UNIT
        if ( cmdDisk->unit == 'k' || cmdDisk->unit == 'm' || cmdDisk->unit == 'K' || cmdDisk->unit == 'M')
        {
            char ext [4] = {'.', 'd', 's', 'k'};
            //VALIDANDO NAME
            if (ValidarNombre(cmdDisk->name, ext))
            {
               //mkdisk -size::66 -name::"adios.dsk" +unit::k -path::"/home/jjenano/Documentos/Mis Discos/"
               //mkdisk -size::66 -name::"prueba.dsk" -path::"/home/jjenano/Documentos/Discos/"
               printf("************SI TIENE EXTENSION************ \n");

               CrearCarpeta(cmdDisk->path);


    /********************************************************************************************************************/
               //CONCATENANDO LAS COMILLAS
               char todo[100];
               int i, Auxiliar, comilla;
               comilla = 0;
               Auxiliar = 0;
               //CONCATENANDO EL PATH SIN LA ULTIMA \"
               for (i = 0; i<100; i++)
               {

                   if (cmdDisk->path[i] == '\"')
                   {
                       comilla++;
                   }
                   if (comilla == 2)
                   {
                       //Auxiliar = i;
                       break;
                   }
                   else if (cmdDisk->path[i] != '\"')
                   {
                      todo[Auxiliar] = cmdDisk->path[i];
                      Auxiliar++;
                   }
               }

               comilla = 0;

               //CONCATENANDO EL NAME SIN EL PRIMER \"
               for (i = 0; i<100; i++)
               {
                   if (cmdDisk->name[i] == '\"' )
                   {
                       comilla++;
                       if (comilla == 2)
                       {
                           //todo[Auxiliar] = cmdDisk->name[i];
                           break;
                       }
                   }
                   else
                   {
                       todo[Auxiliar] = cmdDisk->name[i];
                       Auxiliar++;
                   }

               }
               int aux;
               for (aux = Auxiliar; aux < 100; aux++)
               {
                   todo[aux] = '\0';
               }
    /********************************************************************************************************************/
               /*strcat(todo, cmdDisk->path);
               strcat(todo, cmdDisk->name);*/



               FILE* archivo = fopen(todo, "wb+");
               if (archivo != NULL)
               {

                   unsigned int tamanio;
                   if (cmdDisk->unit == 'k' || cmdDisk->unit == 'K')
                   {
                       tamanio = 1024 * cmdDisk->size;
                   }
                   else
                   {
                       tamanio = 1024 * 1024 * cmdDisk->size;
                   }

                   char caracter [3] = "\0";
                   int i;
                   for (i= 0; i<= tamanio; i++)
                   {
                       fwrite(caracter, 1, sizeof(caracter), archivo);
                       fseek(archivo, i, SEEK_SET);
                   }
                   //AQUI SE CREA EN MBR DEL ARCHIVO
                  int signature = rand()% 1 + 63;

                   MBR  cmdMBR;
                   cmdMBR.mbr_tamanio = tamanio;
                   cmdMBR.mbr_fecha_creacion = time(NULL);
                   cmdMBR.mbr_disk_signature = signature;

                   int part;
                   for (part = 0; part < 4 ; part++)
                   {
                        cmdMBR.particiones[part].part_status = 'n';
                        cmdMBR.particiones[part].part_type = '\0';
                        strcpy(cmdMBR.particiones[part].part_fit, "\0\0");
                        cmdMBR.particiones[part].part_start = 0;
                        cmdMBR.particiones[part].part_size = 0;
                         strcpy(cmdMBR.particiones[part].part_name, "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0");
                   }

                   fseek(archivo, 0, SEEK_SET);
                   fwrite(&cmdMBR, sizeof(MBR), 1, archivo);

                   fclose(archivo);

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

void ejecutarRMDISK (int i)
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

            strcpy(cmdRDisk->path, SplitComandos);

        }
        else
        {
            strcat(cmdRDisk->path, " ");
            strcat(cmdRDisk->path, *(tokens + r));
        }
    }
    printf("********VALIDANDO EL RMDISK********* \n");

    if (strcmp(cmdRDisk->path, "") != 0)
    {
        int opcion;
        printf("***********¿Seguro que desea borrar el archivo? ************** \n 1. Si \n 2. No \n \n");
        scanf("%d", &opcion);
        fflush(stdin);
        // rmdisk -path::"/home/jjenano/Documentos/Discos/prueba.dsk"
        if (opcion == 1)
        {
            /********************************************************************************************************************/
                       //CONCATENANDO LAS COMILLAS
                       char todo[100];
                       int i, Auxiliar, comilla;
                       comilla = 0;
                       Auxiliar = 0;
                       //CONCATENANDO EL PATH SIN LA ULTIMA \"
                       for (i = 0; i<100; i++)
                       {

                           if (cmdRDisk->path[i] == '\"')
                           {
                               comilla++;
                           }
                           if (comilla == 2)
                           {
                               //Auxiliar = i;
                               break;
                           }
                           else if (cmdRDisk->path[i] != '\"')
                           {
                              todo[Auxiliar] = cmdRDisk->path[i];
                              Auxiliar++;
                           }
                       }
                       int aux;
                       for (aux = Auxiliar; aux < 100; aux++)
                       {
                           todo[aux] = '\0';
                       }
            /********************************************************************************************************************/

            FILE * archivo = fopen(todo, "r+");
            if (archivo != NULL)
            {
               fclose(archivo);
               remove(todo);
            }
            else
            {
                 printf("*************NO EXISTE EL ARCHIVO************* \n");
            }
        }
        else
        {
            printf("*************NO SE ELMINARA EL DISCO************* \n");
        }
    }
    else
    {
        printf("***********ERROR NO SE ESPECIFICO EL PATH************** \n");
    }
}

void ejecutarFDISK (int i)
{
    bool banderaDelete = false, banderaAdd = false;
    Fdisk * cmdFdisk = malloc(sizeof(Fdisk));
    cmdFdisk->unit = 'k';
    cmdFdisk->type = 'p';
    cmdFdisk->fit[0] = 'w';
    cmdFdisk->fit[1] = 'f';
    cmdFdisk->add = 'k';

    int r;

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
            strcpy(cmdFdisk->fit, SplitComandos);

        }
        else if (strncasecmp((*(tokens + r)), "+delete", 7) == 0)
        {
            char * SplitComandos;
            SplitComandos = strtok((*(tokens + r)),"::");
            SplitComandos = strtok(NULL, "::");
            strcpy(cmdFdisk->borrar, SplitComandos);
            banderaDelete = true;

        }
        else if (strncasecmp((*(tokens + r)), "-name", 5) == 0)
        {
            //printf("si entra al name");

            char * SplitComandos;
            SplitComandos = strtok((*(tokens + r)),"::");
            SplitComandos = strtok(NULL, "::");
            strcpy(cmdFdisk->name, SplitComandos);
        }
        else if (strncasecmp((*(tokens + r)), "+add", 4) == 0)
        {
            char * SplitComandos;
            SplitComandos = strtok((*(tokens + r)),"::");
            SplitComandos = strtok(NULL, "::");
            cmdFdisk->add = *SplitComandos;
            banderaAdd = true;
        }
        else
        {
            strcat(cmdFdisk->path, " ");
            strcat(cmdFdisk->path, *(tokens + r));
        }

         // printf("token = [%s] \n", *(tokens + r));
    }

    printf("********VALIDANDO EL FDISK********* \n");

    if (cmdFdisk->size > 0 && strcmp(cmdFdisk->path, "") != 0 && strcmp(cmdFdisk->name, "") != 0)
    {
        //VALIDANDO UNIT
        if ( cmdFdisk->unit == 'k' || cmdFdisk->unit == 'm' || cmdFdisk->unit == 'b' || cmdFdisk->unit == 'K' || cmdFdisk->unit == 'M' || cmdFdisk->unit == 'B')
        {
            //VALIDANDO TYPE
            if ( cmdFdisk->type == 'l' || cmdFdisk->type == 'e' || cmdFdisk->type == 'p' || cmdFdisk->type == 'L' || cmdFdisk->type == 'E' || cmdFdisk->type == 'P')
            {

                //VALIDANDO FIT
                if ( strcasecmp(cmdFdisk->fit, "bf") == 0 || strcasecmp(cmdFdisk->fit, "ff") == 0 || strcasecmp(cmdFdisk->fit, "wf") == 0)
                {
                    //VALIDANDO ADD
                    if ( cmdFdisk->add == 'k' || cmdFdisk->add == 'm' || cmdFdisk->add == 'b' || cmdFdisk->add == 'K' || cmdFdisk->add == 'M' || cmdFdisk->add == 'B')
                    {
                        if (banderaDelete)
                        {
                            //SE BORRARA LA PARTICION

                            FILE * archivo = fopen (cmdFdisk->path, "rb+");

                            MBR  tempMBR;
                            fseek(archivo, 0, SEEK_SET);
                            fread(&tempMBR, sizeof(MBR), 1, archivo);

                            int recopart;
                            for (recopart= 0; recopart < 4; recopart++)
                            {
                                if (strcasecmp(tempMBR.particiones[recopart].part_name, cmdFdisk->name) == 0)
                                {
                                    printf("************ESTA SE DEBE DE BORRAR***************** \n");
                                }
                            }

                            fclose(archivo);
                        }
                        else if (banderaAdd)
                        {

                        }
                        else
                        {
                            //SE CREA LA PARTICION
                            //fdisk -Size::300 -path::"/home/jjenano/Documentos/Discos/prueba.dsk" -name::"Particion1"
                            //fdisk -Size::300 -path::"/home/jjenano/Documentos/Mis Discos/adios.dsk" -name::"Particion1"
                            //fdisk -Size::300 -path::/home/jjenano/Documentos/Discos/prueba.dsk -name::Particion1


                            /********************************************************************************************************************/
                                       //CONCATENANDO LAS COMILLAS
                                       char todo[100];
                                       int i, Auxiliar, comilla;
                                       comilla = 0;
                                       Auxiliar = 0;
                                       //CONCATENANDO EL PATH SIN LA ULTIMA \"
                                       for (i = 0; i<100; i++)
                                       {

                                           if (cmdFdisk->path[i] == '\"')
                                           {
                                               comilla++;
                                           }
                                           if (comilla == 2)
                                           {
                                               //Auxiliar = i;
                                               break;
                                           }
                                           else if (cmdFdisk->path[i] != '\"')
                                           {
                                              todo[Auxiliar] = cmdFdisk->path[i];
                                              Auxiliar++;
                                           }
                                       }
                                       int aux;
                                       for (aux = Auxiliar; aux < 100; aux++)
                                       {
                                           todo[aux] = '\0';
                                       }
                                       strcpy(cmdFdisk->path, todo);


                                       i = 0;
                                       Auxiliar = 0;
                                       comilla = 0;
                                       aux = 0;
                                       //CONCATENANDO EL PATH SIN LA ULTIMA \"
                                       for (i = 0; i<100; i++)
                                       {

                                           if (cmdFdisk->name[i] == '\"')
                                           {
                                               comilla++;
                                           }
                                           if (comilla == 2)
                                           {
                                               //Auxiliar = i;
                                               break;
                                           }
                                           else if (cmdFdisk->name[i] != '\"')
                                           {
                                              todo[Auxiliar] = cmdFdisk->name[i];
                                              Auxiliar++;
                                           }
                                       }

                                       for (aux = Auxiliar; aux < 100; aux++)
                                       {
                                           todo[aux] = '\0';
                                       }
                                       strcpy(cmdFdisk->name, todo);

                            /********************************************************************************************************************/




                            FILE * archivo = fopen (cmdFdisk->path, "rb+");
                            fseek(archivo, 0, SEEK_SET);

                            if (archivo != NULL)
                            {
                                MBR tempMBR;

                                fread(&tempMBR, sizeof(MBR), 1, archivo);

                                int recopart;
                                bool banderaPart = false;
                                for (recopart= 0; recopart < 4; recopart++)
                                {
                                    if (tempMBR.particiones[recopart].part_size != 0)
                                    {
                                        if (strcasecmp(tempMBR.particiones[recopart].part_name, cmdFdisk->name) == 0)
                                        {
                                            printf("************ERROR: NOMBRE DE PARTICIONES DUPLICADO***************** \n");
                                            break;
                                        }
                                    }
                                    else
                                    {
                                        unsigned int tamanioPart;

                                        if (cmdFdisk->unit == 'k' || cmdFdisk->unit == 'K')
                                        {
                                            tamanioPart = 1024 * cmdFdisk->size;
                                        }
                                        else if (cmdFdisk->unit == 'b' || cmdFdisk->unit == 'B')
                                        {
                                            tamanioPart = cmdFdisk->size;
                                        }
                                        else
                                        {
                                            tamanioPart = 1024 * 1024 * cmdFdisk->size;
                                        }


                                        //VALIDANDO SI HAY ESPACIO EN EL DISCO
                                      if  ((tempMBR.mbr_tamanio - tamanioPart) > 0)
                                      {
                                          PARTITON  particion;
                                          particion.part_status = 'n';
                                          particion.part_type = cmdFdisk->type;
                                          strcpy(particion.part_fit, cmdFdisk->fit);
                                          particion.part_size = tamanioPart;
                                          strcpy(particion.part_name, cmdFdisk->name);

                                          //PARTICION PRIMARIA
                                          if (cmdFdisk->type == 'p' || cmdFdisk->unit == 'P')
                                          {
                                                if (recopart != 0)
                                                {
                                                    banderaPart = true;
                                                    particion.part_status = 'y';
                                                    particion.part_start = (tempMBR.particiones[recopart - 1].part_start + tempMBR.particiones[recopart - 1].part_size + 1);
                                                    tempMBR.particiones[recopart] = particion;
                                                }
                                                else
                                                {
                                                    banderaPart = true;
                                                    particion.part_status = 'y';
                                                    particion.part_start = sizeof(MBR) + 1;
                                                    tempMBR.particiones[recopart] = particion;
                                                }
                                          }
                                          //PARTICION EXTENDIDA
                                          else if (cmdFdisk->type == 'e' || cmdFdisk->unit == 'E')
                                          {
                                              printf("salir alla \n");
                                          }
                                          //LOGICA
                                          else
                                          {
                                              printf("todo malo \n");
                                          }

                                          fseek(archivo, 0, SEEK_SET);
                                          fwrite(&tempMBR, sizeof(MBR), 1, archivo);
                                          if (banderaPart)
                                          {
                                              break;
                                          }
                                      }
                                      else
                                      {
                                          printf("************ERROR: NO HAY ESPACIO EN EL DISCO***************** \n");
                                      }
                                    }
                                }
                                fclose(archivo);
                            }
                            else
                            {
                                printf("************ERROR: NO EXISTE EL DISCO***************** \n");
                            }
                        }
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
banderaAdd = false;
banderaDelete = false;
}


void reportMBR(char path_in[100])
{


    /********************************************************************************************************************/
               //CONCATENANDO LAS COMILLAS
               char todo[100];
               int j, Auxiliar, comilla;
               comilla = 0;
               Auxiliar = 0;
               //CONCATENANDO EL PATH SIN LA ULTIMA \"
               for (j = 0; j<100; j++)
               {

                   if (path_in[j] == '\"')
                   {
                       comilla++;
                   }
                   if (comilla == 2)
                   {
                       //Auxiliar = i;
                       break;
                   }
                   else if (path_in[j] != '\"')
                   {
                      todo[Auxiliar] = path_in[j];
                      Auxiliar++;
                   }
               }
               int aux;
               for (aux = Auxiliar; aux < 100; aux++)
               {
                   todo[aux] = '\0';
               }
    /********************************************************************************************************************/





    FILE * reader = fopen(todo, "rb+");
    fseek(reader, 0, SEEK_SET);

    FILE * writer = fopen("/home/jjenano/Documentos/Discos/ReportemBR.txt", "wb+");
    fseek(writer, 0, SEEK_SET);

    MBR aux_mbr;
    int cont= 0;

    fread(&aux_mbr, sizeof(MBR), 1, reader);

    fprintf(writer,"digraph\n{\nrankdir = LR;\nsubgraph clusterEncabezado\n{\nnode[shape = record, height = 0.5];\nlabel=\"Reporte MBR\"");
    int i;
    for (i = 0; i<4 ; i++){
        if (aux_mbr.particiones[i].part_status=='y'){
            fprintf(writer,"Node%d[label = \"%s &#92;n size: %d &#92;n start: %d &#92;n type: %c\"];\n",cont, aux_mbr.particiones[i].part_name, aux_mbr.particiones[i].part_size,
                    aux_mbr.particiones[i].part_start, aux_mbr.particiones[i].part_type);
            cont++;
        }

    }

    fprintf(writer,"}\n\t}");
    fclose(writer);
    fclose(reader);


    //INVOCANDO GRAPHVIZ
    system("dot -Tjpg /home/jjenano/Documentos/Discos/ReportemBR.txt -o /home/jjenano/Documentos/Discos/diagrama.jpg");
    //ABRIENDO LA IMAGEN
    system("xdg-open /home/jjenano/Documentos/Discos/diagrama.jpg &");
}

void ReportDisk(char path_in[140])
{


    /********************************************************************************************************************/
               //CONCATENANDO LAS COMILLAS
               char todo[100];
               int j, Auxiliar, comilla;
               comilla = 0;
               Auxiliar = 0;
               //CONCATENANDO EL PATH SIN LA ULTIMA \"
               for (j = 0; j<100; j++)
               {

                   if (path_in[j] == '\"')
                   {
                       comilla++;
                   }
                   if (comilla == 2)
                   {
                       //Auxiliar = i;
                       break;
                   }
                   else if (path_in[j] != '\"')
                   {
                      todo[Auxiliar] = path_in[j];
                      Auxiliar++;
                   }
               }
               int aux;
               for (aux = Auxiliar; aux < 100; aux++)
               {
                   todo[aux] = '\0';
               }
    /********************************************************************************************************************/





    FILE * reader = fopen(todo, "rb+");
    fseek(reader, 0, SEEK_SET);

    FILE * writer = fopen("/home/jjenano/Documentos/Discos/ReportemBR.txt", "wb+");
    fseek(writer, 0, SEEK_SET);

    MBR aux_mbr;

    fread(&aux_mbr, sizeof(MBR), 1, reader);

    fprintf(writer,"digraph{\nrankdir = TB;\nsubgraph clusterEncabezado{\nnode[shape = record, height = 0.5];\nlabel=\"reporte Disk\"");
    int contadorp=0;
    int i;
    for (i = 0; i<4 ; i++)
    {
        if (aux_mbr.particiones[i].part_status == 'n')
        {
            fprintf(writer,"node%d[label = \" Libre \"];\n", contadorp);
            contadorp++;
        }
        else if (aux_mbr.particiones[i].part_status == 'y')
        {

            if (aux_mbr.particiones[i].part_type == 'p' || aux_mbr.particiones[i].part_type == 'P')
            {
                fprintf(writer,"node%d[label = \"  %s \"];\n",contadorp, aux_mbr.particiones[i].part_name);
                contadorp++;
            }
        }
    }
    fprintf(writer,"nodembr[label=\"MBR\"];\n\t}\n}");
    fclose(writer);

    //INVOCANDO GRAPHVIZ
    system("dot -Tjpg /home/jjenano/Documentos/Discos/ReportemBR.txt -o /home/jjenano/Documentos/Discos/diagrama.jpg");
    //ABRIENDO LA IMAGEN
    system("xdg-open /home/jjenano/Documentos/Discos/diagrama.jpg &");

}


void ejecutarRep (int i)
{
    Rep * cmdRep = malloc(sizeof(Rep));
    int r;
    for (r = (i+1); *(tokens + r); r++)
    {
        if (strncasecmp((*(tokens + r)), "-path", 5) == 0)
        {
            char * SplitComandos;
            SplitComandos = strtok((*(tokens + r)),"::");
            SplitComandos = strtok(NULL, "::");
            strcpy(cmdRep->path, SplitComandos);

        }
        else if (strncasecmp((*(tokens + r)), "-name", 4) == 0)
        {
            //printf("si entra al name");

            char * SplitComandos;
            SplitComandos = strtok((*(tokens + r)),"::");
            SplitComandos = strtok(NULL, "::");
            strcpy(cmdRep->name, SplitComandos);
        }
        else if (strncasecmp((*(tokens + r)), "-id", 3) == 0)
        {
            //printf("si entra al name");

            char * SplitComandos;
            SplitComandos = strtok((*(tokens + r)),"::");
            SplitComandos = strtok(NULL, "::");
            strcpy(cmdRep->id, SplitComandos);
        }
        else if (strncasecmp((*(tokens + r)), "+ruta", 5) == 0)
        {
            //printf("si entra al name");

            char * SplitComandos;
            SplitComandos = strtok((*(tokens + r)),"::");
            SplitComandos = strtok(NULL, "::");
            strcpy(cmdRep->ruta, SplitComandos);
        }
        else if (strncasecmp((*(tokens + r)), "\\", 1) == 0)
        {
            //no hago nada jeje xdXdXD
            printf("**");
        }
        else
        {
            strcat(cmdRep->path, " ");
            strcat(cmdRep->path, *(tokens + r));
        }

         // printf("token = [%s] \n", *(tokens + r));
    }
    printf("********VALIDANDO EL REP********* \n");

    if (strcmp(cmdRep->path, "") != 0 && strcmp(cmdRep->name, "") != 0)
    {
        //rep -path::"/home/jjenano/Documentos/Discos/prueba.dsk" -name::disk -id::"saber"
        //rep -path::"/home/jjenano/Documentos/Mis Discos/adios.dsk" -id::"saber" -name::disk
        //rep -path::"/home/jjenano/Documentos/Discos/prueba.dsk" -name::mbr -id::"saber"
        if (strcasecmp(cmdRep->name, "mbr") == 0)
        {
            CrearCarpeta(cmdRep->path);
            BorrarCarpeta(cmdRep->path);
            reportMBR(cmdRep->path);
        }
        else if (strcasecmp(cmdRep->name, "disk") == 0)
        {
           ReportDisk(cmdRep->path);
        }
    }
    else
    {
        printf("***********ERROR EXISTEN CAMPOS VACIOS************** \n");
    }
}


void  ejecutarMount(int i)
{
    // mount -path::"/home/jjenano/Documentos/Discos/prueba.dsk" -name::"Particion1"

    Mkdisk * cmdMount = malloc(sizeof(Mkdisk));
    int r;
    for (r = (i+1); *(tokens + r); r++)
    {
        if (strncasecmp((*(tokens + r)), "-path", 5) == 0)
        {

            char * SplitComandos;
            SplitComandos = strtok((*(tokens + r)),"::");
            SplitComandos = strtok(NULL, "::");

            strcpy(cmdMount->path, SplitComandos);

        }
        else if (strncasecmp((*(tokens + r)), "-name", 5) == 0)
        {
            //printf("si entra al name");

            char * SplitComandos;
            SplitComandos = strtok((*(tokens + r)),"::");
            SplitComandos = strtok(NULL, "::");
            strcpy(cmdMount->name, SplitComandos);
        }
        else
        {
            strcat(cmdMount->path, " ");
            strcat(cmdMount->path, *(tokens + r));
        }
    }
    printf("********VALIDANDO EL MOUNT********* \n");


    if (strcmp(cmdMount->path, "") != 0 && strcmp(cmdMount->name, "") != 0)
    {
        /********************************************************************************************************************/
        //CONCATENANDO LAS COMILLAS
        char todo[100];
        int j, Auxiliar, comilla;
        comilla = 0;
        Auxiliar = 0;
        //CONCATENANDO EL PATH SIN LA ULTIMA \"
        for (j = 0; j<100; j++)
        {

            if (cmdMount->path[j] == '\"')
            {
                comilla++;
            }
            if (comilla == 2)
            {
                //Auxiliar = i;
                break;
            }
            else if (cmdMount->path[j] != '\"')
            {
                todo[Auxiliar] = cmdMount->path[j];
                Auxiliar++;
            }
        }
        int aux;
        for (aux = Auxiliar; aux < 100; aux++)
        {
            todo[aux] = '\0';
        }

        char ruta[100];
        strcpy(ruta, todo);

        j = 0;
        Auxiliar = 0;
        comilla = 0;
        aux = 0;
        //CONCATENANDO EL PATH SIN LA ULTIMA \"
        for (i = 0; i<100; i++)
        {

            if (cmdMount->name[i] == '\"')
            {
                comilla++;
            }
            if (comilla == 2)
            {
                //Auxiliar = i;
                break;
            }
            else if (cmdMount->name[i] != '\"')
            {
                todo[Auxiliar] = cmdMount->name[i];
                Auxiliar++;
            }
        }

        for (aux = Auxiliar; aux < 100; aux++)
        {
            todo[aux] = '\0';
        }
        strcpy(cmdMount->name, todo);
        /********************************************************************************************************************/
        char idAux[5] = "vd", num[2];
        int fil, col;
        MBR auxMBR;
        bool validacion=false;
        FILE * archivo = fopen(ruta,"rb+");



        if(archivo!=NULL)
        {
            fread(&auxMBR,sizeof(MBR),1,archivo);

            int recopart=0;
            for(recopart=0; recopart<4; recopart++)
            {
                if(strcasecmp(auxMBR.particiones[recopart].part_name, cmdMount->name)==0)
                {
                    validacion=true;
                }
            }

            if (!validacion)
            {
                printf("************LA PARTICION NO EXISTE EN EL DISCO**************");
            }
            else
            {
                int fila, columna;
                for (fila=1; fila < 27;fila++)
                {
                    for(columna=1; columna < 150; columna++)
                    {
                        if(strcasecmp(Montadura[fila][columna].name , cmdMount->name)==0)
                        {
                            printf("**************ERROR ESTA PARTICION YA HA SIDO MONTADO ANTERIORMENTE********************\n");
                            fila=27;
                            break;
                        }
                        else if (strcasecmp(Montadura[fila][columna].name , "")==0)
                        {
                            fil = fila;
                            col = columna;
                            fila = 27;
                            break;
                        }
                    }
                }

                strcpy(Montadura[fil][col].name, cmdMount->name);
                strcpy(Montadura[fil][col].path, cmdMount->path);

                idAux[2] = letras[fila-1];
                sprintf(num,"%d",columna);
                strcat(idAux,num);
                strcpy(Montadura[fil][col].id, idAux);
                Montadura[fil][col].disponible = false;
                printf("PARTICION %s MONTADA CON EXITO DEL DISCO %s CON EL ID: %s \n", Montadura[fil][col].name, Montadura[fil][col].path, Montadura[fil][col].id);
            }

        }
        else
        {
            printf("**********ERROR NO SE ENCONTRO NINGUN ARCHIVO************\n");
        }
    }
}

//LECTURA DEL ARCHIVO DE ENTRADA
void Exec(int i)
{
    // exec -path::"/home/jjenano/Documentos/Discos/p1.sh"
    Mkdisk * cmdExec = malloc(sizeof(Mkdisk));
    int r;
    for (r = (i+1); *(tokens + r); r++)
    {
        if (strncasecmp((*(tokens + r)), "-path", 5) == 0)
        {

            char * SplitComandos;
            SplitComandos = strtok((*(tokens + r)),"::");
            SplitComandos = strtok(NULL, "::");

            strcpy(cmdExec->path, SplitComandos);

        }
        else
        {
            strcat(cmdExec->path, " ");
            strcat(cmdExec->path, *(tokens + r));
        }
    }

    free(tokens);

    /********************************************************************************************************************/
               //CONCATENANDO LAS COMILLAS
               char todo[100];
               int j, Auxiliar, comilla;
               comilla = 0;
               Auxiliar = 0;
               //CONCATENANDO EL PATH SIN LA ULTIMA \"
               for (j = 0; j<100; j++)
               {

                   if (cmdExec->path[j] == '\"')
                   {
                       comilla++;
                   }
                   if (comilla == 2)
                   {
                       //Auxiliar = i;
                       break;
                   }
                   else if (cmdExec->path[j] != '\"')
                   {
                      todo[Auxiliar] = cmdExec->path[j];
                      Auxiliar++;
                   }
               }
               int aux;
               for (aux = Auxiliar; aux < 100; aux++)
               {
                   todo[aux] = '\0';
               }
    /********************************************************************************************************************/



    FILE * archivo = fopen(todo,"r");
    if (archivo == NULL)
    {
        printf("*****NO SE ENCONTRO NINGUN ARCHIVO********");
    }

    while ((read = getline(&line, &len, archivo)) != -1)

    {
        printf("\nLeyendo la cadena de %zu caracteres:\n", read);
        printf("La cadena es: %s \n", line);

        tokens = str_split(line, ' ');

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
                    printf("***************EXEC ESTA EJECUTANDO MKDISK*************** \n");
                    ejecutarMKDISK(i);
                }
                else if (strcasecmp((*(tokens + i)), "rmdisk") ==0 )
                {
                    printf("***************EXEC ESTA EJECUTANDO RMDISK*************** \n");
                    ejecutarRMDISK(i);
                }
                else if (strcasecmp((*(tokens + i)), "fdisk") ==0 )
                {
                    printf("***************EXEC ESTA EJECUTANDO FDISK *************** \n");
                    ejecutarFDISK(i);
                }
                else if (strcasecmp((*(tokens + i)), "exec") ==0 )
                {
                    Exec(i);
                }
                else if (strcasecmp((*(tokens + i)), "mount") ==0 )
                {
                    ejecutarMount(i);
                }
                else
                {
                    printf("***************EXEC ESTA LEYENDO UN COMENTARIO*************** \n");
                    break;
                }

                //free(*(tokens + i));
            }
            free(tokens);
        }
    }

}

int main()
{
    while (d==0)
    {
        printf("Escriba comandos \n");
        fgets(Linea,150,stdin);
        fflush(stdin);
        Linea[strlen(Linea)-1] = '\0';


        char aux =  Linea[strlen(Linea)-1];
        //printf("%c",aux);
        if(aux=='\\'){
            Linea[strlen(Linea)-1] = '\0';
            printf("Escriba comandos \n");
            char aux1[150];
            int b;
            for(b=0;b<150;b++){
                aux1[b]='\0';
            }
            fgets(aux1,150,stdin);
            fflush(stdin);
            strcat(Linea,aux1);

        }

        if (strlen(Linea) > 0)
        {
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
                        ejecutarMKDISK(i);
                    }
                    else if (strcasecmp((*(tokens + i)), "rmdisk") ==0 )
                    {
                        ejecutarRMDISK(i);
                    }
                    else if (strcasecmp((*(tokens + i)), "fdisk") ==0 )
                    {
                        ejecutarFDISK(i);
                    }
                    else if (strcasecmp((*(tokens + i)), "exec") ==0 )
                    {
                        Exec(i);
                    }
                    else if (strcasecmp((*(tokens + i)), "rep") ==0 )
                    {
                        ejecutarRep(i);
                    }
                    else if (strcasecmp((*(tokens + i)), "mount") ==0 )
                    {
                        ejecutarMount(i);
                    }

                    //free(*(tokens + i));
                }
                free(tokens);
            }
        }
    }
    return 0;
}
