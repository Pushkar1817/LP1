#include<bits/stdc++.h>
using namespace std;

class memoryManagement{
    private:
        vector<vector<pair<string,int>>>memory;
        int n;
        int prevPointer;
    public:
        memoryManagement(){
            cout<<"Enter the number of blocks : ";
            cin>>n;
            for(int i=0 ;i<n ;i++){
                cout<<"Enter the size of block "<<i+1<<" : ";
                int siz;
                cin>>siz;
                // vector<pair<string,int>>temp;
                // temp.push_back({"FREE",siz});
                memory.push_back({{"FREE",siz}});
            }
        }

        void displayMemory(){

            for(int i = 0 ;i< n;i++){
                cout<<i+1<<endl;
                for(int j = 0 ; j< memory[i].size() ;j++){
                    cout<<" "<<j+1<<" "<<memory[i][j].first<<" "<<memory[i][j].second<<endl;
                }
            }
        }

        void firstFit(){
            string name ;
            int siz;
            cout<<"Enter the name of process : ";
            cin>>name;
            cout<<"Enter the size of process : ";
            cin>>siz;

            bool allocated = false;
            for(int i = 0 ;i< n ;i++){
                for(int j = 0 ;j<memory[i].size();j++){
                    if(memory[i][j].second >= siz && memory[i][j].first == "FREE"){
                        memory[i][j].first = name;
                        int blockSiz = memory[i][j].second;
                        memory[i][j].second = siz;
                        if(blockSiz - siz > 0){
                            memory[i].push_back({"FREE",(blockSiz - siz)});
                        }
                        allocated = true;
                        break;
                    }
                }
                if(allocated){
                    break;
                }
            }

            if(!allocated){
                cout<<"Memory not allocated to process "<<name<<" ."<<endl;
            }
            else{
                cout<<"Memory Allocated to process "<<name<<" . "<<endl;
            }
        }

        void nextFit(){
            string name ;
            int siz;
            cout<<"Enter the name of process : ";
            cin>>name;
            cout<<"Enter the size of process : ";
            cin>>siz;
            cout<<"Enter the previous loction of pointer : ";
            cin>>prevPointer;
            prevPointer--;
            int pointer = prevPointer+1;
            bool allocated = false;
            while(pointer != prevPointer){
                for(int j = 0 ;j<memory[pointer].size();j++){
                    if(memory[pointer][j].second >= siz && memory[pointer][j].first == "FREE"){
                        memory[pointer][j].first = name;
                        int blockSiz = memory[pointer][j].second;
                        memory[pointer][j].second = siz;
                        if(blockSiz - siz > 0){
                            memory[pointer].push_back({"FREE",(blockSiz - siz)});
                        }
                        allocated = true;
                        break;
                    }
                }
                if(allocated){
                    break;
                }
                pointer++;
                pointer = pointer%n;
            }
            if(!allocated){
                cout<<"Memory not allocated to process "<<name<<" ."<<endl;
            }
            else{
                cout<<"Memory Allocated to process "<<name<<" . "<<endl;
            }

        }

        void bestFit(){
            string name ;
            int siz;
            cout<<"Enter the name of process : ";
            cin>>name;
            cout<<"Enter the size of process : ";
            cin>>siz;

            int bestInd = -1;
            int curFit = INT_MAX;

            for(int i = 0 ;i< n ;i++){
                for(int j = 0 ;j < memory[i].size() ;j++){
                    if(memory[i][j].first == "FREE" && memory[i][j].second >= siz && memory[i][j].second < curFit){
                        bestInd = i;
                        curFit = memory[i][j].second;
                    }
                }
            }
            if(bestInd==-1){
                cout<<"Memory not available for "<<name<<endl;
                return;
            }

            memory[bestInd].back().first = name;
            memory[bestInd].back().second = siz;
            if(curFit > siz){
                memory[bestInd].push_back({"FREE",curFit-siz});
            }
            cout<<"Memory allocated to process "<<name<<" ."<<endl;
        }
        void worstFit(){
            string name ;
            int siz;
            cout<<"Enter the name of process : ";
            cin>>name;
            cout<<"Enter the size of process : ";
            cin>>siz;

            int worstInd = -1;
            int curFit = INT_MIN;

            for(int i = 0 ;i< n ;i++){
                for(int j = 0 ;j < memory[i].size() ;j++){
                    if(memory[i][j].first == "FREE" && memory[i][j].second >= siz && memory[i][j].second > curFit){
                        worstInd = i;
                        curFit = memory[i][j].second;
                    }
                }
            }
            if(worstInd==-1){
                cout<<"Memory not available for "<<name<<endl;
                return;
            }

            memory[worstInd].back().first = name;
            memory[worstInd].back().second = siz;
            if(curFit > siz){
                memory[worstInd].push_back({"FREE",curFit-siz});
            }
            cout<<"Memory allocated to process "<<name<<" ."<<endl;
        }
};

int main ()
{
    memoryManagement obj;

    while (true) {
        cout << "\nSelect allocation method:" << endl;
        cout << "1. First Fit" << endl;
        cout << "2. Best Fit" << endl;
        cout << "3. Worst Fit" << endl;
        cout << "4. Next Fit" << endl;
        cout << "5. View Memory" << endl;
        cout << "6. Exit" << endl;

        int choice;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                cout << "First Fit Allocation:" << endl;
                obj.firstFit();
                break;
            case 2:
                cout << "Best Fit Allocation:" << endl;
                obj.bestFit();
                break;
            case 3:
                cout << "Worst Fit Allocation:" << endl;
                obj.worstFit();
                break;
            case 4:
                cout << "Next Fit Allocation:" << endl;
                obj.nextFit();
                break;
            case 5:
                cout << "Current Memory Status:" << endl;
                obj.displayMemory();
                break;
            case 6:
                cout << "Exiting..." << endl;
                return 0;
            default:
                cout << "Invalid choice. Please try again." << endl;
        }
    }

    return 0;
}