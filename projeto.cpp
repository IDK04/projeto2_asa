#include <stdio.h>
#include <stdlib.h>
#include <vector>

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

vector<vertex> vertices;
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
    vertices = vector<vertex>(num_vertices+1,{WHITE,0,0,0});
    graphAdjacent = vector<vector<int>>(num_vertices+1, vector<int>());
    graphAdjacentT = vector<vector<int>>(num_vertices+1, vector<int>());
    for (int i=0;i<num_edges;i++){
        scanf("%d%d",&v1,&v2);
        graphAdjacent[v1].push_back(v2);
        graphAdjacentT[v2].push_back(v1);
    }
}

int DFS_visit(int vertexID,int time,vector<vector<int>> &connections, int dfs1){
    time +=1;
    vertices[vertexID].found = time;
    vertices[vertexID].color = GREY;
    for (int j : connections[vertexID]){
        if (vertices[j].color == WHITE){
            vertices[j].pi = vertexID;
            time = DFS_visit(j,time,connections, dfs1);
        }
    }
    time +=1;
    if(dfs1)
        sortedVerticesID.push_back(vertexID);
    vertices[vertexID].color = BLACK;
    vertices[vertexID].closed = time;
    return time;
}

void DFS1(){
    int time = 0;
    for (int v=1;v<=num_vertices;v++){
        if (vertices[v].color == WHITE){
            time = DFS_visit(v,time,graphAdjacent,1);
        }
    }
}

void DFS2(){
    int time = 0;
    for (int v=1;v<=num_vertices;v++){
        vertices[v].color = WHITE;
        vertices[v].pi = 0;
    }
    for (int v=num_vertices-1;v>=0;v--){
        if (vertices[sortedVerticesID[v]].color == WHITE){
            time = DFS_visit(sortedVerticesID[v],time,graphAdjacentT,0);
        }
    }
}

void buildScc(){
    vector<int> checkedVertices = vector<int>(num_vertices+1, 0);
    SCCsT = vector<int>(num_vertices+1, -1);
    int SCCcounter = 0;
    for(int i = 1; i <= num_vertices;i++){
        if(checkedVertices[i] == 0 && (vertices[i].closed-vertices[i].found)==1){
            int currentVertex = i;
            vector<int> currentScc;
            while(currentVertex != 0){
                currentScc.push_back(currentVertex);
                checkedVertices[currentVertex] = 1;
                SCCsT[currentVertex] = SCCcounter;
                currentVertex = vertices[currentVertex].pi;
            }
            SCCs.push_back(currentScc);
            SCCcounter++;
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
    DFS2();
    buildScc();
    buildSccConections();
    int res = findMaxStep();
    printf("%d\n", res);
    return 0;
}