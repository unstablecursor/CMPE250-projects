/*
Student Name: Recep Deniz Aksoy
Student Number: 2014400150
Project Number: #5
Operating System: written in osx
Compile Status: compiled
Program Status: finished
Notes: Heuristic values are irrevelant, i guess project is pointless w/o the real heuristic values and it can be solved
 with dijkstra. Without the heuristic values that makes sense, this project is not an A* project...

*/
#include <fstream>
#include <iostream>
#include <vector>
#include <queue>
#include "limits.h"
using namespace std;
//first is vertex, second is the weight of the edge.
struct Edge{
    int vertex;
    int weight;
    Edge(int a, int b){
        vertex = a;
        weight = b;
    }
};
int start;
int endd;
int N, E;
vector<vector<Edge>> graph; //The graph
vector<int> h_value;
vector<int> f_value;
vector<int> g_value;
vector<bool> visit;
vector<bool> visit_heap;
void read_and_process(const string& s){
    fstream fin(s);
    fin >> N >> E;
    graph.resize(N);
    for(int i = 0; i < N; i++){
        int fl;
        h_value.resize(N);
        fin >> fl;
        h_value[i] = fl;
    }
    for(int i = 0; i < E; i++){
        int st,e,w;
        fin >> st >> e >> w;
        Edge edge(e,w);
        Edge edge2(st,w);
        graph[st].push_back(edge);
        graph[e].push_back(edge2);
    }
    f_value.resize(N,INT_MAX);
    g_value.resize(N,INT_MAX);
    visit.resize(N,false);
    visit_heap.resize(N,false);
    fin>>start;
    fin>>endd;
    fin.close();
}
struct compare_func{
    bool operator()(int i, int j){
        return f_value[i] > f_value[j];
    }
};
int main(int argc, char* argv[]){
    string s = argv[1];
    read_and_process(s);
    int result = 0;
    priority_queue<int, vector<int>, compare_func> f_minheap;
    f_minheap.push(start);
    visit_heap[start] = true;
    int current = 0;
    g_value[start] = 0;
    f_value[start] = h_value[start];
    while(f_minheap.size() != 0){
        current = f_minheap.top();
        if(current == endd){
            result = g_value[current];
            break;
        }
        f_minheap.pop();
        visit[current] = true;
        for (int i = 0; i < graph[current].size() ; ++i) {
            int neighbour = graph[current][i].vertex;
            int temp = g_value[current] + graph[current][i].weight;
            if(visit[neighbour] == true && temp >= g_value[neighbour]){
                continue;
            }
            else if(visit[neighbour] == true && temp < g_value[neighbour]){
                g_value[neighbour] = temp;
                f_value[neighbour] = g_value[neighbour] + h_value[neighbour];
                f_minheap.push(neighbour);
                continue;
            }
            if(visit_heap[neighbour] == false){
                f_minheap.push(neighbour);
                visit_heap[neighbour] = true;
            }
            else if(temp >= g_value[neighbour]){
                continue;
            }
            g_value[neighbour] = temp;
            f_value[neighbour] = g_value[neighbour] + h_value[neighbour];
        }
    }
    ofstream output_file;
    output_file.open(argv[2]);
    output_file << result <<endl;
    output_file.close();
    return 0;
}