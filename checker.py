import sys

try:
    l = open(sys.argv[1], "r")
except IOError:
    print("Não foi possível abrir o arquivo " + sys.argv[0] + ".")

if l.mode is not "r":
    exit(-1)

log = l.readlines()

params = log[0].split()
qtd_exec = 2 * ((int(params[0]) * int(params[2])) + (int(params[1]) * int(params[3]))) #¯\_(ツ)_/¯

var = -1

log_thread = []

for i in range(qtd_exec):
    linha = log[i+1].split(";")
    print(linha[0])
    
    if int(linha[0]) % 2 == 0:
        print("leitora")
    else:
        print("escritora")

def entraEscrita(x):
    var = int(x)

def entraLeitura():
    return var