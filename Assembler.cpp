#include <bits/stdc++.h>
#include <fstream>

#include "Tables.cpp"

using namespace std;
/*
class Assembler{
    Variáveis:
        > pair<int, int> text_section: armazena as linhas de início e fim da seção de texto
          do programa;

        > pair<int, int> data_section: armazena as linhas de início e fim da seção de dados
          do programa;

        > vector<vector<string>> tokens1: armazena os tokens das linhas do programa fonte na
          ordem original das linhas;

        > vector<vector<string>> tokens2: armazena os tokens das linhas do programa fonte na
          ordem de processamento (seção de texto e seção de dados, repectivamente);

        > unordered_map<string, int> symbol_table: tabela de síbolos

    Métodos:
        > void extract_text_from_file(string file_name): recebe como argumento o nome do arquivo
          fonte e extrai os tokens de cada linha desse arquivo- ignorando comentários e espaços
          e quebra de linhas desnecessários, e os armazena em `tokens1`;

        > void prepare_tokens(): prepara os tokens para a tradução, colocando em `tokens2` os
          tokens da seção de texto e os tokens da seção de dados, respectivamente;

        > bool check_symbol(string symbol): recebe como argumento um símbolo do arquivo fonte
          e verifica se esse símbolo não começa com um número, possui até 30 caracteres e não
          possui nenhum caracter especial além do "_". Se todas as restrições são satisfeitas,
          retorna true. Caso contrário, retorna false;

        > int get_original_line(int n): recebe como parâmetro um número que representa o index
          da variável `tokens2` e retorna i tal que tokens1[i][x] == tokens2[n][x], para todo
          0 <= x < tokens1[i].size().;

        > void first_pass(): realiza a primeira passagem do algoritmo de tradução e constroi a
          tabela de símbolos `symbol_table`. Indica os seguintes erros: símbolo inválido,
          símbolo redefinido e instrução inválida;
}
*/

class Assembler
{
private:
    bool error;
    vector<int> obj_code;
    vector<vector<string>> tokens1;
    vector<vector<string>> tokens2;
    unordered_map<string, int> symbol_table;
    pair<int, int> text_section, data_section;

    void extract_text_from_file(string file_name)
    {
        bool ts = false;
        bool ds = false;
        int line_counter = 0; // Armazena a linha sendo lida
        string line;          // line --> armazena a linha do arquivo
        string token;         // token --> armazena o token da linha

        // asm_file --> objeto arquivo
        ifstream asm_file(file_name);

        // Iterar pela linhas do arquivo, armazenando a string da linha em `line`
        while (getline(asm_file, line))
        {
            // cout << line << "\n";
            // tokens_line --> vetor que armazena os tokens de uma determinada linha
            vector<string> tokens_line;

            // Verificar se tem comentário na linha
            if (line.find(';') != line.npos)
            {
                // Se sim, ignorá-los
                line.erase(line.find(';'), line.length() - line.find(';'));
            }
            // Passar a linha para lower case
            transform(line.begin(), line.end(), line.begin(), ::tolower);

            // ss_line --> objeto stringstream da linha
            stringstream ss_line(line);

            // Iterar por cada token, separado por espaço, e armazená-lo em `token`
            while (ss_line >> token)
            {
                // cout << token << "\n";
                // Colocar o token em `tokens_line`
                tokens_line.push_back(token);
            }
            // Colocar `tokens_line` em `tokens`
            this->tokens1.push_back(tokens_line);
            // Verificar começo da seção de texto
            if (tokens_line.empty())
            {
                line_counter++;
                continue;
            }
            if (tokens_line[0] == "secao" and tokens_line[1] == "text")
            {
                cout << "text\n";
                this->text_section.first = line_counter;
                ts = true;
                if (ds)
                {
                    ds = false;
                    this->data_section.second = line_counter - 1;
                }
            }
            // Verificar começo da seção de dados
            if (tokens_line[0] == "secao" and tokens_line[1] == "data")
            {
                cout << "data\n";
                this->data_section.first = line_counter;
                ds = true;
                if (ts)
                {
                    ts = false;
                    this->text_section.second = line_counter - 1;
                }
            }
            line_counter++;
        }
        // Fechar o arquivo
        asm_file.close();

        if (ds)
        {
            this->data_section.second = line_counter - 1;
        }
        if (ts)
        {
            this->text_section.second = line_counter - 1;
        }
    }

