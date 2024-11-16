#include<bits/stdc++.h>
using namespace std;

vector<string> readLines(const string& filename) {
    ifstream file(filename);
    vector<string> lines;
    string line;

    while (getline(file, line)) {
        lines.push_back(line);
    }
    return lines;
}

vector<string> split(const string& str, char delimiter) {
    vector<string> tokens;
    string token;
    stringstream tokenStream(str);

    while (getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

class newMNT{
    public:
        string name;
        int kp;
        int pp;
        int kpdtp;
        int mdtp;

        newMNT(){
            name = "";
            kp = 0;
            pp = 0;
            kpdtp = 0;
            mdtp = 0;
        }
};

class macroPass2{
        vector<string>pntab = readLines("pnt.txt");
        vector<string>kptab = readLines("kpdt.txt");
        vector<string>mntab = readLines("mnt.txt");
        vector<string>mdtab = readLines("mdt.txt");
    public:
        vector<string>macCode;
        newMNT mnt;
        vector<string> macroCall;
        map<string,string>aptab;
        vector<string>para;
        macroPass2(string call){
            macroCall = split(call,' ');
        }

        void setMNT(){
            
            for(int i = 0 ;i<mntab.size() ;i++){
                vector<string> line = split(mntab[i],' ');
                if(macroCall[0] == line[0]){
                    mnt.name = line[0];
                    mnt.pp = stoi(line[1]);
                    mnt.kp = stoi(line[2]);
                    mnt.kpdtp = stoi(line[3]);
                    mnt.mdtp = stoi(line[4]);
                }
            }
        }

        void getPara(){
            for(int i =  0 ;i< pntab.size() ;i++){
                if(split(pntab[i],' ')[0]==mnt.name){
                    i++;
                    para = split(pntab[i],' ');
                }
            }
        } 

        void fillAPTAB(){
            for(int i = 0 ;i<para.size();i++){
                aptab[para[i]] ="";
            }
            int j = 0;
            for(int i = mnt.kpdtp+1 ;i<kptab.size() && j <mnt.kp ;i++){
                aptab[split(kptab[i],' ')[0]] = split(kptab[i],' ')[1];
                j++;
            }
            vector<string>param = split(macroCall[1],',');
            
            for(int i = 0 ;i< param.size() ;i++){
                if(param[i].find("=")!=string::npos){
                    aptab[split(param[i],'=')[0]] = split(param[i],'=')[1];
                }
                else{
                    for(auto&j:aptab){
                        if(j.second==""){
                            j.second = param[i];
                            break;
                        }
                    }
                }
                // cout<<param[i]<<aptab[param[i]]<<endl;
            }
        }       

        string resolveOP(string op){
            if(op[0]=='=')
                return op;
            else{
                return aptab[para[op[3]-'0'-1]];
            }
        }
        void writeMachineCode(){
            // macCode.push_back("START");
            string line ="";
            for(int i = mnt.mdtp-1 ;;i++){
                line = mdtab[i];
                string macLine = "+";
                if(split(line,' ')[0] == "MEND"){
                    // macCode.push_back("END");
                    return;
                }
                string instr = split(line,' ')[0];
                string op1 = split(line,' ')[1];
                string op2 = split(line,' ')[2];
                macLine+=" "+instr+" "+resolveOP(op1)+" "+resolveOP(op2);
                macCode.push_back(macLine);
            }
            
        }

        // void displayTables(){
        //     cout<<"-------APTAB---------"<<endl;
        //     for(auto i : aptab){
        //         cout<<"  "<<i.first<<":"<<i.second<<endl;
        //         apt<<"  "<<i.first<<":"<<i.second<<endl;
        //     }
        //     apt.close();
        //     ofstream mac("machine.txt",ios::app);
        //     cout<<"-----EXPANDED LINES OF CODE----"<<endl;
        //     mac<<"-----EXPANDED LINES OF CODE----"<<endl;
        //     mac<<"START"<<endl;
        //     for(int i = 0 ;i<macCode.size() ;i++){
        //         cout<<macCode[i]<<endl;
        //         mac<<macCode[i]<<endl;
        //     }
        //     mac<<"END"<<endl;
        // }
};

int main ()
{
    vector<string>macroCalls = readLines("macro_call.txt");
    ofstream apt("aptab.txt");
    ofstream mac("machine.txt");
    mac<<"----MACHINE CODE----"<<endl;
    mac<<"START"<<endl;
    vector<macroPass2>all;
    for(auto&macroCall : macroCalls){
        macroPass2 mp2(macroCall);
        mp2.setMNT();
        mp2.getPara();
        mp2.fillAPTAB();
        mp2.writeMachineCode();
        // mp2.displayTables();
        apt<<mp2.mnt.name<<":"<<endl;
        cout<<"---APTAB("<<mp2.mnt.name<<")---"<<endl;
        for(auto i : mp2.aptab){
            cout<<"  "<<i.first<<":"<<i.second<<endl;
            apt<<"  "<<i.first<<":"<<i.second<<endl;
        }
        cout<<"----EXPANDED LINES OF CODE("<<mp2.mnt.name<<")----"<<endl;
        for(auto line : mp2.macCode){
            cout<<line<<endl;
            mac<<line<<endl;
        }
        all.push_back(mp2);
    }
    mac<<"END"<<endl;
    mac.close();
    apt.close();
    return 0;
}