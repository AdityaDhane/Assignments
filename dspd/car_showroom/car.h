#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<stdbool.h>

typedef enum {LH,EH,RH} balancefactor;
typedef enum {Hatchback,Sedan,SUV} cartype;
typedef enum {petrol,diesel} fueltype;
typedef struct Car
{
    int VIN;
    char name[20];
    char color[10];
    int price;
    fueltype fuel;
    cartype type;
}car;
typedef struct Cartree
{
    struct Car data;
    balancefactor bf;
    struct Cartree* left;
    struct Cartree* right;
}carnode;

void ctype(cartype c,char *s)
{
    if(c==Sedan)
        strcpy(s,"Sedan");
    else if(c==SUV)
        strcpy(s,"SUV");
    else
        strcpy(s,"Hatchback");
}

void ftype(fueltype f,char* s)
{
    if(f==diesel)
        strcpy(s,"Diesel");
    else
        strcpy(s,"Petrol");
}

car initialize_car(int VIN,char *name,char* color,int price,fueltype f,cartype t)
{
    car data;
    data.VIN=VIN;
    strcpy(data.color,color);
    strcpy(data.name,name);
    data.fuel=f;
    data.type=t;
    data.price=price;
    return data;
}


void inorder_car(carnode* root)
{
    if(root)
    {
        char s1[10],s2[10];
        ftype(root->data.fuel,s1);
        ctype(root->data.type,s2);
        inorder_car(root->left);
        printf("%4d%15s%10s%10d%10s%15s\n",root->data.VIN,root->data.name,root->data.color,root->data.price,s1,s2);
        inorder_car(root->right);
    }
}

void print_cars(carnode* root)
{
    char s1[]="VIN";
    char s2[]="Name";
    char s3[]="color";
    char s4[]="price";
    char s5[]="fuel";
    char s6[]="type";
    printf("%4s%15s%10s%10s%10s%15s\n",s1,s2,s3,s4,s5,s6);
    printf("-----------------------------------------------------------------\n");
    inorder_car(root);
}

carnode* leftRotate_car(carnode* root)
{
    carnode* p = root->right;
    root->right = p->left;
    p->left = root;
    return p;
}

carnode* rightRotate_car(carnode* root)
{
    carnode* p = root->left;
    root->left = p->right;
    p->right = root;
    return p;
}

carnode* rightBalance_car(carnode* root)
{
    carnode* p=root->right,*q;
    switch(p->bf)
    {
        case EH:
            break;
        case RH:
            root->bf=p->bf=EH;
            root = leftRotate_car(root);
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
            root->right = rightRotate_car(p);
            root = leftRotate_car(root);
    }
    return root;
}

carnode* leftBalance_car(carnode* root)
{
    carnode* p=root->left,*q;
    switch(p->bf)
    {
        case EH:
            break;
        case LH:
            root->bf=p->bf=EH;
            root = rightRotate_car(root);
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
            root->left = leftRotate_car(p);
            root = rightRotate_car(root);
    }
    return root;
}

carnode* insert_car(carnode* root,car data,bool* taller)
{
    carnode* newnode;
    if(!root)
    {
        newnode = (carnode*)malloc(sizeof(carnode));
        newnode->data=data;
        newnode->left=newnode->right=NULL;
        newnode->bf=EH;
        *taller=true;
        root=newnode;
    }
    else
    {
        if(root->data.VIN>data.VIN)
        {
            root->left=insert_car(root->left,data,taller);
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
                    root=leftBalance_car(root);
                    *taller=false;
                    break;
                }
            }
        }
        else
        {
            root->right=insert_car(root->right,data,taller);
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
                    root=rightBalance_car(root);
                    *taller=false;
                    break;
                }
            }
        }
    }
    return root;
}

carnode* remove_car(carnode* root,int data,bool* shorter)
{
    if(root==NULL)
        return root;
    if(root->data.VIN>data)
    {
        root->left=remove_car(root->left,data,shorter);
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
                    root=rightBalance_car(root);   
                    break;

            }
        }
    }
    else if(root->data.VIN<data)
    {
        root->right=remove_car(root->right,data,shorter);    
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
                    root=leftBalance_car(root);
                    break;
            }
        }
    }
    else
    {
        carnode* p,*q;
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
            root->left=remove_car(root->left,p->data.VIN,shorter);
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
                        root=rightBalance_car(root);   
                        break;

                }
            }
        }
    }
    return root;
}