    void prepare_tokens()
    {
        for (int i = this->text_section.first; i <= this->text_section.second; i++)
        {
            this->tokens2.push_back(this->tokens1[i]);
        }
        for (int i = this->data_section.first; i <= this->data_section.second; i++)
        {
            this->tokens2.push_back(this->tokens1[i]);
        }
    }

    bool check_symbol(string symbol)
    {
        // Verificar se o símbolo começa com um dígito
        if (isdigit(symbol[0]))
        {
            // Erro Léxico -> símbolo inválido
            return false;
        }
        // Verificar se o tamanho do símbolo é no máximo 30 caracteres
        if (symbol.length() > 30)
        {
            // Erro Léxico -> símbolo inválido
            return false;
        }
        // Verificar se o símbolo possui algum caracter inválido
        string chars = "abcdefghijklmnopqrstuvwxyz013456789_";
        for (int i = 0; i < symbol.size(); i++)
        {
            if (chars.find(symbol[i]) == chars.npos)
            {
                // Erro Léxico -> símbolo inválido
                return false;
            }
        }

        // Não há erros
        return true;
    }

    int get_original_line(int n)
    {
        for (int i = 0; i < this->tokens1.size(); i++)
        {
            if (this->tokens1[i] == this->tokens2[n])
            {
                return i;
            }
        }
        return -1;
    }

    vector<string> get_args(string str_args)
    {
        vector<string> args;
        string arg;
        stringstream ss_args(str_args);

        while (getline(ss_args, arg, ','))
        {
            args.push_back(arg);
        }
        return args;
    }

    bool check_args(vector<string> args)
    {
        for (int i = 0; i < args.size(); i++)
        {
            string sym;
            if (args[i].find('+') != args[i].npos)
            {
                sym = args[i].substr(0, args[i].find('+'));
            }
            else if (args[i].find('-') != args[i].npos)
            {
                sym = args[i].substr(0, args[i].find('-'));
            }
            else
            {
                sym = args[i];
            }

            if (this->symbol_table.find(sym) == this->symbol_table.end())
            {
                return false;
            }
        }

        return true;
    }

    void gen_obj_code(string inst, vector<string> args)
    {
        if (inst == "const")
        {
            this->obj_code.push_back(stoi(args[0]));
        }
        else if (inst == "space")
        {
            for (int i = 0; i < stoi(args[0]); i++)
            {
                this->obj_code.push_back(0);
            }
        }
        else
        {
            string sym;
            int mem;

            this->obj_code.push_back(OPCODE_TABLE[inst][0]);
            if (inst != "stop")
            {
                if (args[0].find('+') != args[0].npos)
                {
                    sym = args[0].substr(0, args[0].find('+'));
                    mem = this->symbol_table[sym] + stoi(args[0].substr(args[0].find('+')));
                }
                else if (args[0].find('-') != args[0].npos)
                {
                    sym = args[0].substr(0, args[0].find('-'));
                    mem = this->symbol_table[sym] + stoi(args[0].substr(args[0].find('-')));
                    cout << stoi(args[0].substr(args[0].find('-'))) << "\n";
                }
                else
                {
                    sym = args[0];
                    mem = this->symbol_table[sym];
                }
                this->obj_code.push_back(mem);
            }
            if (inst == "copy")
            {
                if (args[1].find('+') != args[1].npos)
                {
                    sym = args[1].substr(0, args[1].find('+'));
                    mem = this->symbol_table[sym] + stoi(args[1].substr(args[1].find('+')));
                }
                else if (args[1].find('-') != args[1].npos)
                {
                    sym = args[1].substr(0, args[1].find('-'));
                    mem = this->symbol_table[sym] + stoi(args[1].substr(args[1].find('-')));
                }
                else
                {
                    sym = args[1];
                    mem = this->symbol_table[sym];
                }
                this->obj_code.push_back(mem);
            }
        }
    }

