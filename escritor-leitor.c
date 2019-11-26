#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define MSG_ERRO_EXECUCAO "Digite: %s <nthreads_leitor> <nthreads_escritor> <qtd_execucoes_leitura> <qtd_execucoes_escrita> <arquivo_log>\n"

int nthreads_leitoras = 0, nthreads_escritoras = 0, qtd_execucoes_leitura = 0, qtd_execucoes_escrita =0;

pthread_mutex_t mutex;
pthread_cond_t cond_leitor, cond_escritor;

void entraLeitura() {}

void saiLeitura() {}

void entraEscrita() {}

void saiEscrita() {}

void *leitora(void *arg) {}

void *escritora(void *arg) {}

void main(int argc, char *argv[]) {
    pthread_t *threads_leitoras, *threads_escritoras;
    int i, *id_leitoras, *id_escritoras;

    if(argc < 6){
        fprintf(stderr, MSG_ERRO_EXECUCAO, argv[0]);
        exit(-1);
    }

    nthreads_leitoras = atoi(argv[1]);
    nthreads_escritoras = atoi(argv[2]);
    qtd_execucoes_leitura = atoi(argv[3]);
    qtd_execucoes_escrita = atoi(argv[4]);
    char *path_log = argv[5];

    FILE *arquivo_log = fopen(path_log, "w");
    if(arquivo_log == NULL){
        fprintf(stderr, "Erro ao abrir arquivo %s\n", arquivo_log);
        exit(-1);
    }

    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond_leitor, NULL);
    pthread_cond_init(&cond_escritor, NULL);

    threads_leitoras = (pthread_t *)malloc(sizeof(pthread_t) * nthreads_leitoras);
    if (threads_leitoras == NULL){
        printf("--ERRO: malloc do tid leitoras\n");
        exit(-1);
    }

    threads_escritoras = (pthread_t *)malloc(sizeof(pthread_t) * nthreads_escritoras);
    if (threads_escritoras == NULL){
        printf("--ERRO: malloc do tid escritoras\n");
        exit(-1);
    }

    for (i = 0; i < nthreads_leitoras; i++){
        id_leitoras = malloc(sizeof(int));
        if (id_leitoras == NULL){
            printf("--ERRO: malloc do id leitoras\n");
            exit(-1);
        }
        *id_leitoras = i + 1;

        if (pthread_create(&threads_leitoras[i], NULL, leitora, (void *)id_leitoras)){
            printf("--ERRO: pthread create da thread leitora %d\n", i);
            exit(-1);
        }
    }

    for (i = 0; i < nthreads_escritoras; i++){
        id_escritoras = malloc(sizeof(int));
        if (id_escritoras == NULL){
            printf("--ERRO: malloc do id escritoras\n");
            exit(-1);
        }
        *id_escritoras = i + 1;

        if (pthread_create(&threads_escritoras[i], NULL, escritora, (void *)id_escritoras)){
            printf("--ERRO: pthread create da thread escritora %d\n", i);
            exit(-1);
        }
    }

    for (i = 0; i < nthreads_leitoras; i++){
        if (pthread_join(threads_leitoras[i], NULL)){
            printf("--ERRO: pthread join da thread leitora %d\n", i);
            exit(-1);
        }
    }

    for (i = 0; i < nthreads_escritoras; i++){
        if (pthread_join(threads_escritoras[i], NULL)){
            printf("--ERRO: pthread join da thread escritora %d\n", i);
            exit(-1);
        }
    }
}