#include <bits/stdc++.h>

#include "Simulator.cpp"

using namespace std;

int main(int argc, char *argv[])
{
    // Criar objeto Simulator
    Simulator simulador = Simulator();

    // Traduzir o arquivo passado pelo terminal
    simulador.simulate(argv[1]);

    return 0;
}