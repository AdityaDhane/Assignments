#include"basic.h"

void addNode(graph* g,int id);
void deleteNode(graph*,int id);

void _addEdge(graph*,int x,int y,int weight,int f);
void _deleteEdge(graph* g,int x,int y);
//undirected = 0 , directed =1
void addEdge(graph* g,int x,int y,int weight,int f);
void deleteEdge(graph* g,int x,int y,int f);

void breadthFirst(graph g);
void _depthFirst(graph g,int id,int* f);
void depthFirst(graph g);

void topoSort(graph g);

void breadthFirstSearch(graph g,int id);
int _depthFirstSearch(graph g,int id,int* f,int x);
void depthFirstSearch(graph g,int id);

void _isConnected(graph g,int id,int* f);
int isConnected(graph g);

int _isCycle(graph g,int id,int* f,int parent);
void isCycle(graph g);

void Dijkstra(graph g,int id);

void Floyd(graph g);
void FloydPrint(int path[][N],int i,int j);

void Prim(graph g);

void _allPaths(graph g,int u,int v,int* f,int* path,int pos);
void allPaths(graph g,int u,int v);

int main()
{
    graph g;
    initialize(&g);

    addEdge(&g,0,1,1,0);
    addEdge(&g,1,2,2,0);

    print(g);
    Prim(g);
}

void addNode(graph* g,int id)
{
    if(!g->array[id])
    {
        node* nptr=(node*)malloc(sizeof(node));
        nptr->id=-1;
        nptr->next=NULL;
        nptr->weight=0;
        g->array[id]=nptr;
        g->no_nodes++;
    }
}
void deleteNode(graph* g,int id)
{
    int i;
    node *p,*q;
    for(i=0;i<N;i++)
    {
        if(i==id && g->array[i])
        {
            g->no_nodes--;
            p=g->array[i];
            while(p)
            {
                q=p->next;
                free(p);
                p=q;
            }
            g->array[i]=NULL;
        }
        else
        {
            q=g->array[i];
            if(q)
            {
                p=q->next;
                while(p)
                {
                    if(p->id==id)
                    {
                        q->next=p->next;
                        free(p);
                        p=q->next;
                    }
                    else
                    {
                        q=p;
                        p=p->next;
                    }
                }
            }
        }
    }
}

void _addEdge(graph* g,int x,int y,int weight,int f)
{
    node *p = g->array[x],*q;
    node* nptr=makenode(y,weight);
    nptr->isDirected=f;
    q=p->next;
    p->next=nptr;
    nptr->next=q;
}
void addEdge(graph* g,int x,int y,int weight,int f)
{
    addNode(g,x);
    addNode(g,y);
    if(!f)
    {
        _addEdge(g,x,y,weight,f);
        _addEdge(g,y,x,weight,f);
    }
    else 
        _addEdge(g,x,y,weight,f);
}
void _deleteEdge(graph* g,int x,int y)
{
    node *p,*q;
    q=g->array[x];
    p=q->next;
    while(p)
    {
        if(p->id==y)
        {
            q->next=p->next;
            free(p);
            p=q->next;
        }
        else
        {
            q=p;
            p=p->next;
        }
    }
}
void deleteEdge(graph* g,int x,int y,int f)
{
    if(!f)
    {
        _deleteEdge(g,x,y);
        _deleteEdge(g,y,x);
    }
    else 
        _deleteEdge(g,x,y);
}

void _depthFirst(graph g,int id,int* f)
{
    f[id]=1;
    printf("%d ",id);
    node* p = g.array[id]->next;
    while(p)
    {
        if(!f[p->id])
            _depthFirst(g,p->id,f);
        p=p->next;
    }
}
void depthFirst(graph g)
{
    int f[N]={0};
    for(int i=0;i<N;i++)
    {
        if(g.array[i] && !f[i])
            _depthFirst(g,i,f);
    }
    printf("\n");
}

