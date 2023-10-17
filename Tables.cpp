#include <bits/stdc++.h>

using namespace std;

// {"instrução", {opcode, espaço_memória, n_operandos}}
unordered_map<string, vector<int>> OPCODE_TABLE = {
    {"add", {1, 2, 1}},
    {"sub", {2, 2, 1}},
    {"mul", {3, 2, 1}},
    {"div", {4, 2, 1}},
    {"jmp", {5, 2, 1}},
    {"jmpn", {6, 2, 1}},
    {"jmpp", {7, 2, 1}},
    {"jmpz", {8, 2, 1}},
    {"copy", {9, 3, 2}},
    {"load", {10, 2, 1}},
    {"store", {11, 2, 1}},
    {"input", {12, 2, 1}},
    {"output", {13, 2, 1}},
    {"stop", {14, 1, 0}}};

// {{"diretiva", {espaço_memória, argumentos}}}
unordered_map<string, vector<int>> DIRECTIVE_TABLE = {
    {"const", {1, 1}},
    {"space", {1, 1}},
};

int directive_subroutine(string directive, int argument)
{
    /*
    A diretiva const coloca o argumento no local da memória,
    então ocupa apenas 1 posição.A diretiva space reserva o
    número de espaços de memória indicados pelo argumento,
    então ocupa argumento posiçõs.
    */
    // Verificar se é a diretiva const
    if (directive == "const")
    {
        return 1;
    }
    else
    {
        // Se não, é a diretiva space
        return argument;
    }
}