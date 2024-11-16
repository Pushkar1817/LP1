#include<bits/stdc++.h>
using namespace std;

class process{
    private:
        string name;
        int arv_time;
        int burst_time;
        int tat;
        int wt;
        int comp_time;
        int priority;
    
    public:
        process(){
            name = "";
            arv_time = 0 ;
            burst_time =0;
            comp_time =0;
            tat = 0;
            wt = 0;
            priority = 0;
        }

        void create(){
            cout<<"Enter the name of process : ";
            cin>>name;
            cout<<"Enter the arrival time of "<<name<<" : ";
            cin>>arv_time;
            cout<<"Enter the burst time of process "<<name<<": ";
            cin>>burst_time;
        }

        friend class Scheduler;
};

class Scheduler{
    private:
        vector<process>Processes;
        int n;
    public:
        Scheduler(){
            cout<<"Enter the number of processes :";
            cin>>n;
            Processes.resize(n);
        }

        void insertDetails(){
            for(int i = 0 ;i< n ;i++){
                Processes[i].create();
            }
        }

        void display(vector<process>temp,int n,float avg_tat,float wt_tat){
            cout<<endl<<"name\tat\tbt\tct\tTAT\tWT"<<endl;
            for(int i = 0 ;i< n ;i++){
                cout<<temp[i].name<<"\t"
                    <<temp[i].arv_time<<"\t"
                    <<temp[i].burst_time<<"\t"
                    <<temp[i].comp_time<<"\t"
                    <<temp[i].tat<<"\t"
                    <<temp[i].wt<<"\n";
            }

            cout<<"Average Turn Around Time  : "<<avg_tat<<endl;
            cout<<"Average Waiting Time : "<<wt_tat;
        }

        void FCFS(){
            vector<process>temp(Processes);

            sort(temp.begin(),temp.end(),[](const process&a,const process&b){
                return a.arv_time < b.arv_time;
            });

            int ct = 0;
            for(int i = 0 ;i< n ;i++){
                ct+=temp[i].burst_time;

                temp[i].comp_time = ct;
                temp[i].tat = temp[i].comp_time - temp[i].arv_time;
                temp[i].wt = temp[i].tat - temp[i].burst_time;
            }

            float avg_tat = 0;
            float avg_wt = 0;

            for(int i = 0 ;i< n ;i++){
                avg_tat += temp[i].tat;
                avg_wt += temp[i].wt;
            }
            avg_tat/=n;
            avg_wt/=n;

            display(temp,n,avg_tat,avg_wt);
        }

        void SJF_preemptive(){
            vector<process>temp(Processes);
            int totalTime = 0;
            for(int i = 0 ;i< temp.size();i++){
                totalTime+=temp[i].burst_time;
            }
            // cout<<totalTime<<endl;
            sort(temp.begin(),temp.end(),[](const process& a, const process&b){
                return a.arv_time < b.arv_time;
            });

            vector<string>gantt;
            auto cmp = [](const process&a,const process&b){
                return a.burst_time > b.burst_time;
            };
            priority_queue<process,vector<process>,decltype(cmp)> readyQ(cmp);
            int timeStamp =0;
            int cp = 0;

            while(timeStamp<totalTime){
                while(cp<n && temp[cp].arv_time <= timeStamp){
                    readyQ.push(temp[cp]);
                    cp++;
                }
                if(!readyQ.empty()){
                    process minBurst = readyQ.top();
                    if(!gantt.size() ||gantt.back() != minBurst.name){
                        gantt.push_back(minBurst.name);
                    }
                    readyQ.pop();
                    minBurst.burst_time--;
                    if(minBurst.burst_time == 0){
                        for(int i = 0 ;i< n ;i++){
                            if(temp[i].name == minBurst.name){
                                temp[i].comp_time = timeStamp+1;
                                temp[i].tat = temp[i].comp_time - temp[i].arv_time;
                                temp[i].wt = temp[i].tat - temp[i].burst_time;
                                break;
                            }
                        }
                    }
                    else{
                        readyQ.push(minBurst);
                    }
                    timeStamp++;
                }
            }

            float avg_tat = 0;
            float avg_wt = 0;
            for(int i = 0 ;i<  n; i++){
                avg_tat+=temp[i].tat;
                avg_wt+=temp[i].wt;
            }
            avg_tat/=n;
            avg_wt/=n;
            display(temp,n,avg_tat,avg_wt);
            cout<<"\nGantt chart : \t";
            for(int i= 0 ;i< gantt.size() ;i++){
                cout<<gantt[i]<<" ";
            }
            cout<<endl;
        }

