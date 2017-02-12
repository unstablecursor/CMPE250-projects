/*
Student Name: Recep Deniz Aksoy
Student Number: 2014400150
Project Number: #4
Operating System: written in macosx
Compile Status: compiled
Program Status: finished
Notes:

*/
#include <fstream>
#include <iostream>
#include <vector>
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
int N, M;
vector<vector<Edge>> graph; //The graph
vector<int> cut;
vector<int> path;
vector<int> path_length;
vector<bool> visited;
vector<bool> is_cut;
void read_and_process(const string& s){
    fstream fin(s);
    fin >> N >> M;
    graph.resize(N);
    visited.resize(N);
    for(int i = 0; i < N - 1; i++){
        int st,e,w;
        fin >> st >> e >> w;
        Edge edge(e,w);
        Edge edge2(st,w);
        graph[st].push_back(edge);
        graph[e].push_back(edge2);
    }
    for(int i = 0; i < M; i++){
        int t;
        fin>>t;
        cut.push_back(t);
        is_cut.resize(N, false);
        is_cut[t] = true;
    }
    fin.close();
}
int Dfs(int node){
    visited[node] = true;
    for (int i = 0; i < graph[node].size() ; ++i) {
        if(visited[graph[node][i].vertex] == true){
            continue;
        }
        if (is_cut[graph[node][i].vertex] && visited[graph[node][i].vertex] == false){
            path.push_back(graph[node][i].vertex);
            path.push_back(node);
            path_length.push_back(graph[node][i].weight);
            return 1;
        }
        if(Dfs(graph[node][i].vertex) == 1) {
            path.push_back(node);
            path_length.push_back(graph[node][i].weight);
            return 1;
        }
    }
    return 0;
}

int main(int argc, char* argv[]){
    string s = argv[1];
    read_and_process(s);
    int result = 0;
    for(int i = 0; i < cut.size() - 1; i++){ //This is for DFS precision. 10 does the job but i guess 100 is safer.
        while(Dfs(cut[i]) == 1) {
            int start, end;
            int min_road = path_length[0];
            for (int j = 0; j < path_length.size(); ++j) {
                if (min_road >= path_length[j]) {
                    min_road = path_length[j];
                    start = path[j];
                    end = path[j + 1];
                }
            }
            result += min_road;
            for (int k = 0; k < graph[start].size(); ++k) {
                if (graph[start][k].vertex == end) {
                    graph[start].erase(graph[start].begin() + k);
                }
            }
            for (int k = 0; k < graph[end].size(); ++k) {
                if (graph[end][k].vertex == start) {
                    graph[end].erase(graph[end].begin() + k);
                }
            }
            visited.clear();
            visited.resize(N, false);
            path.clear();
            path_length.clear();
        }
    }
    ofstream output_file;
    output_file.open(argv[2]);
    output_file << result <<endl;
    output_file.close();
    return 0;
}