void _isConnected(graph g,int id,int* f)
{
    f[id]=1;
    node* p = g.array[id]->next;
    while(p)
    {
        if(!f[p->id])
            _isConnected(g,p->id,f);
        p=p->next;
    }
}
int isConnected(graph g)
{
    int f[N]={0};
    int x=0;
    for(int i=0;x<2 && i<N;i++)
    {
        if(g.array[i] && !f[i])
        {
            x++;
            _isConnected(g,i,f);
        }
    }
    if(x==2)
    {
        printf("Disconnected\n");
        return 0;
    }
    else 
    {
        printf("Connected\n");
        return 1;
    }
}

void breadthFirst(graph g)
{
    int f[N]={0};
    node *p,*nptr;
    queue q;
    q.first=q.last=NULL;
    for(int i=0;i<N;i++)
    {
        if(g.array[i] && !f[i])
        {
            add(&q,i);
            while(q.first)
            {
                f[q.first->id]=1;
                printf("%d ",q.first->id);
                p=g.array[q.first->id]->next;
                while(p)
                {
                    if(!f[p->id])
                        add(&q,p->id);
                    p=p->next;
                }
                remov(&q);
            }
        }
    }
}

void topoSort(graph g)
{
    int i;
    int parents[N]={0};
    node *p,*nptr,*r;
    queue q;
    
    for(i=0;i<N;i++)
    {
        if(g.array[i])
        {
            p=g.array[i]->next;
            while(p)
            {
                parents[p->id]++;
                p=p->next;
            }
        }
    }

    q.first=q.last=NULL;
    for(i=0;i<N;i++)
    {
        if(g.array[i] && parents[i]==0)
            add(&q,i);
    }

    while(q.first)
    {
        p=q.first;
        printf("%d ",p->id);
        r=g.array[p->id]->next;
        while(r)
        {
            parents[r->id]--;
            if(parents[r->id]==0)
                add(&q,r->id);
            r=r->next;
        }
        remov(&q);
    }
}

void Dijkstra(graph g,int id)
{
    int dist[N],f[N]={0},min,i,k,n=g.no_nodes,path[i];
    node *p,*r;
    for(int i=0;i<N;i++)
    {
        if(g.array[i])
        {
            dist[i]=infinity;
            path[i]=-1;
        }
    }

    dist[id]=0;
    f[id]=1;
    k=id;

    while(--n)
    {
        r=g.array[k]->next;
        while(r)
        {
            if(!f[r->id] && dist[k]+r->weight<dist[r->id])
            {
                dist[r->id]=dist[k]+r->weight;
                path[r->id]=k;
            }
            r=r->next;
        }
        min=infinity;
        for(i=0;i<N;i++)
        {
            if(g.array[i] && !f[i])
            {
                if(min>=dist[i])
                {
                    min=dist[i];
                    k=i;
                }
            }
        }
        f[k]=1;
    }

    //printing min-dist and path
    for(i=0;i<N;i++)
    {
        if(g.array[i] && dist[i]!=infinity)
        {
            printf("%d[%d] : ",i,dist[i]);
            queue q;
            q.first=q.last=NULL;
            k=i;
            while(k!=id)
            {
                addFront(&q,k);
                k=path[k];
            }
            printf("%d",id);
            while(!isEmpty(q))
            {
                printf("->");
                printf("%d",q.first->id);
                remov(&q);
            }
            printf("\n");
        }
    }

}

void FloydPrint(int path[][N],int i,int j)
{
    if(path[i][j]!=-1)
    {
        FloydPrint(path,i,path[i][j]);
        printf("%d->",path[i][j]);
        FloydPrint(path,path[i][j],j);
    }
}
void Floyd(graph g)
{
    node *p;
    int i,j,k,W[N][N],path[N][N];

    matrix(g,W);
    for(i=0;i<N;i++)
        for(j=0;j<N;j++)
            path[i][j]=-1;

    for(k=0;k<N;k++)
        for(i=0;i<N;i++)
            for(j=0;j<N;j++)
                if(W[i][k]+W[k][j]<W[i][j])
                {
                    W[i][j]=W[i][k]+W[k][j];
                    path[i][j]=k;
                }

    for(i=0;i<N;i++)
    {
        for(j=0;j<N;j++)
        {
            if(W[i][j]==infinity)
                printf("- ");
            else 
                printf("%d ",W[i][j]);
        }
        printf("\n");
    }
    printf("\n");

    for(i=0;i<N;i++)
        for(j=0;j<N;j++)
            if(i!=j && W[i][j]!=infinity)
            {
                printf("[%d->%d] weight=%d : %d->",i,j,W[i][j],i);
                FloydPrint(path,i,j);
                printf("%d\n",j);
            }
}

