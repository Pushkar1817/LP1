#include <bits/stdc++.h>
using namespace std;

class Mnemonics {
private:
    map<string, pair<string, int>> IS = {
        {"STOP", {"IS", 0}}, 
        {"ADD", {"IS", 1}}, 
        {"SUB", {"IS", 2}},
        {"MUL", {"IS", 3}}, 
        {"MOVER", {"IS", 4}}, 
        {"MOVEM", {"IS", 5}},
        {"COMP", {"IS", 6}}, 
        {"BC", {"IS", 7}}, 
        {"DIV", {"IS", 8}},
        {"READ", {"IS", 9}}, 
        {"PRINT", {"IS", 10}}
    };
    
    map<string, pair<string, int>> AD = {
        {"START", {"AD", 1}}, 
        {"END", {"AD", 2}}, 
        {"ORIGIN", {"AD", 3}},
        {"EQU", {"AD", 4}}, 
        {"LTORG", {"AD", 5}}
    };

    map<string, pair<string, int>> DL = {
        {"DC", {"DL", 1}}, 
        {"DS", {"DL", 2}}
    };

    map<string, int> REG = {
        {"AREG", 1},
        {"BREG", 2}, 
        {"CREG", 3}, 
        {"DREG", 4}
    };

    map<string, int> COND = {
        {"LT", 1}, 
        {"LE", 2}, 
        {"EQ", 3}, 
        {"GE", 4}, 
        {"GT", 5}, 
        {"ANY", 6}
    };

public:
    friend class asmPass1;
};

class asmPass1 {
private:
    map<string, int> symTab;
    vector<pair<string, int>> litTab;
    vector<string> lbl;
    vector<string> interCode;
    vector<vector<string>> inputCode;
    vector<int> poolTab;
    int poolInd = 0;
    Mnemonics m;
    int loc_counter = 0;
    vector<int> loc;

public:
    asmPass1(vector<vector<string>> intCode) {
        inputCode = intCode;
    }

    string resolveOprand(string op) {
        if (op == "-" || op =="")
            return "";
        else if (m.REG.find(op) != m.REG.end())
            return "(R," + to_string(m.REG[op]) + ")";
        else if (m.COND.find(op) != m.COND.end())
            return "(" + to_string(m.COND[op]) + ")";
        else if (op[0] == '=') {
            litTab.push_back({op, -1});
            return "(L," + to_string(litTab.size() - 1) + ")";
        } 
        else if (isdigit(op[0]))
            return "(C," + op + ")";
        else if(op.find('+') == string::npos && op.find('-')==string::npos){
        // else{
            // 
            int ind = -1;
            int j = 0;
            for (auto i:symTab) {
                j++;
                if( i.first== op)
                    ind = j;
                // cout<<lbl[i]<<"  ";
            }
            // cout<<endl;
            if (ind == -1){
                // lbl.push_back(op);
                // ind = lbl.size()-1;
                symTab[op] = 0;
                ind = symTab.size()-1;
            }
            return "(S," + to_string(ind+1) + ")";
        }
        else{
            int add = op.find('+');
            if(add!=string::npos){
                return resolveOprand(op.substr(0,add))+"+"+op.substr(add+1);
            }
            int sub = op.find('-');
            if(sub!=string::npos){
                return resolveOprand(op.substr(0,sub))+"-"+op.substr(sub+1);
            }
        }
    }

    vector<string> processLiterals(int loc_counter) {
        vector<string>dec;
        for (auto &i : litTab) {
            if (i.second == -1) {
                i.second = loc_counter;
                dec.push_back("(DL,1)(C,"+i.first.substr(1)+")");
                loc_counter++;
            }
        }
        poolInd++;
        poolTab.push_back(poolInd);
        return dec;
    }

    int expEval(string op) {
        char operation = ' ';
        for(int i = 0 ;i<op.size() ;i++){
            if(op[i] =='+' || op[i] =='-')
                operation = op[i];
        }
        if(operation==' ' && isdigit(op[0]))
            return stoi(op);
        if(operation == '+'){
            int pos = op.find('+');
            string sym = op.substr(0,pos);
            int num = stoi(op.substr(pos+1));
            int addr = symTab[sym];
            return addr+num;
        }
        else if(operation == '-'){
            int pos = op.find('-');
            string sym = op.substr(0,pos);
            int num = stoi(op.substr(pos+1));
            int addr = symTab[sym];
            return addr-num;
        }
        return 0;
    }

