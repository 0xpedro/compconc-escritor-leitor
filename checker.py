import sys

if len(sys.argv) < 2:
    print("Digite " + sys.argv[0] + " <arquivo_log>")
    exit()

try:
    l = open(sys.argv[1], "r")
except IOError:
    print("Não foi possível abrir o arquivo " + sys.argv[0] + ".")
if l.mode is not "r":
    sys.exit()

var = -1 # variavel compartilhada entre as threads
escritoras_ativas = 0 # qtd de escritoras ativas, tem de ser 0 ou 1

log = l.readlines() # log do programa
params = log[0].split() # primeira linha do log contem os args que o programa recebeu (nthreads, qtd_execucao)
nthreads_leitoras = int(params[0])
nthreads_escritoras = int(params[1])
n_exec_l = int(params[2])
n_exec_e = int(params[3])
qtd_exec = 2 * ((int(params[0]) * int(params[2])) + (int(params[1]) * int(params[3]))) # ¯\_(ツ)_/¯ isso ta horroroso

qtd_exec_leitoras = {} # map p guardar quantas execucoes as threads leitoras realizaram
for i in range(nthreads_leitoras): # populando o map com a qtd de execucoes
    qtd_exec_leitoras[i*2] = int(params[2]) # a key é o id das threads

qtd_exec_escritoras = {} # map p guardar quantas execucoes as threads escritoras realizaram
for i in range(nthreads_escritoras): # populando o map com a qtd de execucoes
    qtd_exec_escritoras[i + (i + 1)] = int(params[3]) # a key é o id das threads

log_thread = {} # map p guardar os logs das threads
for i in range(nthreads_leitoras):
    file_name = "thread" + str(i*2) + ".txt"
    with open(file_name) as f:
        log_thread[i*2] = f.readlines()

def entraEscrita(id):
    global var
    var = id # guarda o id da thread na variavel
    
    if qtd_exec_escritoras[id] == 0: # verifica se essa thread executou mais vezes do que deveria
        print("Thread escritora executando mais vezes do que deveria.")
    
    qtd_exec_escritoras[id] -= 1 # diminui o numero de execucoes que ainda restam para determinada thread

def entraLeitura(id):
    global var
    if qtd_exec_leitoras[id] == 0:
       print("Thread leitora executando mais vezes do que deveria.")
    execucao = n_exec_l - qtd_exec_leitoras[id]
    valor_lido = int(log_thread[id][execucao].rstrip("\n"))
    if valor_lido != var:
        print("Thread " + str(id) + " leu um valor incorreto.")
    qtd_exec_leitoras[id] -= 1


for i in range(qtd_exec):
    linha = log[i+1].split("\n") # pega uma linha do log
    linha = linha[0].split(";") # splita a linha em 3 posicoes <id> <funcao> <valor_da_variavel>

    if int(linha[0]) % 2 == 0: # se id é par, thread leitora
        if linha[1] == "entraLeitura()":
            entraLeitura(int(linha[0])) # x thread executa a operação de leitura
    else: # id é impar, então thread escritora
        if linha[1] == "entraEscrita()":
            escritoras_ativas += 1 # incrementa o numero de threads escritoras ativas, tem de ser 0 ou 1
            entraEscrita(int(linha[0])) # x thread executa a operação de escrita
            if escritoras_ativas > 1: # se mais de uma thread esta escrevendo ao mesmo tempo, deu ruim
                print("Mais de uma thread escritora ativa, a execução está incorreta. threads: " + str(escritoras_ativas))
        elif linha[1] == "saiEscrita()": # se a operação de escrita é saiEscrita(), decrementa o número de escritoras ativas
            escritoras_ativas -= 1
        else:
            print("sei la mano qq isso  " + linha[1])

for i in range(nthreads_escritoras):
    if qtd_exec_escritoras[i + (i + 1)] is not 0: # verifica se escreveu o numero esperado de vezes
        print("A thread " + str(i + (i + 1)) + " não executou todas as vezes que deveria.")

for i in range(nthreads_leitoras):
    if qtd_exec_leitoras[i * 2] is not 0: # verifica se leu o numero esperado de vezes
        print("A thread " + str(i * 2) + " não executou todas as vezes que deveria.")