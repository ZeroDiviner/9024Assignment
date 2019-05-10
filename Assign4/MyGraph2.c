//
//  main.c
//  Assignment4
//
//  Created by Jasper on 21/4/19.
//  Copyright © 2019 Jasper. All rights reserved.
//

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include <limits.h>

#define V_num 50

// A vertex is a 2D point
typedef struct Vertex {
    int x; // x-coordinate
    int y; // y-coordinate
} Vertex;
// each edge is a pair of vertices (end-points)
typedef struct Edge {
    Vertex *p1; // first end point
    Vertex *p2; // second end point
} Edge;

//typedef struct AdjancyNode {
//    Vertex v;
//    int weight;
//    int valid;
//    struct  AdjancyNode *next;
//} AdjancyNode;

typedef struct VertexNode {
    Vertex *v;
    int valid;
    struct VertexNode *next;
    struct VertexNode *toMain;
    struct VertexNode *Qnext;
    struct VertexNode *nearest;
    float weight;
    float alreadydis;
    int showed;
    int reached;
} VertexNode;


typedef struct GraphRep *Graph;
typedef struct GraphRep { // graph header
    VertexNode **edges; // an array of vertices or a linked list of vertices
    int nV; // #vertices
    int nE; // #edges
} GraphRep;

typedef struct Queue{
    struct VertexNode *head;
    struct VertexNode *tail;
}Queue;

typedef struct Stack{
    struct StackNode *top;
}Stack;

typedef struct StackNode{
    Vertex *v;
    struct StackNode *next;
}StackNode;

Stack *CreateStack(){
    Stack *s = malloc(sizeof(Stack));
    assert(s!=NULL);
    s->top = NULL;
    return s;
};
StackNode * CreateStackNode(Vertex *v){
    StackNode *node = malloc(sizeof(StackNode));
    node->v = v;
    node->next = NULL;
    return node;
}
// A vertex node stores a vertex and other information, and you need to expand this type

//The above types serve as a starting point only. You need to expand them and add more types.
// Watch the lecture video between 7:50pm-8:20 or so for my discussion about this assignment

// Add the time complexity analysis of CreateEmptyGraph() here
// this function initiallize a empty graph, and for every node in graph, change their pointer(to adjancy list) to NULL;
Queue *CreateQueue(){
    Queue *q = malloc(sizeof(Queue));
    assert(q != NULL);
    q->head = NULL;
    q->tail = NULL;
    return q;
}

void Enqueue(Queue *q,VertexNode *v){
    if (q->head==NULL){
        q->head = v;
        q->tail = v;
    }
    else{
        q->tail->Qnext = v;
        q->tail = v;
    }
}
VertexNode *Dequeue(Queue *q){
    if(q->head == NULL){
        return NULL;
    }
    VertexNode *temp = q->head;
    if (q->head->Qnext == NULL){
        q->tail = NULL;
        q->head = NULL;
    }
    else{
        q->head = q->head->Qnext;
    }
    return temp;
}
int Inqueue(Queue *q , VertexNode *node){
    VertexNode *current = q->head;
    while(current){
        if(current == node){
            return 0;
        }
        current = current->Qnext;
    }
    return 1;
}
Graph CreateEmptyGraph()
{
    GraphRep *newGraph;
    newGraph = malloc(sizeof(GraphRep));
    assert(newGraph != NULL);
    newGraph ->nV = V_num;
    newGraph ->nE = 0;
    newGraph ->edges = malloc(V_num*sizeof(VertexNode));
    assert(newGraph -> edges != NULL);
    for (int i = 0; i < V_num; i++){
        newGraph -> edges[i] = NULL;
    }
    return newGraph;
}

