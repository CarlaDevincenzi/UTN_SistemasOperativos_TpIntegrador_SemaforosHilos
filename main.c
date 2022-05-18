#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <windows.h>

#define CANT_INVITADOS 4
#define N_platos CANT_INVITADOS+1 ///los 4 invitados + Mirtha
#define PREGPOLEMICA 2


sem_t sit_invitados[CANT_INVITADOS];
sem_t sit_Mirtha;
sem_t servir;
sem_t comer_invitados;
sem_t come_Mirtha;
sem_t pregunta_polemica1;
sem_t pregunta_polemica2;
sem_t respuesta_polemica1;
sem_t respuesta_polemica2;
sem_t resp2;
sem_t enojarse;
sem_t levantarse;


///variables globales
int inv_sentados = 0;
int mirtha_sentada = 0;
int platos_servidos = 0;
int comio_Mirtha = 0;
int respuesta = 0;
int id_respondio;

void * procesoInvitados(void* param){
        int i = (int)param;

        sem_wait(&sit_invitados[i]);
        inv_sentados++;
        sleep(1);
        printf("Se sento el invitado Nro.: %i\n", pthread_self());
        if(inv_sentados < CANT_INVITADOS){
            sem_post(&sit_invitados[i+1]);
        }else{
            if(inv_sentados == CANT_INVITADOS){
                sleep(1);
                sem_post(&sit_Mirtha);        ///se libera el semaforo para que se siente Mirtha
            }
        }

        sem_wait(&comer_invitados);           ///liberados por el proceso mozos
        printf("Empieza a comer el invitado %i\n", pthread_self());

        sem_wait(&pregunta_polemica1);        /// los libera Mirtha

        sem_wait(&respuesta_polemica1);      /// iniciado en 1: se bloquean todos menos el que responde. Los libera Mirtha
        if(respuesta == 0){
            printf("----- El invitado %i responde la primera pregunta\n", pthread_self());
            respuesta = 1;        //para controlar cuando se liberan los demas, que no respondan la pregunta
            id_respondio = pthread_self(); // guardo el id del proceso que contesto

            sem_post(&pregunta_polemica2);      /// el que responde libera la pregunta 2

        }

        sem_wait(&respuesta_polemica2);       /// aca quedan todos bloqueados. Los libera Mirtha
        int id = pthread_self();
        if(id != id_respondio){            ///verifica que no responda el primero que respondio antes
            sem_wait(&resp2);             /// inicio en 1, entra uno solo. Quedan bloqueados 2. Los libera Mirtha
            if(respuesta == 1){
                respuesta = 2;           ///entra uno solo y la setea en 2
                printf("----- El invitado %i responde la segunda pregunta\n", pthread_self());

                sem_post(&enojarse);      ///libera a Mirtha para enojarse
            }
        }

        printf("Termino de comer el invitado %i\n", pthread_self());

        sem_wait(&levantarse);         /// los libera Mirtha

        printf("Se ha levantado el invitado %i\n", pthread_self());
        pthread_exit(0);
}

void* procesoMirthaLeBig(void* param){
    sem_wait(&sit_Mirtha);
    printf("Mirtha Le Big se acaba de sentar\n\n");
    //mirtha_sentada = 1;
    sleep(2);
    sem_post(&servir);         ///libera el semaforo para que los mozos sirvan los platos

    sem_wait(&come_Mirtha);    ///liberado por mozos
    //sleep(1);
    printf("Empezo a comer la Sra. Mirtha Le Big\n");
    //sleep(5);
    comio_Mirtha = 1;

    if(comio_Mirtha == 1){
        printf("Termino de comer la Sra. Mirtha Le Big\n");
        int p = 0;
        while(p < PREGPOLEMICA){
            sleep(1);
            printf("+++++++ Lanzando pregunta polemica Nro.: %i\n", ++p);
            sleep(1);
            if(p < PREGPOLEMICA){
                for(int i = 0; i < CANT_INVITADOS; i++){
                    sem_post(&pregunta_polemica1);     ///bloqueado en invitados
                }

                sem_wait(&pregunta_polemica2);        /// lo libera invitados

                for(int i = 0; i < CANT_INVITADOS-1; i++){
                    sem_post(&respuesta_polemica1);   ///libera 3 bloqueados en invitados
                }

            }else if(p == PREGPOLEMICA){
                int v;
                sem_getvalue(&respuesta_polemica2, &v);
                printf("El valor del semaforo respuesta polemica 2 es: %i\n", v);

                for(int i = 0; i < CANT_INVITADOS; i++){
                    sem_post(&respuesta_polemica2);    ///libera a todos bloqueados  en invitados
                }
            }
        }

        sem_wait(&enojarse);
        printf("Mirtha se ha enojado por la 2da. respuesta\n");
        printf("Mirtha se ha levantado y retirado del estudio\n");
        int va;
        sem_getvalue(&resp2, &va);
        printf("El valor del semaforo resp2 es: %i\n", va);
        for(int i = 0; i < CANT_INVITADOS-2; i++){ /// libera a los que habian quedado bloqueados, menos los 2 que respondieron
            sem_post(&resp2);
        }

        for(int i = 0; i < CANT_INVITADOS; i++){
            sleep(1);
            sem_post(&levantarse);
        }
    }
    pthread_exit(0);
}

