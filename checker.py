import sys

try:
    l = open(sys.argv[1], "r")
except IOError:
    print("Não foi possível abrir o arquivo " + sys.argv[0] + ".")

if l.mode is not "r":
    exit(-1)

var = -1

log = l.readlines()
params = log[0].split()
qtd_exec = 2 * ((int(params[0]) * int(params[2])) + (int(params[1]) * int(params[3]))) #¯\_(ツ)_/¯

log_thread = []

def entraEscrita(x):
    global var
    var = x

def entraLeitura():
    global var
    print(var)


for i in range(qtd_exec):
    linha = log[i+1].split("\n")
    linha = linha[0].split(";")

    if int(linha[0]) % 2 == 0:
        entraLeitura()
    else:
        entraEscrita(int(linha[2]))