    void pass1() {
        for (auto &line : inputCode) {
            string label = "-";
            string instr = "-";
            string op1 = "-";
            string op2 = "-";
            string interLine = "";
            for (int i = 0; i < line.size(); i++) {
                if (i == 0 && m.IS.find(line[i]) == m.IS.end() &&
                    m.AD.find(line[i]) == m.AD.end() &&
                    m.DL.find(line[i]) == m.DL.end()) {
                    label = line[i];
                } 
                else if (m.IS.find(line[i]) != m.IS.end() ||
                           m.AD.find(line[i]) != m.AD.end() ||
                           m.DL.find(line[i]) != m.DL.end()) {
                    instr = line[i];
                } 
                else {
                    if (op1 == "-") {
                        op1 = line[i];
                    } 
                    else {
                        op2 = line[i];
                    }
                }
            }
            // cout<<"'"<<label<<"'"<<loc_counter<<endl;
            if (label != "-") {
                symTab[label] = loc_counter;
                // cout<<label<<"  "<<symTab[label]<<endl;int i = 0
                int i = 0;
                for( ;i< lbl.size();i++){
                    if(label == lbl[i])
                        break;
                }
                if(i==lbl.size()) lbl.push_back(label);
            }

            if (m.IS.find(instr) != m.IS.end()) {
                interLine += "(IS," + to_string(m.IS[instr].second) +")" + resolveOprand(op1) +resolveOprand(op2);
                interCode.push_back(interLine);
                loc_counter++;   
            } 
            else if (m.AD.find(instr) != m.AD.end()) {
                if(instr == "EQU"){
                    symTab[label] = expEval(op1);
                    interCode.push_back("no ic for equ only reflect in sym tab");
                    continue;
                }
                interLine += "(AD," + to_string(m.AD[instr].second) +")" + resolveOprand(op1) +resolveOprand(op2);
                interCode.push_back(interLine);
                if (instr == "START") {
                    loc_counter = stoi(op1) - 1;
                } 
                else if (instr == "LTORG" || instr == "END"){
                    vector<string> consta = processLiterals(loc_counter);
                    for(int i = 0 ;i< consta.size() ;i++){
                        interCode.push_back(consta[i]);
                    }
                }
                else if (instr == "ORIGIN")
                    loc_counter = expEval(op1) - 1;
                loc_counter++;   
            } 
            else if (m.DL.find(instr) != m.DL.end()) {
                interLine += "(DL," + to_string(m.DL[instr].second) + ")"+ resolveOprand(op1) +resolveOprand(op2);
                interCode.push_back(interLine);
                if (m.DL[instr].second == 2) {
                    // cout<<op1<<endl;
                    loc_counter += stoi(op1) ;
                }
                else{
                    loc_counter++;   
                }
            }
            
        }
    }

    void showTables() {
        cout << "---INTERMEDIATE CODE---" << endl;
        for (const auto &line : interCode) {
            cout << line << endl;
        }
        cout << "----SYMBOL TABLE----" << endl;
        int j = 1;
        cout << "ind.\tname\taddr" << endl;
        for (const auto &i : symTab) {
            cout << j++ << "\t" << i.first << "\t" << i.second << endl;
        }
        cout << "----LITERAL TABLE----" << endl;
        cout << "ind.\tname\taddr" << endl;
        for (int i = 0; i < litTab.size(); i++) {
            cout << (i + 1) << "\t" << litTab[i].first << "\t" << litTab[i].second << endl;
        }
    }
};

vector<vector<string>> split(string filename){
    ifstream inc(filename);
    vector<vector<string>>tokens;
    string line;
    while(getline(inc,line)){
        string word;
        stringstream ss(line);
        vector<string>words;
        while(getline(ss,word,' ')){
            if(word.size()>0){words.push_back(word);}
        }
        tokens.push_back(words);
    }
    return tokens;
} 

int main() {
    vector<vector<string>> input = split("input2.txt") ;
    // vector<vector<string>> input = {
    //     {"-", "START", "200", "-"},
    //     {"-", "ADD", "AREG", "=3"},
    //     {"-", "MOVEM", "AREG", "A"},
    //     {"-", "LTORG", "-", "-"},
    //     {"-", "ADD", "AREG", "B"},
    //     {"-", "MOVER", "AREG", "C"},
    //     {"-", "SUB", "BREG", "=5"},
    //     {"-", "LTORG", "-", "-"},
    //     {"-", "SUB", "AREG", "A"},
    //     {"-", "PRINT", "A", "-"},
    //     {"-", "STOP", "-", "-"},
    //     {"A", "DS", "9", "-"},
    //     {"B", "DC", "20", "-"},
    //     {"C", "DS", "2", "-"},
    //     {"-", "END", "-", "-"}
    // };

    asmPass1 p1(input);
    p1.pass1();
    p1.showTables();
    return 0;
}
