#include<bits/stdc++.h>

#include "Tables.cpp"

using namespace std;

class Parser{
    private:
        bool check_symbol(string symbol){
            // Verificar se o símbolo começa com um dígito
            if(isdigit(symbol[0])){
                // Erro -> símbolo inválido
                return false;
            }
            // Verificar se o tamanho do símbolo é no máximo 30 caracteres
            if(symbol.length() > 30){
                // Erro -> símbolo inválido
                return false;
            }
            // Verificar se o símbolo possui algum caracter inválido
            string chars = "abcdefghijklmnopqrstuvwxyz013456789_";
            for(int i = 0; i < symbol.size(); i++){
                if(chars.find(symbol[i]) == chars.npos){
                    // Erro -> símbolo inválido
                    return false;
                }
            }

        }

    public:
        Parser(){}

        // Extrai o texto do arquivo, ignorando os comentários
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

                // Verificar se tem comentário na linha
                if(line.find(';') != line.npos){
                    // Se sim, ignorá-los
                    line.erase(line.find(';'), line.length()-line.find(';'));
                }

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

        // Verifica se existe seção de texto no programa. Se sim, retorna a
        // linha de início da seção, se não retorna -1.
        int check_text_section(unordered_map<int, vector<string>> &tokens){
            for(int i = 1; i < tokens.size(); i++){
                if(tokens[i][0] == "secao" and tokens[i][1] == "text"){
                    return i;
                }
            }
            return -1;
        }

        // Realiza a primeira passagem e retorna a tabela de símbolos
        unordered_map<string, int> make_symbol_table(unordered_map<int, vector<string>> &tokens, int text_section_line){
            // Contador de posição de memória
            int location_counter = 0;
            // Tabela de símbolos
            unordered_map<string, int> symbol_table;

            // Procurar rótulos na seção de texto
            for(int i = text_section_line; i <= tokens.size(); i++){
                if(tokens[i].empty()){
                    continue;
                }
                int x = 0;
                // cout << tokens[i][0] << "\n";
                // Verificar se o primeiro token da linha é um rótulo
                if(tokens[i][0].find(':') != tokens[i][0].npos){
                    // Se sim, verificar se o rótulo é um rótulo válido
                    if(!this->check_symbol(tokens[i][0].substr(0, tokens[i][0].find(':')))){
                        // Erro -> símbolo inválido

                    }
                    // Verificar se esse rótulo já está na tabela
                    if(symbol_table.find(tokens[i][0].substr(0, tokens[i][0].find(':'))) != symbol_table.end()){
                        // Se já estiver, erro variável já definida

                    } else{
                        // Se não, colocá-lo na tabela
                        symbol_table[tokens[i][0].substr(0, tokens[i][0].find(':'))] = location_counter;
                    }
                    x = 1;
                }

                // Verificar se a instrução está na tabela de instruções
                if(OPCODE_TABLE.find(tokens[i][x]) != OPCODE_TABLE.end()){
                    // Se sim, atualizar o contador de posições
                    location_counter += OPCODE_TABLE[tokens[i][x]][1];
                    // cout << "ok\n";
                } else{
                    // Se não, verificar se a instrução está na tabela de diretivas
                    if(DIRECTIVE_TABLE.find(tokens[i][x]) != DIRECTIVE_TABLE.end()){
                        // Se sim, realizar a subrotina referente à diretiva
                        // Verificar se é a diretiva const
                        if(tokens[i][x] == "const"){
                            // Se sim, reservar 1 espaço
                            location_counter++;
                        } else{
                            // Se não, é a diretiva space
                            // Verificar se tem argumento
                            if(tokens[i].size() == 2){
                                // Se não, reservar 1 espaço
                                location_counter++;
                            } else{
                                // Se sim, reservar n espaços
                                location_counter += stoi(tokens[i][x+1]);
                            }
                        }
                    } else{
                        // Se não, erro de instrução não identificada
                    }
                }
            }

            // Procurar símbolos na seção de dados
            for(int i = 1; i < text_section_line; i++){
                if(tokens[i].empty()){
                    continue;
                }
                int x = 0;
                // cout << tokens[i][0] << "\n";
                // Verificar se o primeiro token da linha é um rótulo
                if(tokens[i][0].find(':') != tokens[i][0].npos){
                    // Se sim, verificar se o rótulo é um rótulo válido
                    if(!this->check_symbol(tokens[i][0].substr(0, tokens[i][0].find(':')))){
                        // Erro -> símbolo inválido
                        
                    }
                    // Verificar se esse rótulo já está na tabela
                    if(symbol_table.find(tokens[i][0].substr(0, tokens[i][0].find(':'))) != symbol_table.end()){
                        // Se já estiver, erro variável já definida

                    } else{
                        // Se não, colocá-lo na tabela
                        symbol_table[tokens[i][0].substr(0, tokens[i][0].find(':'))] = location_counter;
                    }
                    x = 1;
                }
                // Verificar se a instrução está na tabela de instruções
                if(OPCODE_TABLE.find(tokens[i][x]) != OPCODE_TABLE.end()){
                    // Se sim, atualizar o contador de posições
                    location_counter += OPCODE_TABLE[tokens[i][x]][1];
                } else{
                    // Se não, verificar se a instrução está na tabela de diretivas
                    if(DIRECTIVE_TABLE.find(tokens[i][x]) != DIRECTIVE_TABLE.end()){
                        // Se sim, realizar a subrotina referente à diretiva
                        // Verificar se é a diretiva const
                        if(tokens[i][x] == "const"){
                            // Se sim, reservar 1 espaço
                            location_counter++;
                        } else{
                            // Se não, é a diretiva space
                            // Verificar se tem argumento
                            if(tokens[i].size() == 2){
                                // Se não, reservar 1 espaço
                                location_counter++;
                            } else{
                                // Se sim, reservar n espaços
                                location_counter += stoi(tokens[i][x+1]);
                            }
                        }
                    } else{
                        // Se não, erro de instrução não identificada
                    }
                }    
            }
            return symbol_table;
        }
};