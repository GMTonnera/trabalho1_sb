#include<bits/stdc++.h>
#include<fstream>

#include "Parser.cpp"

using namespace std;


class Translator{
    private:
        int line_counter;
        int location_counter;

        // Parser
        Parser parser = Parser();

        // Symbol table       
        unordered_map<string, vector<int>> symbol_table;

        


        void first_pass(string file_name){
            
        }

        void second_pass(string file_name){

        }

        void pre_processing(string file_name){
            
        }
    
    public:
        Translator(){}

        void translate(string file_name){
            unordered_map<int, vector<string>> tokens;

            // Adquirir os tokens do arquivo por linha          
            tokens = this->parser.extract_text_from_file(file_name);
            
            // Ignorar comentário
            this->parser.erase_comments(tokens);
            
            for(int i = 1; i < tokens.size(); i++){
                string aux = "";
                for(string word : tokens[i]){
                    aux += word + " ";
                }
                cout << i << ": " << aux << "\n";
            }
        }
};