VertexNode *createNewNode(int x, int y){
    VertexNode *newEdge = malloc(sizeof(VertexNode));
    assert(newEdge != NULL);
    Vertex *v1 = (Vertex*) malloc(sizeof(Vertex));
    assert(v1 != NULL);
    v1->x = x;
    v1->y = y;
    newEdge->v = v1;
    newEdge->next = NULL;
    newEdge->valid = 1;
    newEdge->reached = 0;
    newEdge->showed = 0;
    newEdge->toMain = NULL;
    newEdge->Qnext =NULL;
    newEdge->alreadydis = INT_MAX;
    newEdge->nearest = NULL;
    return newEdge;
}
// Add the time complexity analysis of InsertEdge() here
// this function traverse nodelist of graph to find if the node is exist(O(n)), and if the node exist, add a new element in adjancy list of this node(O(m)), else if node is not exist, then add node into the nodelist of graph and change its adjancy list, overall, the time complexity of this function is O(m+n);
int InsertEdge(Graph g, Edge *e)
{
    int exist_flag1,exist_flag2;
    exist_flag1 = 0;
    exist_flag2 = 0;
    assert(g!=NULL);
    VertexNode *first_main, *first_sub, *second_main, *second_sub;
    double distance = sqrt((double)((e->p1->x - e->p2->x)*(e->p1->x - e->p2->x)+
                                    (e->p1->y - e->p2->y)*(e->p1->y - e->p2->y)));
    for (int i = 0; i < V_num; i++){
        if (g->edges[i] == NULL){
            if (exist_flag1 == 0) {
                VertexNode *node = createNewNode(e->p1->x,e->p1->y);
                g->edges[i] = node;
                g->nV+=1;
                VertexNode *node2 = createNewNode(e->p2->x,e->p2->y);
                node ->next = node2;
                node2->weight = distance;
                exist_flag1 = 1;
                if (exist_flag2 == 0){
                    first_main = node;
                    first_sub = node2;
                }
                else{
                    second_main =node;
                    second_sub = node2;
                    second_sub->toMain = first_main;
                    first_sub ->toMain = second_main;
                }
                continue;
            }
            else if (exist_flag2 == 0){
                VertexNode *node2 = createNewNode(e->p2->x,e->p2->y);
                g->edges[i] = node2;
                g->nV+=1;
                VertexNode *node = createNewNode(e->p1->x,e->p1->y);
                node2->next = node;
                node->weight = distance;
                exist_flag2 = 1;
                if (exist_flag1 == 0){
                    first_main = node2;
                    first_sub = node;
                }
                else{
                    second_main = node2;
                    second_sub = node;
                    second_sub ->toMain = first_main;
                    first_sub ->toMain = second_main;
                }
                break;
            }
            else{
                break;
            }
        }
        //        printf("%d\n",g->edges[i]->valid);
        //        printf("%d\n",g->edges[i]->v->x);
        //        printf("%d\n",g->edges[i]->v->y);
        if (g->edges[i]->v->x == e->p1->x && g->edges[i]->v->y == e->p1->y){
            exist_flag1 = 1;
            if (g->edges[i]->next == NULL){
                VertexNode *node = createNewNode(e->p2->x,e->p2->y);
                g->edges[i]->next = node;
                g->edges[i]->next ->weight = distance;
                if (exist_flag2 ==0){
                    first_main = g->edges[i];
                    first_sub = node;
                }else{
                    second_main = g->edges[i];
                    second_sub = node;
                    first_sub ->toMain = second_main;
                    second_sub ->toMain = first_main;
                }
            }
            else{
                VertexNode *current_pointer = g->edges[i];
                while (current_pointer->next){
                    if (current_pointer->v->x == e->p2->x && current_pointer->v->y == e->p2->y){
                        return 0;
                    }
                    current_pointer = current_pointer->next;
                }
                if (current_pointer->v->x == e->p2->x && current_pointer->v->y == e->p2->y){
                    return 0;
                }
                VertexNode *node = createNewNode(e->p2->x,e->p2->y);
                current_pointer ->next = node;
                current_pointer ->next->weight = distance;
                if (exist_flag2 ==0){
                    first_main = g->edges[i];
                    first_sub = node;
                }else{
                    second_main = g->edges[i];
                    second_sub = node;
                    first_sub ->toMain = second_main;
                    second_sub ->toMain = first_main;
                }
                
            }
        }
        if (g->edges[i]->v->x == e->p2->x && g->edges[i]->v->y == e->p2->y){
            exist_flag2 = 1;
            if (g->edges[i]->next == NULL){
                VertexNode *node = createNewNode(e->p1->x,e->p1->y);
                g->edges[i]->next = node;
                g->edges[i]->next->weight = distance;
                if (exist_flag1 == 0){
                    first_main = g->edges[i];
                    first_sub = node;
                }else{
                    second_main = g->edges[i];
                    second_sub = node;
                    second_sub ->toMain = first_main;
                    first_sub ->toMain = second_main;
                }
            }
            else{
                VertexNode *current_pointer = g->edges[i];
                while (current_pointer->next){
                    if (current_pointer->v->x == e->p1->x && current_pointer->v->y == e->p1->y){
                        return 0;
                    }
                    current_pointer = current_pointer->next;
                }
                if (current_pointer->v->x == e->p1->x && current_pointer->v->y == e->p1->y){
                    return 0;
                }
                VertexNode *node = createNewNode(e->p1->x,e->p1->y);
                current_pointer ->next = node;
                current_pointer ->next->weight = distance;
                if (exist_flag1 == 0) {
                    first_main = g->edges[i];
                    first_sub = node;
                }else{
                    second_main = g->edges[i];
                    second_sub = node;
                    first_sub ->toMain = second_main;
                    second_sub ->toMain = first_main;
                }
            }
        }
    }
    g->nE +=1;
    return 1;
    
}

