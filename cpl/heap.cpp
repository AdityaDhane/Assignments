// Heap Management using buddy System
#include<iostream>
#include<cmath>
#include<iomanip>
using namespace std;

#define HEAP_SIZE 2048
#define THRESHOLD 4

const int N=int(log2(HEAP_SIZE));
const int M=int(log2(THRESHOLD));
const int n=N-M+1;

typedef enum {failure,success} status;
typedef struct allocateList
{
    string name;
    int size;
    int ifrag;
    int address;
    struct allocateList* next;
}anode;
typedef struct freeList
{
    int address;
    int buddy;
    struct freeList* next;
}fnode;

void print(anode* ahead,fnode** fheads);

status allocate(anode** ahead,fnode** fheads,string name,int size)
{
    status st=success;
    if(size>HEAP_SIZE || size<THRESHOLD)
        st=failure;
    else
    {
        int i,pow2=THRESHOLD,j,add;
        fnode* ftr;
        anode* atr=*ahead,*nptr;
        for(i=0;pow2<size;i++,pow2*=2){}
        fnode* fhead=fheads[i];
        if(!fhead)
        {
            j=i;
            while(++j<n && !fheads[j]){}
            if(j==n)
                st=failure;
            else
            {
                add=fheads[j]->address;
                ftr=fheads[j];
                fheads[j]=fheads[j]->next;
                delete ftr;
                while(--j>=i)
                {
                    fheads[j]=new fnode;
                    fheads[j]->address=add+pow(2,j+M);
                    fheads[j]->buddy=1;
                    fheads[j]->next=NULL;
                }
            }   
        }
        else
        {
            add=fheads[i]->address;
            ftr=fheads[i];
            fheads[i]=fheads[i]->next;
            delete ftr;   
        }
        if(st)
        {
            nptr=new anode;
            nptr->name=name;
            nptr->next=NULL;
            nptr->address=add;
            nptr->size=pow(2,i+M);
            nptr->ifrag=nptr->size - size;
            if(!atr)
            {
                *ahead=nptr;
            }    
            else
            {
                while(atr->next)
                    atr=atr->next;
                atr->next=nptr;
            }
        }
    }
    return st;
}

status free(anode** ahead,fnode** fheads,string name)
{
    status st=success;
    int i,add,f=1,size;
    anode* atr=*ahead,*pre=NULL;
    fnode* ftr,*nptr,*pre1,*pre2;
    while(atr && atr->name!=name)
    {
        pre=atr;
        atr=atr->next;
    }
    if(!atr)
        st=failure;
    else
    {
        size=atr->size;
        i=log2(size)-M;
        add=atr->address;
        if(!pre)
            *ahead=atr->next;
        else 
            pre->next=atr->next;
        delete atr;

        nptr=new fnode;
        while(f && i<n)     
        {
            nptr->buddy=(add%(size*2))?1:0;
            f=0;
            ftr=fheads[i];
            pre1=pre2=NULL;
            while(ftr && ftr->address<add)
            {
                pre2=pre1;
                pre1=ftr;
                ftr=ftr->next;
            }

            if(nptr->buddy && pre1 && add==pre1->address + size)
            {
                if(pre2)
                    pre2->next=pre1->next;
                else
                    fheads[i]=pre1->next;
                add=pre1->address;
                delete pre1;
                f=1;
            }
            else if(!nptr->buddy && ftr && add==ftr->address - size)
            {
                if(pre1)
                    pre1->next=ftr->next;
                else
                    fheads[i]=ftr->next;
                delete ftr;
                f=1;
            }
            else
            {
                nptr->next=ftr;
                nptr->address=add;
                if(pre1)
                    pre1->next=nptr;
                else
                    fheads[i]=nptr;
            }
            size*=2;
            i++;            
        }   
    }
    return st;
}

int main()
{
    status st;
    int i,c=1,size;
    string name;
    anode* ahead;
    ahead=NULL;

    fnode* fheads[n];
    for(i=0;i<n-1;i++)
        fheads[i]=NULL;
    fheads[i]=new fnode;
    fheads[i]->address=0;
    fheads[i]->next=NULL;

    while(c!=4)
    {
        cout<<endl<<"1:Allocate       2:Free      3:Print     4:Exit"<<endl;
        cout<<"Choice : ";
        cin>>c;
        if(c==1)
        {
            cout<<"Enter name and size of memory : ";
            cin>>name;
            cin>>size;
            st=allocate(&ahead,fheads,name,size);
            if(!st)
                cout<<endl<<" Memory not available"<<endl;
        }
        else if(c==2)
        {
            cout<<"Enter name of memory : ";
            cin>>name;
            st=free(&ahead,fheads,name);
            if(!st)
                cout<<endl<<name<<" does not exist"<<endl;
        }
        else if(c==3)
            print(ahead,fheads);
    }
}

void print(anode* ahead,fnode** fheads)
{
    cout<<"--------------------------------------------------------------------"<<endl;
    int i;
    anode* atr=ahead;
    fnode* ftr;
    cout<<endl;
    cout<<"Allocated space :"<<endl;
    cout<<"Name      Address        Size"<<endl;
    cout<<"-----------------------------"<<endl;
    while(atr)
    {
        cout<<left<<setw(5)<<atr->name<<"      "<<setw(5)<<atr->address<<"          "<<setw(5)<<atr->size<<endl;
        atr=atr->next;
    }
    cout<<endl;
    cout<<"Free space :"<<endl;
    cout<<"Address      Size"<<endl;
    cout<<"-----------------"<<endl;
    for(i=0;i<n;i++)
    {
        ftr=fheads[i];
        while(ftr)
        {
            cout<<setw(5)<<ftr->address<<"        "<<setw(5)<<pow(2,i+M)<<endl;
            ftr=ftr->next;
        }
    }
    cout<<endl;
    cout<<"--------------------------------------------------------------------"<<endl;
    return;
}