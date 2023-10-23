#include <bits/stdc++.h>
#include <fstream>

using namespace std;

class Macros
{
private:

  // Insere o Body da macro chamada no código 
  void insert_body(string line)
  {
    // Pega nome do rótulo da macro no hash e coloca seu body substituindo os argumentos.
    vector<string> argsRotulos;
    // Cria uma lista com lista de strings para substituir o argumento
    vector<vector<string>> change_args;
    
    
    // Armazena o rótulo da macro e seus argumentos em um hash
    stringstream ss(line);
    while (ss >> line)
    {
      argsRotulos.push_back(line);
    }

    string key_name = argsRotulos[0];

    cout << argsRotulos.size() << "\n";

    // Observa se a macro não tem argumentos e coloca o body na lista
    if(argsRotulos.size() == 1){
      cout << "tem 1" << "\n";

      const vector<string>& valores = macroMap[key_name];
      for (const string& valor : valores) {
        if(valor.find("MACRO") == string::npos){
          arq_out.push_back(valor);
          cout << valor << endl;
        }
      }
    }

    else if(argsRotulos.size() == 2){
      cout << "tem 2" << "\n";
      string arg1_substituto = argsRotulos[1];

      for(int i = 0; i < macroMap[key_name].size(); i++){
        vector<string> vetor_linha;   
        string content = macroMap[key_name][i];
        stringstream ss(content);
        while (ss >> content)
        {
          
          vetor_linha.push_back(content);
          
        }
        change_args.push_back(vetor_linha);
      }
      string arg1 = change_args[0][1];

      // for(int i = 0; i < change_args.size(); i++){
      //   change_args[i][1].find(arg1) = arg1_substituto;
      // }

      // for (const auto& vetorInterno : change_args) {
      //   for (const std::string& elemento : vetorInterno) {
      //       std::cout << elemento << " ";
      //   }
      //   std::cout << std::endl;
      // }


      // const vector<string>& valores = macroMap[key_name];
      // for (const string& valor : valores) {
      //   if(valor.find("MACRO") == string::npos){
      //     arq_out.push_back(valor);
      //     cout << valor << endl;
      //   }
      // }
    }

    else{
      cout << "tem 3" << "\n";

    }

    // for (const string &str : argsRotulos)
    // {
    //   cout << str << "\n";
    // }
    //cout << macroMap[key_name].size() << endl;
  }

  /////////////////////// Gera arquivo .pre
  string make_out_file(){
    // Abre o arquivo para a escrita
    string outputFile = "file_pre_process.pre";
    ofstream output(outputFile);
    for (const string &str : arq_out)
    {
      output << str << "\n";
    }
    
    output.close();
    return outputFile;
  }

public:
  unordered_map<string, vector<string>> macroMap; // Hash_map contendo as macros
  vector<string> arq_out; // lista contendo o código .pre

  // Identifica e armazena as macros 
  string manipulaMacros(string inputFile)
  {
    // Abre o arquivo de entrada para leitura
    ifstream input(inputFile);
    string line;

    bool macro = false;
    bool endMacro = false;
    string nameMacro;

    // Lê o arquivo
    while (getline(input, line))
    {
      transform(line.begin(), line.end(), line.begin(), ::toupper);
      // Vê se tem a diretiva de inicialização e gera o index no hash_map com o rótulo da macro
      if (line.find(": MACRO") != string::npos)
      {
        // Retira os dois pontos do rótulo da macro e armazena-o no hash map
        string index = line.substr(0, line.find(':'));
        nameMacro = index;
        macroMap[nameMacro].push_back(line.substr(line.find("MACRO"), -1));
        macro = true;
        endMacro = false;
      } // Fecha o if

      // Armazena o body da macro no respectivo index

      else if ((line.find("ENDMACRO") == string::npos) && !endMacro)
      {
        macroMap[nameMacro].push_back(line);

      } // fecha o else if

      else
      {
        endMacro = true;
        macro = false;
        // cout << line << "\n";

        // Pula se ENDMACRO
        if (line.find("ENDMACRO") != string::npos)
        {
          continue;
        }

        // Se a linha está no hash de macros, substitui pelo corpo da macro
        for (const auto &pair : macroMap){
          const string& chave = pair.first;
          // cout << "Chave: " << chave << "\n";
          // cout << "Line: " << line << "\n";
          if(line.find(chave) != string::npos){ // Dúvida: por que não estava funiconando quando line == chave?
            this -> insert_body(line);
            continue;
          }
        } // Fecha o loop for

        arq_out.push_back(line);

      } // Fecha o else maior

    } // Fecha o While

    input.close();
    
    string pre_process_arq = this -> make_out_file();

    return pre_process_arq;

  } // Fecha função manipula_macros


};  // Fecha classe Macros

