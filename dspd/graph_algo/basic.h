#include<stdio.h>
#include<stdlib.h>
#include<limits.h>
#define N 3
#define infinity INT_MAX/2

typedef struct Node
{
    int id;
    int weight;
    int isDirected;
    struct Node* next;
}node;
typedef struct Graph
{
    int no_nodes;
    struct Node* array[N];
}graph;
typedef struct Queue
{
    struct Node* first;
    struct Node* last;
}queue;

void initialize(graph* g)
{
    g->no_nodes=0;
    for(int i=0;i<N;i++)
        g->array[i]=NULL;
}
void print(graph g)
{
    int i;
    node* p;
    for(i=0;i<N;i++)
    {
        if(g.array[i])
        {
            printf("%d -> ",i);
            p=g.array[i]->next;
            while(p)
            {
                printf("%d ",p->id);
                p=p->next;
            }
            printf("\n");
        }
    }
    printf("\n");
}
node* makenode(int id,int weight)
{
    node* nptr=(node*)malloc(sizeof(node));
    nptr->id=id;
    nptr->weight=weight;
    nptr->next=NULL;
    return nptr;
}

void add(queue* q,int id)
{
    node *nptr = makenode(id,0);
    if(q->first)
    {
        q->last->next=nptr;
        q->last=nptr;
    }
    else
        q->first=q->last=nptr;
}
void addFront(queue* q,int id)
{
    node *nptr = makenode(id,0);
    if(q->first)
    {
        nptr->next=q->first;
        q->first=nptr;
    }
    else
        q->first=q->last=nptr;
}

void remov(queue* q)
{
    node* p;
    if(q->first)
    {
        p=q->first;
        if(!p)
            q->last=NULL;
        q->first=p->next;
        free(p);
    }
}

int isEmpty(queue q)
{
    if(q.first)
        return 0;
    return 1;
}

void matrix(graph g,int W[][N])
{
    int i,j;
    node *p;
    for(i=0;i<N;i++)
        for(j=0;j<N;j++)
        {
            if(i==j)
                W[i][j]=0;
            else 
                W[i][j]=infinity;
        }

    for(i=0;i<N;i++)
    {
        if(g.array[i])
        {
            p=g.array[i]->next;
            while(p)
            {
                W[i][p->id]=p->weight;
                p=p->next;
            }
        }
    }
}