// this function first find node of this edge in (O(n)) time and remove a node from adjancy list(O(m)), so the time complexity for this function is O(m+n)
void DeleteEdge(Graph g, Edge *e)
{   Vertex *p1 = e->p1;
    Vertex *p2 = e->p2;
    for (int i=0;i<V_num;i++){
        if (g->edges[i] == NULL){
            continue;
        }
        else{
            if (g->edges[i]->v->x == p1->x && g->edges[i]->v->y == p1->y){
                VertexNode *current = g->edges[i]->next;
                VertexNode *prev = g->edges[i];
                while (current){
                    if(current->v->x ==p2->x && current->v->y == p2->y){
                        prev->next = current->next;
                        free(current);
                        break;
                    }
                    prev = current;
                    current = current->next;
                }
            }
            if (g->edges[i]->v->x == p2->x && g->edges[i]->v->y == p2->y){
                VertexNode *current = g->edges[i]->next;
                VertexNode *prev = g->edges[i];
                while (current){
                    if(current->v->x ==p1->x && current->v->y == p1->y){
                        prev->next = current->next;
                        free(current);
                        break;
                    }
                    prev = current;
                    current = current->next;
                }
            }
        }
    }
}
// this function get a node and recursively find all the relevant node in linklist,thus the time complexity for this function is(O(mn))
void subReachable(Graph g, Vertex *v, int flag){
    for (int i=0; i<V_num; i++){
        if (g->edges[i] == NULL){
            continue;
        }
        if (g->edges[i]->v->x == v->x && g->edges[i]->v->y == v->y && g->edges[i]->reached == 0){
            VertexNode *current = g->edges[i]->next;
            if(current){
                if (flag == 1){
                    printf("(%d,%d)",g->edges[i]->v->x,g->edges[i]->v->y);
                    g->edges[i]->reached = 1;
                }
                else{
                    printf(",(%d,%d)",g->edges[i]->v->x,g->edges[i]->v->y);
                    g->edges[i]->reached = 1;
                }
                
            }
            else{
                break;
            }
            while(current){
                subReachable(g, current->v,flag);
                current = current ->next;
            }
        }
    }
}
// this function get a node and recursively find all the relevant node in linklist,thus the time complexity for this function is(O(mn))
void ReachableVertices(Graph g, Vertex *v)
{
    subReachable(g,v,1);
    printf("\n");
}