void Prim(graph g)
{
    int i,v,min,W[N][N],mst=0;
    int nearest[N],dist[N],n=N-1;
    graph ms_tree;
    initialize(&ms_tree);

    matrix(g,W);

    for(i=1;i<N;i++)
    {
        nearest[i]=0;
        dist[i]=W[0][i];
    }
    while(n--)
    {
        min=infinity;
        for(i=1;i<N;i++)
            if(dist[i]>=0 && dist[i]<min)
            {
                min=dist[i];
                v=i;
            }

        mst+=min;
        addEdge(&ms_tree,v,nearest[v],min,0);

        dist[v]=-1;
        for(i=1;i<N;i++)
            if(W[i][v]<dist[i])
            {
                dist[i]=W[i][v];
                nearest[i]=v;
            }
    }
    printf("Minimum spanning tree value = %d\n",mst);
    print(ms_tree);
}

int _isCycle(graph g,int id,int* f,int parent)
{
    int flag=0;
    node* p = g.array[id]->next;
    f[id]=1;
    while(p && !flag)
    {
        if(p->isDirected && f[p->id])
            flag=1;
        else if(!p->isDirected && f[p->id] && p->id!=parent)
            flag=1;
        else if(!f[p->id])
            flag=_isCycle(g,p->id,f,id);
        p=p->next;
    }
    return flag;
}
void isCycle(graph g)
{
    int f[N]={0},flag=0;
    for(int i=0;!flag && i<N;i++)
    {
        if(g.array[i] && !f[i])
            flag=_isCycle(g,i,f,-1);
    }
    if(flag)
        printf("Cycle is present\n");
    else 
        printf("Cycle is not present\n");
}

void _allPaths(graph g,int u,int v,int* f,int* path,int pos)
{
    if(u!=v)
    {
        node* p = g.array[u]->next;
        f[u]=1;
        path[pos]=u;
        while(p)
        {
            if(!f[p->id])
                _allPaths(g,p->id,v,f,path,pos+1);
            p=p->next;
        }
        f[u]=0; 
    }
    else
    {
        path[pos]=v;  
        printf("%d",path[0]); 
        for(int i=1;i<=pos;i++)
            printf("->%d",path[i]); 
        printf("\n");  
    }
}
void allPaths(graph g,int u,int v)
{
    int f[N]={0},path[N],i;
    printf("path from node %d to node %d :\n",u,v);
    _allPaths(g,u,v,f,path,0);
}

void breadthFirstSearch(graph g,int id)
{
    int f[N]={0},flag=0;
    node *p,*nptr;
    queue q;
    q.first=q.last=NULL;
    for(int i=0;!flag && i<N;i++)
    {
        if(g.array[i] && !f[i])
        {
            add(&q,i);
            while(q.first && !flag)
            {
                f[q.first->id]=1;
                if(q.first->id==id)
                    flag=1;
                // printf("%d ",q.first->id);
                p=g.array[q.first->id]->next;
                while(p && !flag)
                {
                    if(!f[p->id])
                        add(&q,p->id);
                    p=p->next;
                }
                remov(&q);
            }
        }
    }
    if(flag)
        printf("node %d is present\n",id);
    else 
        printf("node %d is not present\n",id);
}

int _depthFirstSearch(graph g,int id,int* f,int x)
{
    int flag=0;
    if(id==x)
        flag=1;
    else
    {
        f[id]=1;
        // printf("%d ",id);
        node* p = g.array[id]->next;
        while(p && !flag)
        {
            if(!f[p->id])
                flag=_depthFirstSearch(g,p->id,f,x);
            p=p->next;
        }
    }
    return flag;
}
void depthFirstSearch(graph g,int id)
{
    int f[N]={0},flag=0;
    for(int i=0;i<N && !flag;i++)
    {
        if(g.array[i] && !f[i])
            flag=_depthFirstSearch(g,i,f,id);
    }
    if(flag)
        printf("node %d is present\n",id);
    else 
        printf("node %d is not present\n",id);
}

























