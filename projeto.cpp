#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <stack>

using namespace std;

#define WHITE 1
#define GREY 2
#define BLACK 3

typedef struct vertex{
    int color;
    int found;
    int closed;
    int pi;
} vertex;

vector<vertex> verticesFirstDFS;
vector<vertex> verticesSecondDFS;
vector<int> sortedVerticesID;
vector<vector<int>> SCCs;
vector<int> SCCsT;
vector<vector<int>> SCCAdjacent;

vector<vector<int>> graphAdjacent;
vector<vector<int>> graphAdjacentT;

int num_vertices;
int num_edges;

void leinput(){
    int v1,v2;
    scanf("%d%d",&num_vertices,&num_edges);
    verticesFirstDFS = vector<vertex>(num_vertices+1,{WHITE,0,0,0});
    verticesSecondDFS = vector<vertex>(num_vertices+1,{WHITE,0,0,0});
    graphAdjacent = vector<vector<int>>(num_vertices+1, vector<int>());
    graphAdjacentT = vector<vector<int>>(num_vertices+1, vector<int>());
    for (int i=0;i<num_edges;i++){
        scanf("%d%d",&v1,&v2);
        graphAdjacent[v1].push_back(v2);
        graphAdjacentT[v2].push_back(v1);
    }
}

int DFS_visit(int vertexID,int time,vector<vertex>&vertices,vector<vector<int>> &connections, int dfs1){

    stack<int> stack;
    stack.push(vertexID);
    int currentVertex;
    int addToScc=1;
    vector<int> currentScc;

    while(!stack.empty()){
        currentVertex = stack.top();

        if(vertices[currentVertex].color == BLACK){
            stack.pop();
            continue;
        }

        if(vertices[currentVertex].color == GREY){
            vertices[currentVertex].closed = ++time;
            vertices[currentVertex].color = BLACK;
            stack.pop();
            if(dfs1)
                sortedVerticesID.push_back(currentVertex);
            else{
                if (addToScc)
                    SCCs.push_back(currentScc);
                addToScc = 0;
            }
            continue;
        }

        if (vertices[currentVertex].color == WHITE){
            vertices[currentVertex].found = ++time;
            vertices[currentVertex].color = GREY;
            if(!dfs1){
                currentScc.push_back(currentVertex);
                SCCsT[currentVertex] = SCCs.size();
            }
        }

        for (int j : connections[currentVertex]){
            if (vertices[j].color == WHITE){
                vertices[j].pi = currentVertex;
                stack.push(j);
            }
        }

    }

    return time;
}

void DFS1(){
    int time = 0;
    for (int v=1;v<=num_vertices;v++){
        if (verticesFirstDFS[v].color == WHITE){
            time = DFS_visit(v,time,verticesFirstDFS,graphAdjacent,1);
        }
    }
}

void DFS2(){
    int time = 0;
    for (int v=num_vertices-1;v>=0;v--){
        if (verticesSecondDFS[sortedVerticesID[v]].color == WHITE){
            time = DFS_visit(sortedVerticesID[v],time,verticesSecondDFS,graphAdjacentT,0);
        }
    }
}

void buildSccConections(){
    vector<int> bitmap = vector<int>(SCCs.size(), 0);
    SCCAdjacent = vector<vector<int>>(SCCs.size(), vector<int>());
    for(vector<int> SCC : SCCs){
        for(int v1: SCC){
            for(int v2: graphAdjacent[v1]){
                if((SCCsT[v1] != SCCsT[v2]) && (bitmap[SCCsT[v2]]==0)){
                    SCCAdjacent[SCCsT[v1]].push_back(SCCsT[v2]);
                    bitmap[SCCsT[v2]]=1;
                }
            }
            for(int v2: graphAdjacent[v1]){
                bitmap[SCCsT[v2]]=0;
            }
        }
    }
}

int computeStep(vector<int> steps, int i){
    if(SCCAdjacent[i].size() == 0)
        return 0;

    if(steps[i] != -1)
        return steps[i];
    
    int result = 0;
    for(int adjacent : SCCAdjacent[i]){
        steps[adjacent] = computeStep(steps, adjacent);
        result = max(result, steps[adjacent]);
    }
    return result+1;
}

int findMaxStep(){

    vector<int> steps = vector<int>(SCCs.size(), -1);
    int maxStep = 0;

    for (size_t i = 0; i < steps.size(); i++){
        steps[i] = computeStep(steps, i);
        maxStep = max(steps[i], maxStep);
    }
    
    return maxStep;
}

int main(){
    leinput();
    DFS1();
    SCCsT = vector<int>(num_vertices+1, -1);
    DFS2();
    buildSccConections();
    int res = findMaxStep();
    printf("%d\n", res);
    return 0;
}
