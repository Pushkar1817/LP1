#include<bits/stdc++.h>
using namespace std;

class Candidate{
    public:
        string cname;
        int id ;
        Candidate(){
            cname = "";
            id = 0;
        }
        friend class election;
};

class election{
        vector<Candidate>candidates;
        vector<bool>flag;
        int num;
        int highestPosi(vector<Candidate>candidates){
            int maxi = 0;
            int ind = -1;

            for(int i =0 ;i< candidates.size();i++){
                if(maxi < candidates[i].id){
                    maxi = candidates[i].id;
                    ind = i;
                }
            }

            return ind;
        }

        int findPosition(string name){
            for(int i = 0; candidates.size();i++){
                if(candidates[i].cname == name){
                    return i;
                }
            }
            return -1;
        }
        int getCurCord(){
            int ind = 0;
            for(int i = 0; candidates.size();i++){
                if(candidates[i].id > candidates[ind].id && flag[candidates[i].id-1]){
                    ind = i;
                }
            }
            return ind;
        }
    public:
        election(){
            cout<<"Enter the number of Candidates : ";
            cin>>num;
            candidates.resize(num);
            flag.resize(num);
        }

        void insertDetails(){
            for(int i =0;i<candidates.size();i++){
                cout<<"Enter the name of Candidate "<<(i+1)<<" : ";
                cin>>candidates[i].cname;
                cout<<"Enter the id of Candiate "<<(i+1)<<" : ";
                cin>>candidates[i].id;
                // flag[i] = true;
            }
        }

        Candidate bullyAlgorithm(Candidate initializer){
            int coordintor = highestPosi(candidates);
        
            cout<<" Current cooridinator "<<candidates[coordintor].cname<<" is dead was detect "<<initializer.cname<<" and started a election using bully algorithm."<<endl;
            for(int i = 0;i< num ;i++){
                flag[i] = false;
            } 
            flag[coordintor] = true;
            for(int i = 0;i< num ;i++){
                if(i>=initializer.id && !flag[i]){
                    cout<<"Initializer sended message to "<<candidates[i].cname<<" ."<<endl;
                }
                else{
                    flag[i] = true;
                }
            }
            for(int i = 0;i< num ;i++){
                if(!flag[i] && i>=initializer.id){
                    cout<<candidates[i].cname<<" took over from "<<initializer.cname<<endl;
                    flag[initializer.id-1] = true;
                    initializer = candidates[i];
                    flag[initializer.id-1] = true;
                }
            }
            for(int i = 0 ;i< num ;i++){
                if(!flag[i])
                    initializer = bullyAlgorithm(initializer);
            }
            return initializer;
        }

        void ringAlgorithm(Candidate initializer){
            int highest = highestPosi(candidates);
            flag[highest]=1;
            cout<<" Current cooridinator "<<candidates[highest].cname<<" is dead was detect "<<initializer.cname<<" and started a election using Ring algorithm."<<endl;
            vector<int>notDead;
            int count = 0;
            int i = initializer.id-1;
            while(count<num){
                if(!flag[i]){
                    notDead.push_back(candidates[i].id);
                }
                i = (i+1)%num;
                count++;
            }
            cout<<"Message was sended as :\n\t[";
            for(int i = 0 ;i<notDead.size();i++){
                cout<<notDead[i]<<" ";
            }
            cout<<"]"<<endl;

            int co = *max_element(notDead.begin(),notDead.end());
            cout<<"The new co-ordinator is "<<candidates[co-1].cname<<".";
            return;
        }
};


int main ()
{
    election e;
    e.insertDetails();
    Candidate int_co;
    Candidate fin_co;
    cout<<"Enter the name of initilizer : ";
    cin>>int_co.cname;
    cout<<"Enter the id of initilizer : ";
    cin>>int_co.id;
    bool cond = true;
    while(cond){
        cout<<"MENU:\n\t1. BULLY ALGORITHM\n\t2. RING ALGORITHM\n\t3. EXIT\n";
        int ch ;
        cout<<"Enter the choice : ";
        cin>>ch;
        switch(ch){
            case 1:
                fin_co = e.bullyAlgorithm(int_co);
                cout<<"The new coordinator is "<<fin_co.cname<<"."<<endl;
                break;
            case 2:
                e.ringAlgorithm(int_co);
                break;
            default:
                cond = false; 
                break; 
        }
    }
    return 0;
}