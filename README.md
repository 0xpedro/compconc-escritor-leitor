# compconc-escritor-leitor
Repositório contendo o código do segundo trabalho de implementação da disciplina de Computação Concorrente do curso de Bacharelado em Ciência da Computação da UFRJ.

O relatório de desenvolvimento desse trabalho pode ser visto em:
https://docs.google.com/document/d/1w3pVlOBZ7E_ygfBcL0yDDHXbfH7YzcFbdam5297RDjY/edit?usp=sharing

Para compilar o programa em C:
gcc -o <nome_arquivo> escritor-leitor.c -W -lpthread

Para rodar o arquivo gerado:
./<nome_arquivo> <numero_threads_leitoras> <numero_threads_escritoras> <qtd_execucoes_leitoras> <qtd_execucoes_escritoras> <log_file>

Para verificar a corretude:
python checker.py <log_file>
