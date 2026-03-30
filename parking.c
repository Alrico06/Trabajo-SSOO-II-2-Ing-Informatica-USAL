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
    debug = 0;
    int prioridad = 0;

    if (argc < 3 || argc > 5)
    {

        fprintf(stderr, "Argumentos introducidos: %d, esperados: 2-4\n",argc-1);
        fprintf(stderr, "Recuerde: <retardo> <num_choferes> [D] [PA|PD]\n");
        return -1;
    }

    if(argc==3){

        
        if(atoi(argv[1])<0){
            fprintf(stderr,"Argumentos invalidos (Retardo no valido), recuerde: <retardo> <num_choferes> [D] [PA|PD]\n");
            return -2;
        }
        else if (atoi(argv[2]) <= 0){
            fprintf(stderr,"Argumentos invalidos (num_choferes no valido), recuerde: <retardo> <num_choferes> [D] [PA|PD]\n");
            return -2;
        }

        retardo = atoi(argv[1]);
        num_choferes = atoi(argv[2]);
        debug = 0;
        prioridad = 0;
        
    }

    if(argc==4){

        if(atoi(argv[1])<0){
            fprintf(stderr,"Argumentos invalidos (Retardo no valido), recuerde: <retardo> <num_choferes> [D] [PA|PD]\n");
            return -2;
        }
        else if (atoi(argv[2]) <= 0){
            fprintf(stderr,"Argumentos invalidos (num_choferes no valido), recuerde: <retardo> <num_choferes> [D] [PA|PD]\n");
            return -2;
        }else if(strcmp(argv[3],"D")!=0 && strcmp(argv[3],"PA")!=0 && strcmp(argv[3],"PD")!=0){
            fprintf(stderr,"Argumentos invalidos, recuerde: <retardo> <num_choferes> [D] [PA|PD]\n");
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
            fprintf(stderr,"Argumentos invalidos (Retardo no valido), recuerde: <retardo> <num_choferes> [D] [PA|PD]\n");
            return -2;
        }
        else if (atoi(argv[2]) <= 0){
            fprintf(stderr,"Argumentos invalidos (num_choferes no valido), recuerde: <retardo> <num_choferes> [D] [PA|PD]\n");
            return -2;
        }else if(strcmp(argv[3],"D")!=0 && strcmp(argv[3],"PA")!=0 && strcmp(argv[3],"PD")!=0){
            fprintf(stderr,"Argumentos invalidos, recuerde: <retardo> <num_choferes> [D] [PA|PD]\n");
            return -2;
        }else if(strcmp(argv[4],"D")!=0 && strcmp(argv[4],"PA")!=0 && strcmp(argv[4],"PD")!=0){
            fprintf(stderr,"Argumentos invalidos, recuerde: <retardo> <num_choferes> [D] [PA|PD]\n");
            return -2;
        }else if(strcmp(argv[4],"PD")==0 && strcmp(argv[3],"PA")==0){
            fprintf(stderr,"Argumentos invalidos (Escoga solo un modo de prioridad PA/PD), recuerde: <retardo> <num_choferes> [D] [PA|PD]\n");
            return -2;
        }
        else if(strcmp(argv[3],"PD")==0 && strcmp(argv[4],"PA")==0){
            fprintf(stderr,"Argumentos invalidos (Escoga solo un modo de prioridad PA/PD), recuerde: <retardo> <num_choferes> [D] [PA|PD]\n");
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
    sigaction(SIGTERM, &sa, NULL);

    /*----------Fin de los manejadores de señales-----------*/

    /*----------Creacion de recursos IPC----------*/

    int semaforosLibreria = PARKING_getNSemAforos();
    int memoriaLibreria = PARKING_getTamaNoMemoriaCompartida();
    int memoriaTotal = ALINEAR_A_4(memoriaLibreria + MI_MEMORIA);

    //Semaforos

    semid = semget(IPC_PRIVATE, semaforosLibreria+MIS_SEMAFOROS, IPC_CREAT | 0600);
    if (semid == -1) {
        fprintf(stderr,"[PADRE] Error creando semáforos\n");
        exit(1);
    }
    if (debug)
    fprintf(stderr, "[DEBUG] [PADRE] Array de %d semáforos creado, id=%d\n", semaforosLibreria+MIS_SEMAFOROS, semid);
    
    mis_semaforos = semaforosLibreria;
    if (debug)
    fprintf(stderr, "[DEBUG] [PADRE] Zona ajena a la libreria en indice %d\n", mis_semaforos);
    
    // Memoria compartida

    shmid = shmget(IPC_PRIVATE, memoriaTotal, IPC_CREAT | 0600);
    if (shmid == -1) {
        fprintf(stderr,"[PADRE] Error creando memoria compartida\n");
        if (semctl(semid, 0, IPC_RMID) != 0)
            fprintf(stderr, "[PADRE] Error liberando semaforos\n");
        if (debug)
            fprintf(stderr, "[PADRE] [DEBUG] Recursos liberados, saliendo....\n");
        exit(1);
    }
    if (debug)
    fprintf(stderr, "[DEBUG] [PADRE] Memoria creada, id=%d\n", shmid);

    zona_base = (char *)shmat(shmid, NULL, 0);
    if (zona_base == (char *)-1) {
        fprintf(stderr,"[PADRE] Error adjuntando memoria\n");
        if (shmctl(shmid, IPC_RMID, NULL) != 0)
            fprintf(stderr, "[PADRE] Error liberando memoria compartida\n");
        if (semctl(semid, 0, IPC_RMID) != 0)
            fprintf(stderr, "[PADRE] Error liberando semaforos\n");
        if (debug)
            fprintf(stderr, "[PADRE] [DEBUG] Recursos liberados, saliendo....\n");
        exit(1);
    }
    if (debug)
    fprintf(stderr, "[DEBUG] [PADRE] Zona adjuntada en %p\n", zona_base);
    mi_zona = zona_base + memoriaLibreria;
    if (debug)
    fprintf(stderr, "[DEBUG] [PADRE] Zona ajena a la libreria en %p\n",mi_zona);

    memset(zona_base, 0, memoriaTotal);

    //Buzon

    buzid = msgget(IPC_PRIVATE, IPC_CREAT | 0600);
    if (buzid == -1) {
        fprintf(stderr,"[PADRE] Error creando buzón\n");
        if(shmdt(zona_base)!=0)
            fprintf(stderr, "[PADRE] Error desadjuntando memoria compartida\n");
        if (shmctl(shmid, IPC_RMID, NULL) != 0)
            fprintf(stderr, "[PADRE] Error liberando memoria compartida\n");
        if (semctl(semid, 0, IPC_RMID) != 0)
            fprintf(stderr, "[PADRE] Error liberando semaforos\n");
        if (debug)
            fprintf(stderr, "[PADRE] [DEBUG] Recursos liberados, saliendo....\n");
        exit(1);
    }
    if (debug)
    fprintf(stderr, "[DEBUG] [PADRE] Buzón creado, id=%d\n", buzid);

    /*----------Fin de creacion de recursos IPC----------*/


    /*----------Invocacion a PARKING_inicio----------*/
    TIPO_FUNCION_LLEGADA f_llegadas[4];

    f_llegadas[0] = llegada_primer_ajuste;
    f_llegadas[1] = llegada_siguiente_ajuste;
    f_llegadas[2] = llegada_mejor_ajuste;
    f_llegadas[3] = llegada_peor_ajuste;
    
    if (PARKING_inicio(retardo, f_llegadas, semid, buzid, shmid, debug) == -1) {
        fprintf(stderr, "[PADRE] Error en PARKING_inicio\n");
        manejador_senales(0);
    }


    /*----------Fin de la invocacion a PARKING_inicio----------*/

    /*----------Creacion del proceso temporizador----------*/

    pid_t pid_temp = fork();

    if (pid_temp == -1) {                                                                                                                                                                                                    
        fprintf(stderr, "[PADRE] Error en fork del temporizador\n");
        manejador_senales(0);
    }   
    
    if (pid_temp==0){
        if(debug)
        fprintf(stderr,"[P.TEMPORIZADOR] [PID: %d] [DEBUG] Proceso temporizador creado\n", getpid());

        //Registro de la manejadora

        struct sigaction sa;

        sa.sa_handler = manejador_senales_temp;   
        sigemptyset(&sa.sa_mask); 
        sa.sa_flags = 0;       

        signal(SIGINT, SIG_IGN);
        signal(SIGTERM, SIG_IGN); 
        sigaction(SIGALRM, &sa, NULL);

        //Colocamos alarma y dormimos

        alarm(TIEMPO_TEMPORIZADOR);
        pause();
    }
    /*----------Fin de la creacion del proceso temporizador----------*/
    else
    {
        PARKING_simulaciOn();
        waitpid(pid_temp, NULL, 0);
        manejador_senales(0);
    }
}

void manejador_senales_temp(int sig)
{
    switch(sig){
        case SIGALRM:
            if(debug)
                fprintf(stderr, "\n[P.TEMPORIZADOR] [PID: %d] [DEBUG] He recibido la señal SIGALRM\n",getpid());
            PARKING_fin(1);
            break;
        }
}

void manejador_senales(int sig)
{
    switch(sig){
        case SIGTERM:
        case SIGINT:
        case 0:
            if (debug){
            if (sig == SIGINT)
                fprintf(stderr, "\n[PADRE] [DEBUG] He recibido la señal SIGINT\n");
            if(sig==SIGTERM)
                fprintf(stderr, "\n[PADRE] [DEBUG] He recibido la señal SIGTERM\n");
            if(sig==0)
                fprintf(stderr, "\n[PADRE] [DEBUG] Programa terminado\n");
            }
            if(shmdt(zona_base)!=0)
                fprintf(stderr, "[PADRE] Error desadjuntando memoria compartida\n");
            if (shmctl(shmid, IPC_RMID, NULL) != 0)
                fprintf(stderr, "[PADRE] Error liberando memoria compartida\n");
            if (semctl(semid, 0, IPC_RMID) != 0)
                fprintf(stderr, "[PADRE] Error liberando semaforos\n");
            if (msgctl(buzid, IPC_RMID, NULL)!=0)
                fprintf(stderr, "[PADRE] Error liberando buzon\n");
            exit(0);
            break;
        }
}

/* Detalles para evitar fallo en encina
*valor de los semaforos en estructura
*Memoria multipla de 4 
 */
