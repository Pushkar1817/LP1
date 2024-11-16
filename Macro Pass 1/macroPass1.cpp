#include<bits/stdc++.h>
using namespace std;

class KPDTAB{
    public:
        string key;
        string value;

        KPDTAB(){
            key = "";
            value = "";
        }
};

class PNTAB{
    public:
        int index;
        string param;

        PNTAB(){
            index = 0;
            param = "";
        }
};

class MNT{
    public:
        string name;
        int kp ;
        int pp ;
        int kpdtp;
        int mdtp;

        MNT(){
            name="";
            kp = 0;
            pp = 0;
            kpdtp = 0;
            mdtp  = 0 ;
        }
};

vector<vector<string>> tokenizeFile(){
    ifstream inf("input.txt");
    if(inf.fail()){
        cout<<"Failed to open the file...";
        return {};
    }

    vector<vector<string>>tokens;
    string line ;
    while(getline(inf,line)){
        stringstream ss(line);
        vector<string>temp;
        string word;
        while(ss>>word){
            temp.push_back(word);
        }
        tokens.push_back(temp);
    }
    return tokens;
}

class macroPass1{
    private:
        vector<KPDTAB>kpdtab;
        vector<MNT>mnt;
        vector<PNTAB>pntab;
        vector<string>mdt;
        vector<vector<string>>tokens;
    public:
        macroPass1(){
            tokens = tokenizeFile();
        }

        void fillTables(){
            for(int i = 0 ;i< tokens.size() ;i++){
                if(tokens[i][0] == "MACRO"){
                    i++;
                    
                    int pp = 0 ;
                    int kp = 0 ;
                    MNT mntObj;
                    mntObj.kpdtp = kpdtab.size()+1;
                    mntObj.name = tokens[i][0];
                    mntObj.mdtp = mdt.size()+1;

                    for(int j = 1 ;j< tokens[i].size() ;j++){
                        PNTAB pObj;
                        if(tokens[i][j].find("=") != string::npos){
                            kp++;
                            int pos = tokens[i][j].find("=");
                            KPDTAB kpObj;
                            if(pos == tokens[i][j].size()-1){
                                kpObj.key = tokens[i][j].substr(0,pos);
                            }
                            else{
                                kpObj.key = tokens[i][j].substr(0,pos);
                                kpObj.value = tokens[i][j].substr(pos+1);
                            }
                            kpdtab.push_back(kpObj);
                            pObj.param = tokens[i][j].substr(0,pos);
                        }
                        else{
                            pp++;
                            pObj.param = tokens[i][j];
                        }
                        pObj.index = j;
                        pntab.push_back(pObj);
                    }
                    //for MDT
                    for(int j =i+1 ;;j++){
                        string instr = tokens[j][0];
                        if(instr == "MEND"){
                            mdt.push_back("MEND");
                            break;
                        }
                        // if(instr == "MACRO"){
                        //     // mdt.push_back("MACRO");
                        //     j++;
                        //     string macName = tokens[j][0];
                        //     vector<string>para;
                        //     string pName ="";
                        //     for(int k = 1; k<tokens[j].size();k++){
                        //         pName="";
                        //         string value ="" ;
                        //         if(tokens[j][k].find("=") != string::npos){
                        //             int pos = tokens[j][k].find("=");
                        //             pName = tokens[j][k].substr(0,pos);
                        //             value = tokens[j][k].substr(pos+1);
                        //         }
                        //         else{
                        //             pName = tokens[j][k];
                        //         }
                        //         int correctMacro = mnt.size();
                        //         for(int l = 0 ;l< pntab.size() ;l++){
                        //             if(correctMacro >0 && pntab[l].index == -1){
                        //                 correctMacro--;
                        //                 continue;
                        //             }
                        //             if(!correctMacro){
                        //                 if(pName == pntab[l].param){
                        //                     pName = "(p,"+to_string(pntab[l].index)+")"+value;
                        //                 }
                        //             }
                        //         }
                        //         para.push_back(pName);
                        //     } 
                        //     string def ="MACRO ";
                        //     def+=macName;
                        //     for(int l = 0 ;l<para.size() ;l++){
                        //         def+=" "+para[l];
                        //     }
                        //     j++;
                        //     mdt.push_back(def);
                        //     while(tokens[j][0]!="MEND"){
                        //         j++;
                        //     }
                        //     continue;
                        // }
                        string op1 = tokens[j][1];
                        string op2 = tokens[j][2]; 
                        // int ind1 = -1;
                        // int ind2 = -1;
                        int correctMacro = mnt.size();
                        for(int k = 0 ;k< pntab.size() ;k++){
                            if(correctMacro >0 && pntab[k].index == -1){
                                correctMacro--;
                                continue;
                            }
                            if(!correctMacro){
                                if(op1 == pntab[k].param){
                                    op1 = "(p,"+to_string(pntab[k].index)+")";
                                }
                                if(op2 == pntab[k].param){
                                    op2 = "(p,"+to_string(pntab[k].index)+")";
                                }
                            }
                        }
                        
                        mdt.push_back(instr+" "+op1+" "+op2);
                    }
                    mntObj.kp = kp;
                    mntObj.pp = pp;
                    if(kp==0){
                        mntObj.kpdtp = 0;
                    }
                    mnt.push_back(mntObj);
                    PNTAB newObj;
                    newObj.index = -1;
                    pntab.push_back(newObj);
                }
            }
        }

        void displayTables(){
            cout<<"----------------MNT------------------"<<endl;
            cout<<"NAME\tKP\tPP\tKPDTP\tMDTP"<<endl;
            for(int i = 0 ;i<mnt.size();i++){
                cout<<mnt[i].name<<"\t"
                    <<mnt[i].kp<<"\t"
                    <<mnt[i].pp<<"\t"
                    <<mnt[i].kpdtp<<"\t"
                    <<mnt[i].mdtp<<endl;
            }

            cout<<"------------------KPTAB-----------------------"<<endl;
            cout<<"Ind.\tNAME\t\tVALUE"<<endl;
            for(int i = 0 ;i<kpdtab.size() ;i++){
                cout<<i+1<<"\t"
                    <<kpdtab[i].key.substr(1)<<"\t"
                    <<((kpdtab[i].key.size()<=4)?"\t":"")
                    <<((kpdtab[i].value=="")?"-":kpdtab[i].value)<<endl;
            }
            cout<<"------------------PNTAB-----------------------"<<endl;
            cout<<"IND.\tPARAM\n"<<endl;
            
            for(int i = 0 ;i<pntab.size() ;i++){
                if(pntab[i].index == -1){
                    cout<<endl;
                    continue;
                }
                cout<<pntab[i].index<<"\t"
                    <<pntab[i].param.substr(1)<<endl;
            }

            cout<<"-----------------MDT---------------------"<<endl;
            for(int i = 0 ;i< mdt.size();i++){
                cout<<mdt[i]<<endl;
            }
        }
};

int main ()
{
    macroPass1 m;
    m.fillTables();
    m.displayTables();   
    return 0;
}