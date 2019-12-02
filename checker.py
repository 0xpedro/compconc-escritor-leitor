import sys

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
qtd_exec = 2 * ((int(params[0]) * int(params[2])) + (int(params[1]) * int(params[3]))) # ¯\_(ツ)_/¯ isso ta horroroso

qtd_exec_leitoras = {} # map p guardar quantas execucoes as threads leitoras realizaram
for i in range(nthreads_leitoras): # populando o map com a qtd de execucoes
    qtd_exec_leitoras[i*2] = int(params[2]) # a key é o id das threads

qtd_exec_escritoras = {} # map p guardar quantas execucoes as threads escritoras realizaram
for i in range(nthreads_escritoras): # populando o map com a qtd de execucoes
    qtd_exec_escritoras[i + (i + 1)] = int(params[3]) # a key é o id das threads

log_thread = {} # map p guardar os logs das threads
for i in range(nthreads_leitoras):
    log_thread[i*2] = open("thread"+str(i*2)+".txt", "r")

def entraEscrita(id):
    global var
    var = id
    
    if qtd_exec_escritoras[id] == 0:
        print("Thread escritora executando mais vezes do que deveria")
    
    qtd_exec_escritoras[id] -= 1

def entraLeitura(id_thread):
    global var
    #a = log_thread[id_thread].readlines()
    #print(a[0])


for i in range(qtd_exec):
    linha = log[i+1].split("\n")
    linha = linha[0].split(";")

    if int(linha[0]) % 2 == 0:
        entraLeitura(linha[0])
    else:
        if linha[1] == "entraEscrita()":
            escritoras_ativas += 1
            entraEscrita(int(linha[0]))
            if escritoras_ativas > 1:
                print("escritoras_ativas invalido: %d", escritoras_ativas)
        elif linha[1] == "saiEscrita()":
            escritoras_ativas -= 1
        else:
            print("deu merda " + linha[1])

for i in range(nthreads_escritoras):
    if qtd_exec_escritoras[i + (i + 1)] is not 0:
        print("A thread %d não executou todas as vezes que deveria.", i + (i + 1))
    else:
        print("Todas as escritoras executaram suas iterações corretamente.")