    void first_pass()
    {
        int location_counter = 0; // Contador de posições

        for (int i = 0; i < this->tokens2.size(); i++)
        {
            // Index da instrução no vetor de tokens da linha
            int inst_pos = 0;

            // Ignorar linhas em branco
            if (this->tokens2[i].empty())
            {
                continue;
            }
            // Ignorar linha com "secao"
            if (this->tokens2[i][0] == "secao")
            {
                continue;
            }

            // Verificar se um símbolo está sendo declarado
            if (this->tokens2[i][0][this->tokens2[i][0].size() - 1] == ':')
            {
                string symbol = this->tokens2[i][0].substr(0, this->tokens2[i][0].size() - 1);
                // Verificar se o símbolo é válido
                if (!this->check_symbol(symbol))
                {
                    // Flag de erro = true
                    this->error = true;
                    // Adquirir linha original do erro
                    int error_line = this->get_original_line(i);
                    // Erro Léxico --> símbolo inválido
                    cout << "(Linha " << error_line + 1 << ") Erro Léxico: símbolo inválido.\n";
                }
                // Verificar se o símbolo já foi definido
                if (this->symbol_table.find(symbol) != this->symbol_table.end())
                {
                    // Flag de erro = true
                    this->error = true;
                    // Adquirir linha original do erro
                    int error_line = this->get_original_line(i);
                    // Erro Semântico --> símbolo já definido
                    cout << "(Linha " << error_line + 1 << ") Erro Semântico: símbolo já definido.\n";
                }
                else
                {
                    // Colocar símbolo na tabela
                    this->symbol_table[symbol] = location_counter;
                }
                // Mudar posição da instrução para 1
                inst_pos = 1;
            }
            // Instrução
            string inst = this->tokens2[i][inst_pos];
            // Verificar se a instrução está na tabela de instruções
            if (OPCODE_TABLE.find(inst) != OPCODE_TABLE.end())
            {
                // Atualizar contador de posições
                location_counter += OPCODE_TABLE[inst][1];
            }
            // Verificar se a instrução está na tabela de diretivas
            else if (DIRECTIVE_TABLE.find(inst) != DIRECTIVE_TABLE.end())
            {
                // Adquirir o argumento da diretiva
                int arg;
                if (this->tokens2[i].size() > 2)
                {
                    arg = stoi(this->tokens2[i][inst_pos + 1]);
                }
                else
                {
                    arg = 1;
                }
                // Atualizar contador de posições de acordo com a subrotina
                location_counter += directive_subroutine(inst, arg);
            }
            // Se não está em nenhuma das tabelas
            else
            {
                // Flag de erro = true
                this->error = true;
                // Adquirir linha original do erro
                int error_line = this->get_original_line(i);
                // Erro Sintático -> instrução inválida
                cout << "(Linha " << error_line + 1 << ") Erro Sintático: instrução inválida.\n";
            }
        }
    }

    // Realiza a segunda passagem
    void second_pass()
    {
        int location_counter = 0; // Contador de posições

        for (int i = 0; i < this->tokens2.size(); i++)
        {
            // Index da instrução no vetor de tokens da linha
            int inst_pos = 0;

            // Ignorar linhas em branco
            if (this->tokens2[i].empty())
            {
                continue;
            }
            // Ignorar linha com "secao"
            if (this->tokens2[i][0] == "secao")
            {
                continue;
            }

            // Verificar se um símbolo está sendo declarado
            if (this->tokens2[i][0][this->tokens2[i][0].size() - 1] == ':')
            {
                // Ignorar símbolo
                inst_pos = 1;
            }
            // Instrução
            string inst = this->tokens2[i][inst_pos];

            // Verificar se a instrução está na tabela de instruções
            if (OPCODE_TABLE.find(inst) != OPCODE_TABLE.end())
            {
                // Lista de argumentos
                vector<string> args;
                if (this->tokens2[i].size() == inst_pos + 1)
                {
                    args = {};
                }
                else
                {
                    args = this->get_args(this->tokens2[i][inst_pos + 1]);
                }
                // Verificar número de operandos
                if (args.size() != OPCODE_TABLE[inst][2])
                {
                    // Flag de erro = true
                    this->error = true;
                    // Adquirir linha original do erro
                    int error_line = this->get_original_line(i);
                    // Erro Sintático -> número de operandos errado
                    cout << "(Linha " << error_line + 1 << ") Erro Sintático: número de operandos errado.\n";
                }
                // Verificar se os operandos estão definidos
                if (!this->check_args(args))
                {
                    // Flag de erro = true
                    this->error = true;
                    // // Adquirir linha original do erro
                    int error_line = this->get_original_line(i);
                    // Erro Semântico -> operando indefinido
                    cout << "(Linha " << error_line + 1 << ") Erro Semântico: operando indefinido.\n";
                }

                // Se o código não possui erros, gerar o código objeto
                if (!this->error)
                {
                    this->gen_obj_code(inst, args);
                }
            }
            // Verificar se a instrução está na lista de diretivas
            else if (DIRECTIVE_TABLE.find(inst) != DIRECTIVE_TABLE.end())
            {
                // Lista de argumentos
                vector<string> args;
                if (this->tokens2[i].size() == inst_pos + 1)
                {
                    args = {};
                }
                else
                {
                    args = this->get_args(this->tokens2[i][inst_pos + 1]);
                }
                // Verificar o número de argumentos
                if (inst == "const" and args.size() != 1)
                {
                    // Flag de erro = true
                    this->error = true;
                    // Adquirir linha original do erro
                    int error_line = this->get_original_line(i);
                    // Erro Sintático -> número de operandos errado
                    cout << "(Linha " << error_line + 1 << ") Erro Sintático: número de operandos errado.\n";
                }
                else if (inst == "space" and args.size() > 1)
                {
                    // Flag de erro = true
                    this->error = true;
                    // Adquirir linha original do erro
                    int error_line = this->get_original_line(i);
                    // Erro Sintático -> número de operandos errado
                    cout << "(Linha " << error_line + 1 << ") Erro Sintático: número de operandos errado.\n";
                }
                // Se não tiver erros, gerar o código objeto
                if (!this->error)
                {
                    if (this->tokens2[i].size() == inst_pos + 1)
                    {
                        args = {"1"};
                    }
                    this->gen_obj_code(inst, args);
                }
            }
        }
    }

