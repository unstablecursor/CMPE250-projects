/*
Student Name: Recep Deniz Aksoy
Student Number: 2014400150
Project Number: #2
Operating System: written in macOS, compiled and tested in xubuntu
Compile Status: Compiled
Program Status: Finished
Notes:

*/
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <queue>
#include <iomanip>

using namespace std;

class task{
public:
    double arrival_time;
    double cpu_work;
    double out_work;
    double queue_time = 0;
    double exit_time;
    int id;

    task(double arr, double cpu_work, double out_work, int id){
        arrival_time = arr;
        this->cpu_work = cpu_work;
        this->out_work = out_work;
        this->id = id;
    }

    task(const task *t){
        arrival_time = t->arrival_time;
        cpu_work = t->cpu_work;
        out_work = t->out_work;
        queue_time = t->queue_time;
        id = t->id;
        exit_time = t->exit_time;
    }

    task(const task &t) {
        arrival_time = t.arrival_time;
        cpu_work = t.cpu_work;
        out_work = t.out_work;
        queue_time = t.queue_time;
        id = t.id;
        exit_time = t.exit_time;
    }
};

class cpu{
public:
    double frequency;
    int id;
    double exit_time = -1;
    double max_active = 0;
    task *current_task = nullptr;
    cpu(double freq, int id) {
        frequency = freq;
        this->id = id;
    }
};

struct output{
public:
    double quantum;
    int id;
    double exit_time = -1;
    double max_active = 0;
    task *current_task = nullptr;
    output(double quantum, int id) {
        this->quantum = quantum;
        this->id = id;
    }
};

struct CompareCPU{
    bool operator()(task & event1, task & event2){
        return event1.cpu_work > event2.cpu_work;
    }
};
struct CompareArr{
    bool operator()(task & event1, task & event2){
        return event1.arrival_time > event2.arrival_time;
    }
};
struct TimeComp{
    bool operator()(double x, double y){
        return x > y;
    }
};
class TimePQ{
public:
    int length;
    priority_queue<double, vector<double>, TimeComp> min_heap;
    TimePQ() {
        length = 0;
    }
    void push(double e){
        min_heap.push(e);
        length++;
    }
    double pop(){
        double e = min_heap.top();
        min_heap.pop();
        length--;
        return e;
    }
    double top(){
        return min_heap.top();
    }
};
template <class T>
class PQ{
public:
    int max_length;
    int length;
    priority_queue<task, vector<task>, T> min_heap;
    PQ() {
        length = 0;
        max_length = 0;
    }
    void push(task e){
        min_heap.push(e);
        length++;
        maxl_update();
    }
    task pop(){
        task e = min_heap.top();
        min_heap.pop();
        length--;
        maxl_update();
        return e;
    }
    task top(){
        return min_heap.top();
    }
    void maxl_update(){
        if(length >= max_length){
            max_length = length;
        }
    }
};

void read(vector<cpu> &CPUs, vector<output> &OUTs, PQ<CompareArr> &pq, TimePQ &clock){
    //Reading CPUs
    int num_cpus;
    cin >> num_cpus;
    for (int i = 1; i <= num_cpus; ++i) {
        double freq;
        cin >> freq;
        cpu c(freq, i);
        CPUs.push_back(c);
    }
    //Reading output devices
    int num_outs;
    cin >> num_outs;
    for (int i = 1; i <= num_outs; ++i) {
        double quant;
        cin >> quant;
        output o(quant, i);
        OUTs.push_back(o);
    }
    //Reading tasks
    int num_tasks;
    cin >> num_tasks;
    for (int j = 0; j < num_tasks; ++j) {
        double arr_time, cpu_work, out_work;
        cin >> arr_time;
        clock.push(arr_time);
        cin >> cpu_work;
        cin >> out_work;
        task e(arr_time, cpu_work, out_work, j);
        pq.push(e);
    }
}

