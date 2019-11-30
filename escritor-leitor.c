#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

#define MSG_ERRO_EXECUCAO "Digite: %s <nthreads_leitor> <nthreads_escritor> <qtd_execucoes_leitura> <qtd_execucoes_escrita> <arquivo_log>\n"

int nthreads_leitoras = 0, nthreads_escritoras = 0, qtd_execucoes_leitura = 0, qtd_execucoes_escrita = 0, leitoras_executando = 0, escritoras_executando = 0;

int var = -1;

FILE *arquivo_log;

pthread_mutex_t mutex;
pthread_cond_t cond_leitor, cond_escritor;

void entraLeitura(int id)
{
    pthread_mutex_lock(&mutex);

    while (escritoras_executando)
        pthread_cond_wait(&cond_leitor, &mutex);

    leitoras_executando++;

    // escrever no log
    fprintf(arquivo_log, "%d;entraLeitura();%d\n", id, leitoras_executando);
    //printf("%d;entraLeitura();%d\n", id, leitoras_executando);

    pthread_mutex_unlock(&mutex);
}

void saiLeitura(int id)
{
    pthread_mutex_lock(&mutex);
    leitoras_executando--;

    if (!leitoras_executando)
        pthread_cond_signal(&cond_escritor);

    // escrever no log
    fprintf(arquivo_log, "%d;saiLeitura();%d\n", id, leitoras_executando);
    //printf("%d;saiLeitura();%d\n", id, leitoras_executando);

    pthread_mutex_unlock(&mutex);
}

void entraEscrita(int id)
{
    pthread_mutex_lock(&mutex);

    while (leitoras_executando || escritoras_executando)
        pthread_cond_wait(&cond_escritor, &mutex);

    escritoras_executando++;

    // escreve no log
    fprintf(arquivo_log, "%d;entraEscrita();%d\n", id, escritoras_executando);
    //printf("%d;entraEscrita();%d\n", id, escritoras_executando);

    pthread_mutex_unlock(&mutex);
}

void saiEscrita(int id)
{
    pthread_mutex_lock(&mutex);

    escritoras_executando--;

    pthread_cond_signal(&cond_escritor);
    pthread_cond_broadcast(&cond_leitor);

    // escreve no log
    fprintf(arquivo_log, "%d;saiEscrita();%d\n", id, escritoras_executando);
    //printf("%d;saiEscrita();%d\n", id, escritoras_executando);

    pthread_mutex_unlock(&mutex);
}

void *leitora(void *arg)
{
    int id = *(int *)arg;
    char *asdasd = malloc(sizeof(*asdasd));
    FILE *log_thread;

    sprintf(asdasd, "thread%d.txt", id);

    log_thread = fopen(asdasd, "w");
    if (log_thread == NULL)
    {
        fprintf(stderr, "Erro ao abrir arquivo %s\n", log_thread);
        exit(-1);
    }

    for (int i = 0; i < qtd_execucoes_leitura; i++)
    {
        entraLeitura(id);
        fprintf(log_thread, "%d\n", var);
        saiLeitura(id);
    }

    fclose(log_thread);
    pthread_exit(NULL);
}

void *escritora(void *arg)
{
    int id = *(int *)arg;

    for (int i = 0; i < qtd_execucoes_escrita; i++)
    {
        entraEscrita(id);
        var = id;
        saiEscrita(id);
    }

    pthread_exit(NULL);
}

void main(int argc, char *argv[])
{
    pthread_t *threads_leitoras, *threads_escritoras;
    int i, *id_leitoras, *id_escritoras;

    if (argc < 6)
    {
        fprintf(stderr, MSG_ERRO_EXECUCAO, argv[0]);
        exit(-1);
    }

    nthreads_leitoras = atoi(argv[1]);
    nthreads_escritoras = atoi(argv[2]);
    qtd_execucoes_leitura = atoi(argv[3]);
    qtd_execucoes_escrita = atoi(argv[4]);
    char *path_log = argv[5];

    arquivo_log = fopen(path_log, "w");
    if (arquivo_log == NULL)
    {
        fprintf(stderr, "Erro ao abrir arquivo %s\n", arquivo_log);
        exit(-1);
    }

    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond_leitor, NULL);
    pthread_cond_init(&cond_escritor, NULL);

    threads_leitoras = (pthread_t *)malloc(sizeof(pthread_t) * nthreads_leitoras);
    if (threads_leitoras == NULL)
    {
        printf("--ERRO: malloc do tid leitoras\n");
        exit(-1);
    }

    threads_escritoras = (pthread_t *)malloc(sizeof(pthread_t) * nthreads_escritoras);
    if (threads_escritoras == NULL)
    {
        printf("--ERRO: malloc do tid escritoras\n");
        exit(-1);
    }

    for (i = 0; i < nthreads_leitoras; i++)
    {
        id_leitoras = malloc(sizeof(int));
        if (id_leitoras == NULL)
        {
            printf("--ERRO: malloc do id leitoras\n");
            exit(-1);
        }

        //leitoras possuem id par
        *id_leitoras = i * 2;

        if (pthread_create(&threads_leitoras[i], NULL, leitora, (void *)id_leitoras))
        {
            printf("--ERRO: pthread create da thread leitora %d\n", i);
            exit(-1);
        }
    }

    for (i = 0; i < nthreads_escritoras; i++)
    {
        id_escritoras = malloc(sizeof(int));
        if (id_escritoras == NULL)
        {
            printf("--ERRO: malloc do id escritoras\n");
            exit(-1);
        }

        //escritoras possuem id impar
        *id_escritoras = i + (i + 1);

        if (pthread_create(&threads_escritoras[i], NULL, escritora, (void *)id_escritoras))
        {
            printf("--ERRO: pthread create da thread escritora %d\n", i);
            exit(-1);
        }
    }

    for (i = 0; i < nthreads_leitoras; i++)
    {
        if (pthread_join(threads_leitoras[i], NULL))
        {
            printf("--ERRO: pthread join da thread leitora %d\n", i);
            exit(-1);
        }
    }

    for (i = 0; i < nthreads_escritoras; i++)
    {
        if (pthread_join(threads_escritoras[i], NULL))
        {
            printf("--ERRO: pthread join da thread escritora %d\n", i);
            exit(-1);
        }
    }

    fclose(arquivo_log);
}