    // Cria o arquivo .obj
    void make_obj_file(string file_name)
    {
        ofstream obj_file(file_name + ".OBJ");
        string str_obj_code = "";
        for (int i = 0; i < this->obj_code.size(); i++)
        {
            str_obj_code += to_string(this->obj_code[i]) + " ";
        }
        obj_file << str_obj_code;
        obj_file.close();
    }

    // Printa a lista de tokens1
    void show_tokens1()
    {
        for (int i = 0; i < this->tokens1.size(); i++)
        {
            string aux = "";
            // cout << tokens[i].size() << "\n";
            for (int j = 0; j < this->tokens1[i].size(); j++)
            {
                // cout << tokens[i][j] << "\n";
                aux += this->tokens1[i][j] + " ";
            }
            cout << i + 1 << ": " << aux << "\n";
        }
    }

    // Printa a lista de tokens2
    void show_tokens2()
    {
        for (int i = 0; i < this->tokens2.size(); i++)
        {
            string aux = "";
            // cout << tokens[i].size() << "\n";
            for (int j = 0; j < this->tokens2[i].size(); j++)
            {
                // cout << tokens[i][j] << "\n";
                aux += this->tokens2[i][j] + " ";
            }
            cout << i + 1 << ": " << aux << "\n";
        }
    }

    // Printa a tabela de símbolos
    void show_symbol_table()
    {
        for (auto i = this->symbol_table.begin(); i != this->symbol_table.end(); i++)
        {
            cout << i->first << " = " << i->second << "\n";
        }
    }

    void show_obj_code()
    {
        for (int i = 0; i < this->obj_code.size(); i++)
        {
            cout << this->obj_code[i] << " ";
        }
        cout << "\n";
    }

public:
    Assembler() {}

    void assemble(string file_name)
    {
        // Setar flag de erros para false
        this->error = false;

        // Adquirir os tokens do arquivo por linha
        this->extract_text_from_file(file_name);

        // cout << this->data_section.first << " " << this->data_section.second << "\n";
        // cout << this->text_section.first << " " << this->text_section.second << "\n";
        // this->show_tokens1();

        // cout << "==============================================\n";
        this->prepare_tokens();

        // this->show_tokens2();
        // cout << "==============================================\n";

        // Primeira passagem
        this->first_pass();

        // this->show_symbol_table();
        // cout << "==============================================\n";

        // Segunda passagem
        this->second_pass();

        // this->show_obj_code();
        if (!this->error)
        {
            this->make_obj_file(file_name.substr(0, file_name.find('.')));
        }
    }
};