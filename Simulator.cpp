#include <bits/stdc++.h>

using namespace std;

class Simulator
{
private:
    int pc;
    int acc;
    int last_pos;
    int data_section;
    unordered_map<int, int> memory;

    void set_up_memory(string file_name)
    {
        this->pc = 0;

        bool data_sec = false;
        string num, str_obj_code;
        vector<int> obj_code;

        ifstream obj_file(file_name);
        getline(obj_file, str_obj_code);
        stringstream aux(str_obj_code);

        while (aux >> num)
        {
            int inst = stoi(num);
            this->memory[this->pc] = inst;
            this->pc++;
        }
        this->last_pos = this->pc;
    }

    void find_data_section_start()
    {
        vector<int> args;
        for (this->pc = 0; this->pc < this->memory.size(); this->pc++)
        {
            if (find(args.begin(), args.end(), this->pc) != args.end())
            {
                this->data_section = this->pc;
                break;
            }

            if (this->memory[this->pc] == 14)
            {
                continue;
            }
            else if (this->memory[this->pc] == 9)
            {
                args.push_back(this->memory[this->pc + 1]);
                args.push_back(this->memory[this->pc + 2]);
                this->pc += 2;
            }
            else
            {
                args.push_back(this->memory[this->pc + 1]);
                this->pc++;
            }
        }
    }

    void show_memory()
    {
        for (int i = 0; i < this->memory.size(); i++)
        {
            cout << "end " << i << "\t" << this->memory[i] << "\n";
        }
    }

    void exacute()
    {
        this->acc = 0;
        for (this->pc = 0; pc < this->memory.size(); this->pc++)
        {
            if (this->pc >= this->data_section)
            {
                cout << "SEGMENTATION FAULT\n";
                break;
            }
            bool input = false;
            string out = "";
            int inst = this->memory[this->pc];
            if (inst == 1)
            {
                acc += this->memory[this->memory[this->pc + 1]];
                cout << "PC <- " << this->pc << " ACC <- " << this->acc << out << "\n";
                pc++;
            }
            else if (inst == 2)
            {
                acc -= this->memory[this->memory[this->pc + 1]];
                cout << "PC <- " << this->pc << " ACC <- " << this->acc << out << "\n";
                pc++;
            }
            else if (inst == 3)
            {
                acc *= this->memory[this->memory[this->pc + 1]];
                cout << "PC <- " << this->pc << " ACC <- " << this->acc << out << "\n";
                pc++;
            }
            else if (inst == 4)
            {
                acc /= this->memory[this->memory[this->pc + 1]];
                cout << "PC <- " << this->pc << " ACC <- " << this->acc << out << "\n";
                pc++;
            }
            else if (inst == 5)
            {
                this->pc = this->memory[this->memory[this->pc + 1]];
                cout << "PC <- " << this->pc << " ACC <- " << this->acc << out << "\n";
                this->pc--;
            }
            else if (inst == 6)
            {
                if (this->acc < 0)
                {
                    this->pc = this->memory[this->memory[this->pc + 1]];
                    cout << "PC <- " << this->pc << " ACC <- " << this->acc << out << "\n";
                    this->pc--;
                }
                else
                {
                    cout << "PC <- " << this->pc << " ACC <- " << this->acc << out << "\n";
                    this->pc++;
                }
            }
            else if (inst == 7)
            {
                if (this->acc > 0)
                {
                    this->pc = this->memory[this->memory[this->pc + 1]];
                    cout << "PC <- " << this->pc << " ACC <- " << this->acc << out << "\n";
                    this->pc--;
                }
                else
                {
                    cout << "PC <- " << this->pc << " ACC <- " << this->acc << out << "\n";
                    this->pc++;
                }
            }
            else if (inst == 8)
            {
                if (this->acc == 0)
                {
                    this->pc = this->memory[this->memory[this->pc + 1]];
                    cout << "PC <- " << this->pc << " ACC <- " << this->acc << out << "\n";
                    this->pc--;
                }
                else
                {
                    cout << "PC <- " << this->pc << " ACC <- " << this->acc << out << "\n";
                    this->pc++;
                }
            }
            else if (inst == 9)
            {
                this->memory[this->memory[this->pc + 2]] = this->memory[this->memory[this->pc + 1]];
                cout << "PC <- " << this->pc << " ACC <- " << this->acc << out << "\n";
                pc += 2;
            }
            else if (inst == 10)
            {
                this->acc = this->memory[this->memory[this->pc + 1]];
                cout << "PC <- " << this->pc << " ACC <- " << this->acc << out << "\n";
                pc++;
            }
            else if (inst == 11)
            {
                this->memory[this->memory[this->pc + 1]] = this->acc;
                cout << "PC <- " << this->pc << " ACC <- " << this->acc << out << "\n";
                pc++;
            }
            else if (inst == 12)
            {
                int num;
                cout << "INPUT: ";
                cin >> num;
                this->memory[this->memory[this->pc + 1]] = num;
                cout << "PC <- " << this->pc << " ACC <- " << this->acc << out << "\n";
                pc++;
            }
            else if (inst == 13)
            {
                out = " SAIDA: " + to_string(this->memory[this->memory[this->pc + 1]]);
                cout << "PC <- " << this->pc << " ACC <- " << this->acc << out << "\n";
                pc++;
            }
            else if (inst == 14)
            {
                cout << "PC <- " << this->pc << " ACC <- " << this->acc << out << "\n";
                break;
            }

            while (cin.get() != '\n')
            {
                cout << "ok\n";
            }
        }
    }

public:
    void simulate(string file_name)
    {
        this->set_up_memory(file_name);
        this->find_data_section_start();
        cout << this->data_section << "\n";
        this->exacute();
    }
};