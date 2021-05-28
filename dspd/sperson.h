#include"customer.h"

typedef struct Sperson
{
    int id;
    char name[10];
    int target;
    int sales;
    int commission;
    custnode* customers;
}sper;
typedef struct Spertree
{
    struct Sperson data;
    balancefactor bf;
    struct Spertree* left;
    struct Spertree* right;
}spernode;

sper initialize_sper(int id,char *name,int target,int sales,int commission,custnode* root)
{
    sper data;
    data.id=id;
    strcpy(data.name,name);
    data.target=target;
    data.sales=sales;
    data.commission=commission;
    data.customers=root;
    return data;
}

void inorder_sper(spernode* root)
{
    if(root)
    {
        inorder_sper(root->left);
        printf("%3d%10s\n",root->data.id,root->data.name);
        inorder_sper(root->right);
    }
}

void print_spers(spernode* root)
{
    char s1[]="ID";
    char s2[]="Name";
    printf("%3s%10s\n",s1,s2);
    printf("-------------\n");
    inorder_sper(root);
}

spernode* leftRotate_sper(spernode* root)
{
    spernode* p = root->right;
    root->right = p->left;
    p->left = root;
    return p;
}

spernode* rightRotate_sper(spernode* root)
{
    spernode* p = root->left;
    root->left = p->right;
    p->right = root;
    return p;
}

spernode* rightBalance_sper(spernode* root)
{
    spernode* p=root->right,*q;
    switch(p->bf)
    {
        case EH:
            break;
        case RH:
            root->bf=p->bf=EH;
            root = leftRotate_sper(root);
            break;
        case LH:
            q = p->left;
            switch(q->bf)
            {
                case EH:
                    root->bf=p->bf=EH;
                    break;
                case RH:
                    root->bf=LH;
                    p->bf=EH;
                    break;
                case LH:
                    root->bf=EH;
                    p->bf=RH;
                    break;
            }
            q->bf=EH;
            root->right = rightRotate_sper(p);
            root = leftRotate_sper(root);
    }
    return root;
}

spernode* leftBalance_sper(spernode* root)
{
    spernode* p=root->left,*q;
    switch(p->bf)
    {
        case EH:
            break;
        case LH:
            root->bf=p->bf=EH;
            root = rightRotate_sper(root);
            break;
        case RH:
            q = p->right;
            switch(q->bf)
            {
                case EH:
                    root->bf=p->bf=EH;
                    break;
                case RH:
                    root->bf=EH;
                    p->bf=LH;
                    break;
                case LH:
                    root->bf=RH;
                    p->bf=EH;
                    break;
            }
            q->bf=EH;
            root->left = leftRotate_sper(p);
            root = rightRotate_sper(root);
    }
    return root;
}

spernode* insert_sper(spernode* root,sper data,bool* taller)
{
    spernode* newnode;
    if(!root)
    {
        newnode = (spernode*)malloc(sizeof(spernode));
        newnode->data=data;
        newnode->left=newnode->right=NULL;
        newnode->bf=EH;
        *taller=true;
        root=newnode;
    }
    else
    {
        if(root->data.id>data.id)
        {
            root->left=insert_sper(root->left,data,taller);
            if(*taller)
            {
                switch (root->bf)
                {
                case EH:
                    root->bf=LH;
                    break;
                case RH:
                    root->bf=EH;
                    *taller=false;
                case LH:
                    root=leftBalance_sper(root);
                    *taller=false;
                    break;
                }
            }
        }
        else
        {
            root->right=insert_sper(root->right,data,taller);
            if(*taller)
            {
                switch (root->bf)
                {
                case EH:
                    root->bf=RH;
                    break;
                case LH:
                    root->bf=RH;
                    *taller=false;
                case RH:
                    root=rightBalance_sper(root);
                    *taller=false;
                    break;
                }
            }
        }
    }
    return root;
}

spernode* remove_sper(spernode* root,int data,bool* shorter)
{
    if(root==NULL)
        return root;
    if(root->data.id>data)
    {
        root->left=remove_sper(root->left,data,shorter);
        if(*shorter)
        {
            switch(root->bf)
            {
                case EH:
                    root->bf=RH;
                    *shorter=false;
                    break;
                case LH:
                    root->bf=EH;
                    break;
                case RH:
                    root=rightBalance_sper(root);   
                    break;

            }
        }
    }
    else if(root->data.id<data)
    {
        root->right=remove_sper(root->right,data,shorter);    
        if(*shorter)
        {
            switch(root->bf)
            {
                case EH:
                    root->bf=LH;
                    break;
                case RH:
                    root->bf=EH;
                    break;
                case LH:
                    root=leftBalance_sper(root);
                    break;
            }
        }
    }
    else
    {
        spernode* p,*q;
        if(!(root->left) || !(root->right))
        {
            p=root->left?root->left:root->right;
            free(root);
            *shorter=true;
            root=p;
        }
        else
        {
            p=root->left;
            while(p->right)
                p=p->right;
            root->data=p->data;
            root->left=remove_sper(root->left,p->data.id,shorter);
            if(*shorter)
            {
                switch(root->bf)
                {
                    case EH:
                        root->bf=RH;
                        *shorter=false;
                        break;
                    case LH:
                        root->bf=EH;
                        break;
                    case RH:
                        root=rightBalance_sper(root);   
                        break;

                }
            }
        }
    }
    return root;
}