int main (int argc, char* argv[]) {
    if (argc != 3) {
        cout << "Run the code with the following command: ./project2 [input_file] [output_file]" << endl;
        return 1;
    }
    vector<cpu> CPUs;
    vector<output> OUTs;
    PQ <CompareArr> task_arr;
    vector<task> PQ2;
    PQ <CompareCPU> PQ1;
    TimePQ clock;
    vector<task> finished_tasks;
    int maximum_length_pq2 = 0;
    freopen(argv[1], "r", stdin);
    read(CPUs, OUTs, task_arr, clock);
    while(clock.length != 0){
        double current_time = clock.pop();
        //Iterating output devices.
        for (int i = 0; i < OUTs.size() ; ++i) {
            //Finished tasks are handled
            if(current_time == OUTs[i].exit_time){
                if(OUTs[i].current_task->out_work < OUTs[i].quantum){
                    OUTs[i].max_active += OUTs[i].current_task->out_work;
                }
                else {
                    OUTs[i].max_active += OUTs[i].quantum;
                }
                OUTs[i].current_task->out_work -= OUTs[i].quantum;
                //If task totally finished
                if(OUTs[i].current_task->out_work <= 0) {
                    OUTs[i].current_task->queue_time -= (OUTs[i].current_task->out_work + OUTs[i].quantum);
                    OUTs[i].current_task->exit_time = current_time;
                    OUTs[i].current_task->queue_time += (OUTs[i].current_task->exit_time - OUTs[i].current_task->arrival_time);
                    finished_tasks.push_back(new task(OUTs[i].current_task));
                    OUTs[i].current_task = nullptr;

                    //When output device gets empty, it immidiately takes next task
                    if(PQ2.size() != 0) {
                        OUTs[i].current_task = new task(PQ2.front());
                        PQ2.erase(PQ2.begin());
                        if (OUTs[i].quantum > OUTs[i].current_task->out_work) {
                            OUTs[i].exit_time = current_time + OUTs[i].current_task->out_work;
                        } else {
                            OUTs[i].exit_time = current_time + OUTs[i].quantum;
                        }
                        clock.push(OUTs[i].exit_time);
                    }


                }
                //If it's not finished but PQ2 is not empty
                else if(PQ2.size() != 0){
                    OUTs[i].current_task->queue_time -= OUTs[i].quantum;
                    task t = new task(OUTs[i].current_task);
                    OUTs[i].current_task = nullptr;

                   //When output device gets empty, it immidiately takes next task
                    OUTs[i].current_task = new task(PQ2.front());
                    PQ2.erase(PQ2.begin());
                    if(OUTs[i].quantum > OUTs[i].current_task->out_work){
                        OUTs[i].exit_time = current_time + OUTs[i].current_task->out_work;
                    }
                    else {
                        OUTs[i].exit_time = current_time + OUTs[i].quantum;
                    }
                    clock.push(OUTs[i].exit_time);



                    PQ2.push_back(t);
                    if(maximum_length_pq2 <= (int) PQ2.size()){
                        maximum_length_pq2 = (int) PQ2.size();
                    }

                }
                //If PQ2 is empty no need to push, just extend the time for current job.
                else if(PQ2.size() == 0){
                    OUTs[i].current_task->queue_time -= OUTs[i].quantum;
                    if(OUTs[i].quantum > OUTs[i].current_task->out_work){
                        OUTs[i].exit_time = current_time + OUTs[i].current_task->out_work;
                    }
                    else {
                        OUTs[i].exit_time = current_time + OUTs[i].quantum;
                    }
                    clock.push(OUTs[i].exit_time);
                }
            }
        }

        //Finished cpus are evaluated
        for(int i = 0; i < CPUs.size(); ++i){
            if(CPUs[i].exit_time == current_time){
                CPUs[i].max_active += (CPUs[i].current_task->cpu_work / CPUs[i].frequency);
                CPUs[i].current_task->queue_time -= (CPUs[i].current_task->cpu_work / CPUs[i].frequency);
                task t = new task(CPUs[i].current_task);

                PQ2.push_back(t);
                if(maximum_length_pq2 <= (int) PQ2.size()){
                    maximum_length_pq2 = (int) PQ2.size();
                }
                CPUs[i].current_task = nullptr;
            }
        }

        //Putting tasks into output devices from PQ2
        for (int i = 0; i < OUTs.size(); ++i) {
            if (OUTs[i].current_task == nullptr){
                if(PQ2.size() != 0){
                    OUTs[i].current_task = new task(PQ2.front());
                    PQ2.erase(PQ2.begin());
                    if(OUTs[i].quantum > OUTs[i].current_task->out_work){
                        OUTs[i].exit_time = current_time + OUTs[i].current_task->out_work;
                    }
                    else {
                        OUTs[i].exit_time = current_time + OUTs[i].quantum;
                    }
                    clock.push(OUTs[i].exit_time);
                }
            }
        }
        //Filling in PQ1
        while(current_time == task_arr.top().arrival_time && task_arr.length > 0){
            PQ1.push(task_arr.pop());
        }
        //Putting tasks into cpus from PQ1
        for (int i = 0; i < CPUs.size(); ++i){
            if (CPUs[i].current_task == nullptr){
                if (PQ1.length != 0) {
                    CPUs[i].current_task = new task(PQ1.pop());
                    CPUs[i].exit_time = current_time + (CPUs[i].current_task->cpu_work / CPUs[i].frequency);
                    clock.push(CPUs[i].exit_time);
                }
            }
        }
    }
    int maximum_length_pq1 = PQ1.max_length;
    //Finding greatest cpu active time
    double greatest_cpu_active = 0;
    for(int i = 0; i < CPUs.size(); ++i){
        if(greatest_cpu_active < CPUs[i].max_active){
            greatest_cpu_active = CPUs[i].max_active;
        }
    }
    //Finding which cpu has the greatest active time
    int greatest_cpu_id;
    for(int i = 0; i < CPUs.size(); ++i){
        if(greatest_cpu_active == CPUs[i].max_active){
            greatest_cpu_id = CPUs[i].id;
            break;
        }
    }
    //Finding greatest output active time
    double greatest_out_active = 0;
    for(int i = 0; i < OUTs.size(); ++i){
        if(greatest_out_active < OUTs[i].max_active){
            greatest_out_active = OUTs[i].max_active;
        }
    }
    //Finding which output has the greatest active time
    int greatest_out_id;
    for(int i = 0; i < CPUs.size(); ++i){
        if(greatest_out_active == OUTs[i].max_active){
            greatest_out_id = OUTs[i].id;
            break;
        }
    }
    double average_queue_wait = 0;
    double longest_queue_wait = 0;
    double average_time = 0;
    //Calculating average and longest queue time. Also calculating average system time.
    for (int i = 0; i < finished_tasks.size(); ++i) {
        average_queue_wait += finished_tasks[i].queue_time;
        if(longest_queue_wait < finished_tasks[i].queue_time){
            longest_queue_wait = finished_tasks[i].queue_time;
        }
        average_time += finished_tasks[i].exit_time - finished_tasks[i].arrival_time;
    }
    average_queue_wait = average_queue_wait / finished_tasks.size();
    average_time = average_time / finished_tasks.size();

    ofstream output_file;
    output_file.open(argv[2]);
    output_file << maximum_length_pq1<<endl;
    output_file <<maximum_length_pq2<<endl;
    output_file <<greatest_cpu_id<<endl;
    output_file<<greatest_out_id<<endl;
    output_file<<average_queue_wait<<endl;
    output_file<<longest_queue_wait<<endl;
    output_file <<average_time<<endl;
    output_file.close();


    return 0;
}
