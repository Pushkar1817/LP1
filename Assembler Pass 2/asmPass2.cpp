#include <bits/stdc++.h>
#include <string>
using namespace std;

class Pass2 {

public:
    map<int, int> SymTab;
    vector<pair<string, int>> LitTab;
    vector<vector<string>> intCode;
    vector<int> PoolTab;
    vector<string> machine_code;
    vector<int>loc_counter;

    Pass2(map<int, int> symtab, vector<pair<string, int>> littab, vector<int> pooltab, vector<vector<string>> intcode)
    {
        SymTab = symtab;
        LitTab = littab;
        PoolTab = pooltab;
        this->intCode = intcode;
    }

    void pass2(){
        for(auto& line : intCode){
            string macLine = "";
            bool skip = false;
            for(int j = 0 ; j < line.size(); j++){
                int pos = line[j].find(',');
                string instr = line[j].substr(0, pos);
                if(instr == "(AD" && line[j].substr(pos+1) == "1"){
                    int pos = line[j+1].find(',');
                    int loc = stoi(line[j+1].substr(pos+1));
                    loc_counter.push_back(loc-1);
                    machine_code.push_back("No machine code");
                    skip = true;
                    break;
                }
                loc_counter.push_back(loc_counter.back()+1);
                if((instr == "(AD" && line[j].substr(pos + 1) != "5") || (instr == "(DL" && line[j].substr(pos + 1) != "1")){
                    machine_code.push_back("No machine code");
                    skip = true;
                    break;
                }
                if(instr == "(AD" && line[j].substr(pos + 1) == "5"){
                    macLine += "0 0 ";
                }
                else if(instr == "(IS"){
                    macLine += line[j].substr(pos + 1) + " ";
                    if(line.size()==1){
                        macLine+="0 0";
                    }
                    else if(line.size()==2){
                        macLine += "0 ";
                    }
                }
                else if(instr == "(R"){
                    macLine += line[j].substr(pos + 1) + " ";
                }
                else if(instr == "(L"){
                    int id = stoi(line[j].substr(pos + 1))-1;
                    macLine += to_string(LitTab[id].second) + " ";
                }
                else if(instr == "(S"){
                    macLine += to_string(SymTab[stoi(line[j].substr(pos + 1))]) + " ";
                }
                else if(instr=="(C"){
                    macLine +=line[j].substr(pos + 1) + " ";
                }
                else if(instr == "(DL" && line[j].substr(pos + 1) == "1"){
                    macLine += "0 0 ";
                }
            }
            if (!skip) {
                machine_code.push_back(macLine);
            }
        }
    }

    void show_machin_code()
    {
        ofstream mac("mac_code.txt");
        int loc =0;
        cout<<"------MACHIONE CODE------"<<endl;
        mac<<"------MACHIONE CODE------"<<endl;
        for (const auto& line : machine_code)
        {
            cout <<loc_counter[loc]<<"  "<< line << endl;
            mac<<loc_counter[loc]<<"  "<< line << endl;
            loc++;
        }
        mac.close();
    }
};

vector<string> split(const string &line) {
    vector<string> words;
    string word;

    for (char ch : line) {
        if (ch != ')') {
            word += ch;
        } 
        else if (!word.empty()) {
            words.push_back(word);
            word.clear();
        }
    }

    if (!word.empty()) {
        words.push_back(word);
    }

    return words;
}
int main()
{
    map<int, int> symtab = {
        // {1, 209},
        // {2, 210},
        // {3, 211}
        {1,114},
        {2,116},
        {3,117},
        {4,104},
        {5,118},
        {6,115}
    };

    vector<pair<string, int>> littab = {
        // {"=4", 212}
    };

    vector<int> pooltab = {0};

    // vector<string> intcode = {
    //     "(AD,1)(C,200)",
    //     "(IS,9)(S,1)",
    //     "(IS,9)(S,2)",
    //     "(IS,4)(R,1)(L,1)",
    //     "(IS,1)(R,1)(S,2)",
    //     "(IS,4)(R,2)(S,1)",
    //     "(IS,2)(R,2)(S,1)",
    //     "(IS,5)(R,2)(S,3)",
    //     "(IS,10)(S,3)",
    //     "(DL,2)(C,1)",
    //     "(DL,2)(C,1)",
    //     "(AD,2)"
    // };
    vector<string>intcode={
        "(AD,1)(C,101)",
        "(IS,9)(S,1)",
        "(IS,4)(R,2)(S,2)",
        "(IS,4)(R,2)(S,3)",
        "(IS,3)(R,2)(S,3)",
        "(IS,4)(R,3)(S,3)",
        "(IS,1)(R,3)(S,2)",
        "(IS,5)(R,3)(S,3)",
        "(IS,6)(R,3)(S,1)",
        "(IS,7)(R,2)(S,4)",
        "(IS,8)(R,2)(S,5)",
        "(IS,5)(R,2)(S,6)",
        "(IS,10)(S,6)",
        "(IS,0)",
        "(DL,2)(C,1)",
        "(DL,2)(C,1)",
        "(DL,1)(C,1)",
        "(DL,2)(C,1)",
        "(DL,1)(C,2)",
        "(AD,2)"
    };

    vector<vector<string>> interCode;

    for (const auto& line : intcode)
    {
        interCode.push_back(split(line));
    }

    // for(int i = 0 ;i< interCode.size() ;i++){
    //     for(int j = 0 ; j< interCode[i].size() ;j++){
    //         cout<<interCode[i][j]<<endl;
    //     }
    //     cout<<endl;
    // }
    Pass2 assembler_pass2(symtab, littab, pooltab, interCode);

    // Call pass2 method to process intermediate code
    assembler_pass2.pass2();

    // Display the machine code
    assembler_pass2.show_machin_code();

    return 0;
}
