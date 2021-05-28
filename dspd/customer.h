#include"car.h"

typedef enum {cash,loan} paytype;
typedef struct Customer
{
    car carData;
    char name[10];
    char Address[10];
    int mobile;
    paytype paytype;
    int interest;
    int months;
    int downpay;
    int id;
}cust;
typedef struct Custree
{
    struct Customer data;
    balancefactor bf;
    struct Custree* left;
    struct Custree* right;
}custnode;

void ptype(paytype c,char *s)
{
    if(c==loan)
        strcpy(s,"Loan");
    else
        strcpy(s,"Cash");
}

cust initialize_cust(car carData,char *name,char *Address,int mobile,paytype paytype,int interest,int months,int downpay,int id)
{
    cust data;
    strcpy(data.name,name);
    strcpy(data.Address,Address);
    data.carData=carData;
    data.downpay=downpay;
    data.interest=interest;
    data.mobile=mobile;
    data.paytype=paytype;
    data.months=months;
    data.id=id;
    return data;
}

void inorder_cust(custnode* root)
{
    if(root)
    {
        inorder_cust(root->left);
        printf("%s\n",root->data.name);
        inorder_cust(root->right);
    }
}

custnode* leftRotate_cust(custnode* root)
{
    custnode* p = root->right;
    root->right = p->left;
    p->left = root;
    return p;
}

custnode* rightRotate_cust(custnode* root)
{
    custnode* p = root->left;
    root->left = p->right;
    p->right = root;
    return p;
}

custnode* rightBalance_cust(custnode* root)
{
    custnode* p=root->right,*q;
    switch(p->bf)
    {
        case EH:
            break;
        case RH:
            root->bf=p->bf=EH;
            root = leftRotate_cust(root);
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
            root->right = rightRotate_cust(p);
            root = leftRotate_cust(root);
    }
    return root;
}

custnode* leftBalance_cust(custnode* root)
{
    custnode* p=root->left,*q;
    switch(p->bf)
    {
        case EH:
            break;
        case LH:
            root->bf=p->bf=EH;
            root = rightRotate_cust(root);
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
            root->left = leftRotate_cust(p);
            root = rightRotate_cust(root);
    }
    return root;
}

custnode* insert_cust(custnode* root,cust data,bool* taller)
{
    custnode* newnode;
    if(!root)
    {
        newnode = (custnode*)malloc(sizeof(custnode));
        newnode->data=data;
        newnode->left=newnode->right=NULL;
        newnode->bf=EH;
        *taller=true;
        root=newnode;
    }
    else
    {
        if(root->data.carData.VIN>data.carData.VIN)
        {
            root->left=insert_cust(root->left,data,taller);
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
                    root=leftBalance_cust(root);
                    *taller=false;
                    break;
                }
            }
        }
        else
        {
            root->right=insert_cust(root->right,data,taller);
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
                    root=rightBalance_cust(root);
                    *taller=false;
                    break;
                }
            }
        }
    }
    return root;
}

custnode* remove_cust(custnode* root,int data,bool* shorter)
{
    if(root==NULL)
        return root;
    if(root->data.carData.VIN>data)
    {
        root->left=remove_cust(root->left,data,shorter);
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
                    root=rightBalance_cust(root);   
                    break;

            }
        }
    }
    else if(root->data.carData.VIN<data)
    {
        root->right=remove_cust(root->right,data,shorter);    
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
                    root=leftBalance_cust(root);
                    break;
            }
        }
    }
    else
    {
        custnode* p,*q;
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
            root->left=remove_cust(root->left,p->data.carData.VIN,shorter);
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
                        root=rightBalance_cust(root);   
                        break;

                }
            }
        }
    }
    return root;
}