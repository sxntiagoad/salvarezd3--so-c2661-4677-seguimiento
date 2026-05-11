#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>

struct ControlLateral {
    sem_t activacion;
    sem_t accion_disparo;
    sem_t recarga;
    struct ControlLateral *sig_etapa;
    pthread_t id_hilo_control_lateral;
};

struct ControlLateral* iniciarV3(int n_etapas);

void control_proyectil(struct ControlLateral estructura_control[], int n_etapas) {
    int i = 0;
    while (i < n_etapas) {
        sem_post(&estructura_control[i].activacion);
        sem_wait(&estructura_control[i].accion_disparo);
        i++;
    }
}

void control_recarga(struct ControlLateral estructura_control[], int n_etapas) {
    for (int i = n_etapas - 1; i >= 0; i--) {
        sem_post(&estructura_control[i].recarga);
    }
}

static void* hilo_control_disparo_lateral(void *arg) {
    struct ControlLateral *pControlLateral = (struct ControlLateral *) arg;
    for (;;) {
        sem_wait(&pControlLateral->activacion);
        sem_post(&pControlLateral->accion_disparo);
        sem_wait(&pControlLateral->recarga);
    }
}
