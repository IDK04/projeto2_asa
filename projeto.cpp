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

vector<vector<int>> matrix;
vector<vector<int>> transposedMatrix;

int num_vertices;
int num_edges;

void leinput(){
    int v1,v2;
    scanf("%d%d",&num_vertices,&num_edges);
    vertices = vector<vertex>(num_vertices+1,{WHITE,0,0,0});
    matrix = vector<vector<int>>(num_vertices+1, vector<int>(num_vertices+1, 0));
    transposedMatrix = vector<vector<int>>(num_vertices+1, vector<int>(num_vertices+1, 0));
    for (int i=0;i<num_edges;i++){
        scanf("%d%d",&v1,&v2);
        matrix[v1][v2] = 1;
        transposedMatrix[v2][v1] = 1;
    }
}

int DFS_visit(int vertexID,int time,vector<vector<int>> connections, int dfs1){
    time +=1;
    vertices[vertexID].found = time;
    vertices[vertexID].color = GREY;
    for (int j=1;j<=num_vertices;j++){
        if (connections[vertexID][j]==1 && vertices[j].color == WHITE){
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
            time = DFS_visit(v,time,matrix,1);
        }
    }
}

void DFS2(){
    int time = 0;
    for (int v=1;v<=num_vertices;v++){
        vertices[v].color = WHITE;
    }
    for (int v=num_vertices-1;v>=0;v--){
        if (vertices[sortedVerticesID[v]].color == WHITE){
            time = DFS_visit(sortedVerticesID[v],time,transposedMatrix,0);
        }
    }
}

int main(){
    leinput();
    DFS1();
    DFS2();
    for(int i = 1; i <= num_vertices; i++){
        printf("vertice:%d; found:%d; closed:%d; pi:%d\n", i, vertices[i].found, vertices[i].closed, vertices[i].pi);
    }

}