#include<bits/stdc++.h>
#include<iostream>

#include "Assembler.cpp"

using namespace std;

int main(int argc, char* argv[]){

    // Criar objeto Assembler    
    Assembler montador = Assembler();

    // Traduzir o arquivo passado pelo terminal
    montador.assemble(argv[1]);




    return 0;
}