// Add the time complexity analysis of ShortestPath() here
// this function use queue to implement a breadth first search to find a shortest path between 2 nodes, this function find all the edge and for the end node of edge, apply bread first search to find another edge, thus the time complexity is (O(m)), and finally we can get all the node in the connected graph stores its nearest node in the shortest path, and finally use a stack to print them out, this function need to traverse the node list to reset some variables O(n), thus the overall time complexity would be O(m+n)
void ShortestPath(Graph g, Vertex *u, Vertex *v)
{
    float dis;
    int k = 0;
    Queue *q = CreateQueue();
    for (int i=0;i< V_num;i++){
        if (g->edges[i]!=NULL &&u->x == g->edges[i]->v->x && u->y == g->edges[i]->v->y){
            g->edges[i]->alreadydis = 0;
            Enqueue(q, g->edges[i]);
            while (q->head) {
                VertexNode *head = Dequeue(q);
                head->showed = 1;
                VertexNode *current = head->next;
                k+=1;
                if (k>V_num*2){
                    break;
                }
                while(current){
                    dis = head->alreadydis + current->weight;
                    if (dis < current->toMain->alreadydis && current->toMain->showed!=1 &&
                        current->toMain !=head){
                        current->toMain->alreadydis = dis;
                        current->toMain->nearest = head;
                        if (current->v->x ==v->x && current->v->y ==v->y ){
                            
                        }
                        else{
                            Enqueue(q, current->toMain);
                        }
                    }
                    current = current->next;
                }
            }
        }
        break;
    }
    Stack *stack = CreateStack();
    for (int i=0;i< V_num;i++){
        if (g->edges[i]!=NULL && g->edges[i]->v->x == v->x && g->edges[i]->v->y == v->y){
            VertexNode *current = g->edges[i];
            if(current->nearest == NULL){
                break;
            }
            while (current){
                if(stack->top == NULL){
                    StackNode *node = CreateStackNode(current->v);
                    stack->top = node;
                }
                else{
                    StackNode *node = CreateStackNode(current->v);
                    node->next = stack->top;
                    stack->top = node;
                }
                
                current = current ->nearest;
            }
            break;
        }
    }
    StackNode *pointer = stack->top;
    while(pointer){
        printf("(%d,%d)",pointer->v->x,pointer->v->y);
        pointer = pointer->next;
    }
    for (int i=0;i<V_num; i++){
        if(g->edges[i]!=NULL){
            g->edges[i]->showed = 0;
            g->edges[i]->nearest = NULL;
            g->edges[i]->alreadydis = INT_MAX;
            g->edges[i]->Qnext = NULL;
        }
    }
    printf("\n");
}

void freeAdjancyList(VertexNode *head){
    if(head!=NULL){
        freeAdjancyList(head->next);
        free(head);
    }
}
// this function will free n node and m edges totally, so the time complexity will be O(m+n)
void FreeGraph(Graph g)
{
    if (g==NULL){
        return;
    }
    for (int i=0; i<V_num; i++){
        freeAdjancyList(g->edges[i]);
    }
    free(g);
}

// Add the time complexity analysis of ShowGraph() here
// this function use a queue to implement a breadth first search and traverse each vertex once, so the overall time complexity is O(m)
void ShowGraph(Graph g)
{
    Queue *q = CreateQueue();
    for (int i=0;i< V_num;i++){
        if (g->edges[i]!=NULL &&g->edges[i]->showed !=1){
            g->edges[i]->alreadydis = 0;
            Enqueue(q, g->edges[i]);
            while(q->head){
                VertexNode *head = Dequeue(q);
                head->showed = 1;
                VertexNode *current = head->next;
                while(current){
                    if (current->toMain->showed!=1){
                        printf("(%d,%d)-(%d,%d) ",head->v->x,head->v->y,current->v->x,current->v->y);
                        if (Inqueue(q,current->toMain)){
                            Enqueue(q, current->toMain);
                        }
                    }
                    current = current->next;
                }
            }
            printf("\n");
        }
    }
    for (int i=0;i< V_num;i++){
        if(g->edges[i]!=NULL){
            g->edges[i]->showed = 0;
            g->edges[i]->Qnext = NULL;
        }
    }
}