        void roundRobin(){
            int time_quantam;
            cout<<"Enter the time Quantam : ";
            cin>>time_quantam;
            vector<process>temp(Processes);
            int totalTime = 0;
            for(int i = 0 ;i< temp.size() ;i++){
                totalTime+=temp[i].burst_time;
            }

            sort(temp.begin(),temp.end(),[](const process & a , const process & b){
                return a.arv_time < b.arv_time;
            });

            vector<string>gantt;
            queue<process>readyQ;
            int timeStamp = 0;
            int cp = 0;

            while(timeStamp < totalTime){
                while(cp<n && timeStamp >= temp[cp].arv_time){
                    readyQ.push(temp[cp]);
                    cp++; 
                }
                if(!readyQ.empty()){
                    process curr = readyQ.front();
                    if(!gantt.size() ||gantt.back() != curr.name){
                        gantt.push_back(curr.name);
                    }
                    readyQ.pop();
                    if(curr.burst_time <= time_quantam){
                        for(int i = 0 ;i< n ;i++){
                            if(temp[i].name == curr.name){
                                temp[i].comp_time = timeStamp + curr.burst_time;
                                temp[i].tat = temp[i].comp_time - temp[i].arv_time;
                                temp[i].wt = temp[i].tat - temp[i].burst_time;
                                break;
                            }
                        }
                        timeStamp+=time_quantam;
                    }
                    else{
                        curr.burst_time = curr.burst_time - time_quantam;
                        timeStamp += time_quantam;
                        while(cp<n && timeStamp >= temp[cp].arv_time){
                            readyQ.push(temp[cp]);
                            cp++; 
                        }
                        readyQ.push(curr);
                    }
                }
            }
            float avg_tat = 0;
            float avg_wt = 0;
            for(int i = 0 ;i<  n; i++){
                avg_tat+=temp[i].tat;
                avg_wt+=temp[i].wt;
            }
            avg_tat/=n;
            avg_wt/=n;
            display(temp,n,avg_tat,avg_wt);
            cout<<"\nGantt chart : \t";
            for(int i= 0 ;i< gantt.size() ;i++){
                cout<<gantt[i]<<" ";
            }
            cout<<endl;
        }

        void priority_non_preemptive(){
            vector<process>temp(Processes);
            int totalTime = 0;
            for(int i = 0 ;i< n ;i++){
                cout<<"Enter the priority for "<<temp[i].name<<" : ";
                cin>>temp[i].priority;
                totalTime+=temp[i].burst_time;
            }

            sort(temp.begin(),temp.end(),[](const process&a , const process& b){
                return a.arv_time < b.arv_time;
            });

            auto cmp = [](const process&a,const process&b){
                return a.priority < b.priority;
            };
            priority_queue<process,vector<process>,decltype(cmp)>readyQ(cmp);
            int timeStamp = 0 ;
            vector<string>gantt;
            int cp =0;

            while(timeStamp <totalTime ){
                int prevBurst = 1;
                while(cp <n && timeStamp >= temp[cp].arv_time){
                    readyQ.push(temp[cp]);
                    cp++;
                }
                if(!readyQ.empty()){
                    process curr = readyQ.top();
                    gantt.push_back(curr.name);
                    readyQ.pop();
                    for(int i = 0 ;i< n ; i++){
                        if(curr.name == temp[i].name){
                            temp[i].comp_time = timeStamp + temp[i].burst_time;
                            temp[i].tat = temp[i].comp_time - temp[i].arv_time;
                            temp[i].wt = temp[i].tat - temp[i].burst_time;
                            break;
                        }
                    }
                    prevBurst =curr.burst_time; 
                }
                timeStamp+=prevBurst;
            }
            float avg_tat = 0;
            float avg_wt = 0;
            for(int i = 0 ;i<  n; i++){
                avg_tat+=temp[i].tat;
                avg_wt+=temp[i].wt;
            }
            avg_tat/=n;
            avg_wt/=n;
            display(temp,n,avg_tat,avg_wt);
            cout<<"\nGantt chart : \t";
            for(int i= 0 ;i< gantt.size() ;i++){
                cout<<gantt[i]<<" ";
            }
            cout<<endl;
        }

};

int main ()
{
    Scheduler scheduler;
    //3 p2 1 5 p3 2 8 p1 0 8
    cout << "......... Scheduling Algorithm ........." << endl;
    cout << "1. FCFS" << endl;
    scheduler.FCFS();
    cout << "2. SJF_Preemptive" << endl;
    scheduler.SJF_preemptive();
    cout << "3. Round_Robin_Preemptive" << endl;
    scheduler.roundRobin();
    cout << "4. Priority_Non_Preemptive" << endl;
    scheduler.priority_non_preemptive();
    return 0;
}