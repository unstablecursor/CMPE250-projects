/*
Student Name: Recep Deniz Aksoy
Student Number: 2014400150
Project Number: #3
Operating System: written in OSX, compiled and run in xubuntu
Compile Status: Compiled.
Program Status: Finished.
Notes:

*/

#include <iostream>
#include <queue>
#include <fstream>
using namespace std;

struct Node{
    double time;
    double max_time;
    int number;
    Node(double time_, int i){
        time = time_;
        max_time = time_;
        number = i;
    }
};

double readAndTopsort() {
    int N,M;
    cin >> N >> M;
    vector<vector<Node>> graph(N);
    int indegree[N];
    int from, to;
    //Placing the tasks
    for(int i = 0; i < N; i++){
        double time_;
        cin >> time_;
        vector<Node> node_vec;
        Node node(time_, i);
        node_vec.push_back(node);
        graph[i] = node_vec;
        indegree[i] = 0;
    }
    //Placing the tasks dependencies.
    for(int i=0; i<M; i++) {
        cin >> from >> to;
        Node node(graph[to].front().time, graph[to].front().number);
        graph[from].push_back(node);
        indegree[to]++;
    }

    queue<Node> zeros;
    //Preparing the task list for no dependencies.
    for(int i=0; i<=N; i++){
        if(indegree[i]==0){
            zeros.push(graph[i][0]);
        }
    }
    double max = 0;
    int cnt = 0;
    //Top sort algorithm with maximum time update
    while(!zeros.empty()){
        Node node = zeros.front();
        zeros.pop();
        cnt++;
        for(int i=1; i<graph[node.number].size(); i++){
            indegree[graph[node.number][i].number]--;
            //Maximum time update.
            if(node.max_time >= graph[graph[node.number][i].number][0].max_time - graph[graph[node.number][i].number][0].time ){
                graph[graph[node.number][i].number][0].max_time = graph[graph[node.number][i].number][0].time + node.max_time;
            }
            if(indegree[graph[node.number][i].number]==0){
                zeros.push(graph[graph[node.number][i].number][0]);
            }
        }
        if(max < node.max_time){
            max = node.max_time;
        }
    }

    if (cnt<N){
        return -1;
    }
    else {
        return max;
    }
}

int main(int argc, char* argv[]) {
    freopen(argv[1], "r", stdin);
    double x = readAndTopsort();
    //Printing out the result.
    ofstream output_file;
    output_file.open(argv[2]);
    output_file << x <<endl;
    output_file.close();
    return 0;
}