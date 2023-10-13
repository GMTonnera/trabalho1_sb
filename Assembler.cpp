#include<bits/stdc++.h>
#include<fstream>

#include "Parser.cpp"

using namespace std;


class Assembler{
    private:
        // Parser
        Parser parser = Parser();
    
    public:
        Assembler(){}

        void assemble(string file_name){
            // Tokens
            unordered_map<int, vector<string>> tokens;
            // Tabela de símbolos
            unordered_map<string, int> symbol_table;

            // Adquirir os tokens do arquivo por linha          
            tokens = this->parser.extract_text_from_file(file_name);
            
            // for(int i = 1; i <= tokens.size(); i++){
            //     string aux = "";
            //     // cout << tokens[i].size() << "\n";
            //     for(int j = 0; j< tokens[i].size(); j++){
            //         // cout << tokens[i][j] << "\n"; 
            //         aux += tokens[i][j] + " ";
            //     }
            //     cout << i << ": " << aux << "\n";
            // }
            
            // Verificar se existe seção de texto
            int text_section_line = this->parser.check_text_section(tokens);
            if(text_section_line == -1){
                // Erro -> seção de texto inexistente

            }
            
            // Primeira passagem 
            symbol_table = this->parser.make_symbol_table(tokens, text_section_line);
            
            // for(auto i = symbol_table.begin(); i != symbol_table.end(); i++){
            //     cout << i->first << " " << i->second << "\n";
            // }

            // Segunda passagem

        }
};