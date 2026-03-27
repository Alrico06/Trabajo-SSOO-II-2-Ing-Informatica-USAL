#include "lib/parking.h"

//Temporal

int llegada_primer_ajuste(HCoche hc)  { return -2; }
int llegada_siguiente_ajuste(HCoche hc) { return -2; }
int llegada_mejor_ajuste(HCoche hc)   { return -2; }
int llegada_peor_ajuste(HCoche hc)    { return -2; }

//Fin temporal


int main(int argc, char *argv[]){

    /*----------Comienzo de validacion de argumentos----------*/

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

    /*----------Fin de la validacion de argumentos----------*/

    /*----------Comienzo de los manejadores de señales-----------*/

    struct sigaction sa;

    sa.sa_handler = manejador_senales;   
    sigemptyset(&sa.sa_mask);        
    sa.sa_flags = SA_RESTART;

    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGALRM, &sa, NULL);
    sigaction(SIGTERM, &sa, NULL);

    /*----------Fin de los manejadores de señales-----------*/

    /*----------Creacion de recursos IPC----------*/

    int semaforosLibreria = PARKING_getNSemAforos();
    int memoriaLibreria = PARKING_getTamaNoMemoriaCompartida();

    //Semaforos

    semid = semget(IPC_PRIVATE, semaforosLibreria, IPC_CREAT | 0600);
    if (semid == -1) {
        fprintf(stderr,"Error creando semáforos");
        exit(1);
    }
    if (debug)
    fprintf(stderr, "[DEBUG] Array de %d semáforos creado, id=%d\n", semaforosLibreria, semid);

    //Memoria compartida

    shmid = shmget(IPC_PRIVATE, memoriaLibreria, IPC_CREAT | 0600);
    if (shmid == -1) {
        fprintf(stderr,"Error creando memoria compartida");
        exit(1);
    }
    if (debug)
    fprintf(stderr, "[DEBUG] Memoria creada, id=%d\n", shmid);

    zona_base = (char *)shmat(shmid, NULL, 0);
    if (zona_base == (char *)-1) {
        fprintf(stderr,"Error adjuntando memoria");
        shmctl(shmid, IPC_RMID, NULL);
        exit(1);
    }
    if (debug)
    fprintf(stderr, "[DEBUG] Zona adjuntada en %p\n", zona_base);

    //Buzon

    buzid = msgget(IPC_PRIVATE, IPC_CREAT | 0600);
    if (buzid == -1) {
        fprintf(stderr,"Error creando buzón");
        exit(1);
    }
    if (debug)
    fprintf(stderr, "[DEBUG] Buzón creado, id=%d\n", buzid);

    /*----------Fin de creacion de recursos IPC----------*/


    /*----------Invocacion a PARKING_inicio----------*/
    TIPO_FUNCION_LLEGADA f_llegadas[4];

    f_llegadas[0] = llegada_primer_ajuste;
    f_llegadas[1] = llegada_siguiente_ajuste;
    f_llegadas[2] = llegada_mejor_ajuste;
    f_llegadas[3] = llegada_peor_ajuste;

    int resul = PARKING_inicio(retardo, f_llegadas, semid,  buzid,  shmid,  debug);

    printf("%d",resul);

    /*----------Fin de la invocacion a PARKING_inicio----------*/
    return 0;
}




void manejador_senales(int sig)
{
    switch(sig){
        case SIGTERM:
        case SIGINT:
            fprintf(stderr, "\nHe recibido la señal(SIGINT)\n");
            semctl(semid, 0, IPC_RMID);
            shmdt(zona_base);                   /* Desadjuntar */
            shmctl(shmid, IPC_RMID, NULL);      /* Eliminar */
            msgctl(buzid, IPC_RMID, NULL);
            exit(0);
            break;
        case SIGALRM:
            fprintf(stderr, "\nHe recibido la señal(SIGALRM)\n");
            break;
        }
}

/* Detalles para evitar fallo en encina
*valor de los semaforos en estructura
*Memoria multipla de 4 
 */