void* procesoMozos(void* param){
    sem_wait(&servir);
    while(platos_servidos < N_platos){
        platos_servidos++;
        printf("Sirviendo el plato Nro.: %i\n", platos_servidos);
        ///printf("Mozo Nro: %i\n", pthread_self()); puse esta linea para verificar que los 2 mozos ejecutaran alternadamente
        sleep(1);
        sem_post(&servir);
    }

    if(platos_servidos == N_platos){
        printf("\n");
        sem_post(&come_Mirtha);
        //sleep(1);
        for(int i = 0; i < CANT_INVITADOS; i ++){
            sem_post(&comer_invitados);
            sleep(1);            ///aca controla que los hilos invitados coman de a 1 segundos
        }
    }
    pthread_exit(0);
}

int main()
{
    printf("***** BIENVENIDOS A ALMORZANDO CON MIRTHA LE BIG *****\n\n\n");

    /// Inicializacion de semaforos
    sem_init(&sit_invitados[0], 0, 1);

    for(int i = 1; i < CANT_INVITADOS; i++){ ///este sincroniza que se sienten en orden los invitados
        sem_init(&sit_invitados[i], 0, 0);
    }

    sem_init(&sit_Mirtha, 0, 0);
    sem_init(&servir, 0, 0);

    sem_init(&comer_invitados, 0, 0);

    sem_init(&come_Mirtha, 0, 0);
    sem_init(&pregunta_polemica1, 0, 0);
    sem_init(&pregunta_polemica2, 0, 0);
    sem_init(&respuesta_polemica1, 0, 1); ///uno solo puede responderla
    sem_init(&respuesta_polemica2, 0, 0); /// lo libera Mirtha
    sem_init(&resp2, 0, 1);
    sem_init(&enojarse, 0, 0);
    sem_init(&levantarse, 0, 0);

    /// Declaracion de hilos
    pthread_t invitados[CANT_INVITADOS];
    pthread_t mirtha;
    pthread_t mozo1;
    pthread_t mozo2;

    ///CREATE DE HILOS
    /* invitados */
    for(int i = 0; i < CANT_INVITADOS; i++){
        pthread_create(&invitados[i], NULL, procesoInvitados, i);
    }

    /* Mirtha Le Big*/
    pthread_create(&mirtha, NULL, procesoMirthaLeBig, NULL);

    /* mozos */
    pthread_create(&mozo1, NULL, procesoMozos, NULL);
    pthread_create(&mozo2, NULL, procesoMozos, NULL);


    ///JOIN DE HILOS
    for(int i = 0; i < CANT_INVITADOS; i++){
        pthread_join(invitados[i], 0);
    }

    pthread_join(mirtha, 0);

    pthread_join(mozo1, 0);
    pthread_join(mozo2, 0);


    ///Completar destroy sem
//    sem_destroy();
//    sem_destroy();
//    sem_destroy();
//    sem_destroy();
//    sem_destroy();
//    sem_destroy();
//    sem_destroy();
//    sem_destroy();
//    sem_destroy();
//    sem_destroy();


    return 0;
}
