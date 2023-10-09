#include<bits/stdc++.h>

#include "Tables.cpp"

using namespace std;

class Parser{
    private:
        
    public:
        Parser(){}

        unordered_map<int, vector<string>> extract_text_from_file(string file_name){
            // line_counter --> armazena a linha sendo lida
            // line --> armazena a linha do arquivo
            // token --> armazena o token da linha
            int line_counter = 1;
            string line, token;

            // tokens --> armazena os tokens por linha
            unordered_map<int, vector<string>> tokens;
 
            // asm_file --> objeto arquivo
            ifstream asm_file(file_name);

            // Iterar pela linhas do arquivo, armazenando a string da linha em `line`
            while(getline(asm_file, line)){
                // cout << line << "\n";
                // tokens_line --> vetor que armazena os tokens de uma determinada linha
                vector<string> tokens_line;
                // ss_line --> objeto stringstream da linha
                stringstream ss_line(line);
 
                // Iterar por cada token, separado por espaço, e armazená-lo em `token`
                while(ss_line >> token){
                    // cout << token << "\n";
                    // Passar o token para lower case
                    transform(token.begin(), token.end(), token.begin(), ::tolower);
                    // Colocar o token em `tokens_line`
                    tokens_line.push_back(token);
                }
                // Colocar `tokens_line` em `tokens`
                tokens[line_counter] = tokens_line;
                line_counter++;
            }   
            // Fechar o arquivo
            asm_file.close();

            return tokens;
        }


        void erase_comments(unordered_map<int, vector<string>> &tokens){
            for(int i = 1; i < tokens.size(); i++){
                for(int j = 0; j < tokens[i].size(); j++){ 
                    string token = tokens[i][j];
                    if(token.find(';') != token.npos){                        
                        token.erase(token.find(';'), token.length()-token.find(';'));
                        tokens[i].erase(tokens[i].begin()+j, tokens[i].end());
                    }    
                }
            }
        }


        unordered_map<string, int> make_symbol_table(unordered_map<int, vector<string>> &tokens){
            // Contador de posição de memória
            int location_counter = 0;
            // Tabela de símbolos
            unordered_map<string, int> symbol_table;

            for(int i = 0; i < tokens.size(); i++){
                for(int j = 0; j < tokens[i].size(); j++){
                    if(tokens[i][j].find(':') != tokens[i][j].npos){
                        symbol_table[tokens[i][j].substr(0, tokens[i][j].find(':'))] = location_counter;
                    } 
                    else if(OPCODE_TABLE.count(tokens[i][j]) != 0){
                        location_counter += OPCODE_TABLE[tokens[i][j]][1];
                    } 
                    else if(DIRECTIVE_TABLE.count(tokens[i][j]) != 0){
                        location_counter += directive_subroutine(tokens[i][j], stoi(tokens[i][j+1]));
                    }
                }
            }
        }
}; 