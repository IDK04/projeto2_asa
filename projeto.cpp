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
vector<int> steps;

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

int DFS_visit1(int vertexID,int time){
    stack<int> stack;
    stack.push(vertexID);
    int currentVertex;

    while(!stack.empty()){
        currentVertex = stack.top();

        if(verticesFirstDFS[currentVertex].color == BLACK){
            stack.pop();
            continue;
        }

        if(verticesFirstDFS[currentVertex].color == GREY){
            verticesFirstDFS[currentVertex].closed = ++time;
            verticesFirstDFS[currentVertex].color = BLACK;
            stack.pop();
            sortedVerticesID.push_back(currentVertex);
            continue;
        }

        if (verticesFirstDFS[currentVertex].color == WHITE){
            verticesFirstDFS[currentVertex].found = ++time;
            verticesFirstDFS[currentVertex].color = GREY;
        }

        for (int j : graphAdjacent[currentVertex]){
            if (verticesFirstDFS[j].color == WHITE){
                verticesFirstDFS[j].pi = currentVertex;
                stack.push(j);
            }
        }
    }

    return time;
}

int DFS_visit2(int vertexID,int time, int *res){

    stack<int> stack;
    stack.push(vertexID);
    int currentVertex;

    vector<int> currentSccAdjacent = vector<int>(num_vertices+1, 0);
    vector<int> currentScc;

    while(!stack.empty()){
        currentVertex = stack.top();

        if(verticesSecondDFS[currentVertex].color == BLACK){
            stack.pop();
            continue;
        }

        if(verticesSecondDFS[currentVertex].color == GREY){
            verticesSecondDFS[currentVertex].closed = ++time;
            verticesSecondDFS[currentVertex].color = BLACK;

            for (int j : graphAdjacentT[currentVertex]){
                if (currentSccAdjacent[j]==0){
                    *res = max(*res, steps[j]+1);
                    steps[currentVertex] = *res;
                }
            }
            stack.pop();
            continue;
        }

        if (verticesSecondDFS[currentVertex].color == WHITE){
            verticesSecondDFS[currentVertex].found = ++time;
            verticesSecondDFS[currentVertex].color = GREY;
            currentScc.push_back(currentVertex);
            currentSccAdjacent[currentVertex] = 1;
        }

        for (int j : graphAdjacentT[currentVertex]){
            if (verticesSecondDFS[j].color == WHITE){
                verticesSecondDFS[j].pi = currentVertex;
                stack.push(j);
            }
        }
    }
    for(int j : currentScc){
        steps[j] = *res;      
    }

    return time;
}

void DFS1(){
    int time = 0;
    for (int v=1;v<=num_vertices;v++){
        if (verticesFirstDFS[v].color == WHITE){
            time = DFS_visit1(v,time);
        }
    }
}

int DFS2(){
    int time = 0;
    int res=0;
    for (int v=num_vertices-1;v>=0;v--){
        if (verticesSecondDFS[sortedVerticesID[v]].color == WHITE){
            int curRes = 0;
            time = DFS_visit2(sortedVerticesID[v],time,&curRes);
            res = max(res, curRes);
        }
    }
    return res;
}

int main(){
    leinput();
    DFS1();
    steps = vector<int>(num_vertices+1, 0);
    int res = DFS2();   
    printf("%d\n", res);
    return 0;
}