int main() //sample main for testing
{
    Graph g1;
    Edge *e_ptr;
    Vertex *v1, *v2;
    
    // Create an empty graph g1;
    g1=CreateEmptyGraph();
    
    // Create first connected component
    // Insert edge (0,0)-(0,10)
    e_ptr = (Edge*) malloc(sizeof(Edge));
    assert(e_ptr != NULL);
    v1=(Vertex*) malloc(sizeof(Vertex));
    assert(v1 != NULL);
    v2=(Vertex *) malloc(sizeof(Vertex));
    assert(v2 != NULL);
    v1->x=0;
    v1->y=0;
    v2->x=0;
    v2->y=10;
    e_ptr->p1=v1;
    e_ptr->p2=v2;
    if (InsertEdge(g1, e_ptr)==0) printf("edge exists\n");
    
    // Insert edge (0,0)-(5,6)
    e_ptr = (Edge*) malloc(sizeof(Edge));
    assert(e_ptr != NULL);
    v1=(Vertex*) malloc(sizeof(Vertex));
    assert(v1 != NULL);
    v2=(Vertex *) malloc(sizeof(Vertex));
    assert(v2 != NULL);
    v1->x=0;
    v1->y=0;
    v2->x=5;
    v2->y=6;
    e_ptr->p1=v1;
    e_ptr->p2=v2;
    if (InsertEdge(g1, e_ptr)==0) printf("edge exists\n");
    
    // Insert edge (0, 10)-(10, 10)
    e_ptr = (Edge*) malloc(sizeof(Edge));
    assert(e_ptr != NULL);
    v1=(Vertex*) malloc(sizeof(Vertex));
    assert(v1 != NULL);
    v2=(Vertex *) malloc(sizeof(Vertex));
    assert(v2 != NULL);
    v1->x=0;
    v1->y=10;
    v2->x=10;
    v2->y=10;
    e_ptr->p1=v1;
    e_ptr->p2=v2;
    if (InsertEdge(g1, e_ptr)==0) printf("edge exists\n");
    
    // Insert edge (0,10)-(5,6)
    e_ptr = (Edge*) malloc(sizeof(Edge));
    assert(e_ptr != NULL);
    v1=(Vertex*) malloc(sizeof(Vertex));
    assert(v1 != NULL);
    v2=(Vertex *) malloc(sizeof(Vertex));
    assert(v2 != NULL);
    v1->x=0;
    v1->y=10;
    v2->x=5;
    v2->y=6;
    e_ptr->p1=v1;
    e_ptr->p2=v2;
    if (InsertEdge(g1, e_ptr)==0) printf("edge exists\n");
    
    // Insert edge (0,0)-(5,4)
    e_ptr = (Edge*) malloc(sizeof(Edge));
    assert(e_ptr != NULL);
    v1=(Vertex*) malloc(sizeof(Vertex));
    assert(v1 != NULL);
    v2=(Vertex *) malloc(sizeof(Vertex));
    assert(v2 != NULL);
    v1->x=0;
    v1->y=0;
    v2->x=5;
    v2->y=4;
    e_ptr->p1=v1;
    e_ptr->p2=v2;
    if (InsertEdge(g1, e_ptr)==0) printf("edge exists\n");
    
    // Insert edge (5, 4)-(10, 4)
    e_ptr = (Edge*) malloc(sizeof(Edge));
    assert(e_ptr != NULL);
    v1=(Vertex*) malloc(sizeof(Vertex));
    assert(v1 != NULL);
    v2=(Vertex *) malloc(sizeof(Vertex));
    assert(v2 != NULL);
    v1->x=5;
    v1->y=4;
    v2->x=10;
    v2->y=4;
    e_ptr->p1=v1;
    e_ptr->p2=v2;
    if (InsertEdge(g1, e_ptr)==0) printf("edge exists\n");
    
    // Insert edge (5,6)-(10,6)
    e_ptr = (Edge*) malloc(sizeof(Edge));
    assert(e_ptr != NULL);
    v1=(Vertex*) malloc(sizeof(Vertex));
    assert(v1 != NULL);
    v2=(Vertex *) malloc(sizeof(Vertex));
    assert(v2 != NULL);
    v1->x=5;
    v1->y=6;
    v2->x=10;
    v2->y=6;
    e_ptr->p1=v1;
    e_ptr->p2=v2;
    if (InsertEdge(g1, e_ptr)==0) printf("edge exists\n");
    
    // Insert edge (10,10)-(10,6)
    e_ptr = (Edge*) malloc(sizeof(Edge));
    assert(e_ptr != NULL);
    v1=(Vertex*) malloc(sizeof(Vertex));
    assert(v1 != NULL);
    v2=(Vertex *) malloc(sizeof(Vertex));
    assert(v2 != NULL);
    v1->x=10;
    v1->y=10;
    v2->x=10;
    v2->y=6;
    e_ptr->p1=v1;
    e_ptr->p2=v2;
    if (InsertEdge(g1, e_ptr)==0) printf("edge exists\n");
    
    // Insert edge (10, 6)-(10, 4)
    e_ptr = (Edge*) malloc(sizeof(Edge));
    assert(e_ptr != NULL);
    v1=(Vertex*) malloc(sizeof(Vertex));
    assert(v1 != NULL);
    v2=(Vertex *) malloc(sizeof(Vertex));
    assert(v2 != NULL);
    v1->x=10;
    v1->y=6;
    v2->x=10;
    v2->y=4;
    e_ptr->p1=v1;
    e_ptr->p2=v2;
    if (InsertEdge(g1, e_ptr)==0) printf("edge exists\n");
    
    // Create second connected component
    // Insert edge (20,4)-(20,10)
    e_ptr = (Edge*) malloc(sizeof(Edge));
    assert(e_ptr != NULL);
    v1=(Vertex*) malloc(sizeof(Vertex));
    assert(v1 != NULL);
    v2=(Vertex *) malloc(sizeof(Vertex));
    assert(v2 != NULL);
    v1->x=20;
    v1->y=4;
    v2->x=20;
    v2->y=10;
    e_ptr->p1=v1;
    e_ptr->p2=v2;
    if (InsertEdge(g1, e_ptr)==0) printf("edge exists\n");
    
    // Insert edge (20,10)-(30,10)
    e_ptr = (Edge*) malloc(sizeof(Edge));
    assert(e_ptr != NULL);
    v1=(Vertex*) malloc(sizeof(Vertex));
    assert(v1 != NULL);
    v2=(Vertex *) malloc(sizeof(Vertex));
    assert(v2 != NULL);
    v1->x=20;
    v1->y=10;
    v2->x=30;
    v2->y=10;
    e_ptr->p1=v1;
    e_ptr->p2=v2;
    if (InsertEdge(g1, e_ptr)==0) printf("edge exists\n");
    
    // Insert edge (25,5)-(30,10)
    e_ptr = (Edge*) malloc(sizeof(Edge));
    assert(e_ptr != NULL);
    v1=(Vertex*) malloc(sizeof(Vertex));
    assert(v1 != NULL);
    v2=(Vertex *) malloc(sizeof(Vertex));
    assert(v2 != NULL);
    v1->x=25;
    v1->y=5;
    v2->x=30;
    v2->y=10;
    e_ptr->p1=v1;
    e_ptr->p2=v2;
    if (InsertEdge(g1, e_ptr)==0) printf("edge exists\n");
    
    //Display graph g1
    ShowGraph(g1);
    
    // Find the shortest path between (0,0) and (10,6)
    v1=(Vertex*) malloc(sizeof(Vertex));
    assert(v1 != NULL);
    v2=(Vertex *) malloc(sizeof(Vertex));
    assert(v2 != NULL);
    v1->x=0;
    v1->y=0;
    v2->x=10;
    v2->y=6;
    ShortestPath(g1, v1, v2);
    free(v1);
    free(v2);
    
    // Delete edge (0,0)-(5, 6)
    e_ptr = (Edge*) malloc(sizeof(Edge));
    assert(e_ptr != NULL);
    v1=(Vertex*) malloc(sizeof(Vertex));
    assert(v1 != NULL);
    v2=(Vertex *) malloc(sizeof(Vertex));
    assert(v2 != NULL);
    v1->x=0;
    v1->y=0;
    v2->x=5;
    v2->y=6;
    e_ptr->p1=v1;
    e_ptr->p2=v2;
    DeleteEdge(g1, e_ptr);
    free(e_ptr);
    free(v1);
    free(v2);
    
    // Display graph g1
    ShowGraph(g1);
    
    // Find the shortest path between (0,0) and (10,6)
    v1=(Vertex*) malloc(sizeof(Vertex));
    assert(v1 != NULL);
    v2=(Vertex *) malloc(sizeof(Vertex));
    assert(v2 != NULL);
    v1->x=0;
    v1->y=0;
    v2->x=10;
    v2->y=6;
    ShortestPath(g1, v1, v2);
    free(v1);
    free(v2);
    
    // Find the shortest path between (0,0) and (25,5)
    v1=(Vertex*) malloc(sizeof(Vertex));
    assert(v1 != NULL);
    v2=(Vertex *) malloc(sizeof(Vertex));
    assert(v2 != NULL);
    v1->x=0;
    v1->y=0;
    v2->x=25;
    v2->y=5;
    ShortestPath(g1, v1, v2);
    free(v1);
    free(v2);
    
    // Find reachable vertices of (0,0)
    v1=(Vertex*) malloc(sizeof(Vertex));
    assert(v1 != NULL);
    v1->x=0;
    v1->y=0;
    ReachableVertices(g1, v1);
    free(v1);
    
    // Find reachable vertices of (20,4)
    v1=(Vertex*) malloc(sizeof(Vertex));
    assert(v1 != NULL);
    v1->x=20;
    v1->y=4;
    ReachableVertices(g1, v1);
    free(v1);
    
    // Free graph g1
    FreeGraph(g1);
    
    return 0;
}
