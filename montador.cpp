#include<bits/stdc++.h>
#include<iostream>

#include "Translator.cpp"

using namespace std;

int main(int argc, char* argv[]){

    // Criar objeto Translator    
    Translator montador = Translator();

    // Traduzir o arquivo passado pelo terminal
    montador.translate(argv[1]);




    return 0;
}