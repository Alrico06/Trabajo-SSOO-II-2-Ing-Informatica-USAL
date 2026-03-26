#include "lib/parking.h"

int main(int argc, char *argv[]){

    /*Comienzo de validacion de argumentos*/

    int retardo = -1;
    int num_choferes = -1;
    int debug = 0;
    int prioridad = 0;

    if (argc < 3 || argc > 5)
    {

        fprintf(stderr, "Argumentos introducidos: %d, esperados: 2-4\n",argc-1);
        fprintf(stderr, "Recuerde: <retardo> <num_choferes> [D] [PA|PD]\n");
        return -1;
    }

    if(argc==3){

        
        if(atoi(argv[1])<0){
            fprintf(stderr,"Argumentos invalidos (Retardo no valido), recuerde: <retardo> <num_choferes> [D] [PA|PD]\n",argc-1);
            return -2;
        }
        else if (atoi(argv[2]) <= 0){
            fprintf(stderr,"Argumentos invalidos (num_choferes no valido), recuerde: <retardo> <num_choferes> [D] [PA|PD]\n",argc-1);
            return -2;
        }

        retardo = atoi(argv[1]);
        num_choferes = atoi(argv[2]);
        debug = 0;
        prioridad = 0;
        
    }

    if(argc==4){

        if(atoi(argv[1])<0){
            fprintf(stderr,"Argumentos invalidos (Retardo no valido), recuerde: <retardo> <num_choferes> [D] [PA|PD]\n",argc-1);
            return -2;
        }
        else if (atoi(argv[2]) <= 0){
            fprintf(stderr,"Argumentos invalidos (num_choferes no valido), recuerde: <retardo> <num_choferes> [D] [PA|PD]\n",argc-1);
            return -2;
        }else if(strcmp(argv[3],"D")!=0 && strcmp(argv[3],"PA")!=0 && strcmp(argv[3],"PD")!=0){
            fprintf(stderr,"Argumentos invalidos, recuerde: <retardo> <num_choferes> [D] [PA|PD]\n",argc-1);
            return -2;
        }
        retardo = atoi(argv[1]);
        num_choferes = atoi(argv[2]);
        debug = 0;

        prioridad = 0;

        if(strcmp(argv[3],"D")==0)
            debug = 1;
        else if(strcmp(argv[3],"PA")==0)
            prioridad = 1;
        else if(strcmp(argv[3],"PD")==0)
            prioridad = 2;

        if(debug==1)
        printf("[DEBUG] Argumentos introducidos: %s %s %s\n",argv[1],argv[2],argv[3]);
        
    }

    if(argc==5){

        if(atoi(argv[1])<0){
            fprintf(stderr,"Argumentos invalidos (Retardo no valido), recuerde: <retardo> <num_choferes> [D] [PA|PD]\n",argc-1);
            return -2;
        }
        else if (atoi(argv[2]) <= 0){
            fprintf(stderr,"Argumentos invalidos (num_choferes no valido), recuerde: <retardo> <num_choferes> [D] [PA|PD]\n",argc-1);
            return -2;
        }else if(strcmp(argv[3],"D")!=0 && strcmp(argv[3],"PA")!=0 && strcmp(argv[3],"PD")!=0){
            fprintf(stderr,"Argumentos invalidos, recuerde: <retardo> <num_choferes> [D] [PA|PD]\n",argc-1);
            return -2;
        }else if(strcmp(argv[4],"D")!=0 && strcmp(argv[4],"PA")!=0 && strcmp(argv[4],"PD")!=0){
            fprintf(stderr,"Argumentos invalidos, recuerde: <retardo> <num_choferes> [D] [PA|PD]\n",argc-1);
            return -2;
        }else if(strcmp(argv[4],"PD")==0 && strcmp(argv[3],"PA")==0){
            fprintf(stderr,"Argumentos invalidos (Escoga solo un modo de prioridad PA/PD), recuerde: <retardo> <num_choferes> [D] [PA|PD]\n",argc-1);
            return -2;
        }
        else if(strcmp(argv[3],"PD")==0 && strcmp(argv[4],"PA")==0){
            fprintf(stderr,"Argumentos invalidos (Escoga solo un modo de prioridad PA/PD), recuerde: <retardo> <num_choferes> [D] [PA|PD]\n",argc-1);
            return -2;
        }

        retardo = atoi(argv[1]);
        num_choferes = atoi(argv[2]);
        debug = 0;

        prioridad = 0;

        if(strcmp(argv[3],"D")==0 || strcmp(argv[4],"D")==0)
            debug = 1;
        if(strcmp(argv[3],"PA")==0 || strcmp(argv[4],"PA")==0)
            prioridad = 1;
        if(strcmp(argv[3],"PD")==0 || strcmp(argv[4],"PD")==0)
            prioridad = 2;

        if(debug==1)
        printf("[DEBUG] Argumentos introducidos: %s %s %s %s\n",argv[1],argv[2],argv[3],argv[4]);
    }

    /*Fin de la validacion de argumentos*/

    return 0;
}

/* Detalles para evitar fallo en encina
*valor de los semaforos en estructura
*Memoria multipla de 4 
 */

