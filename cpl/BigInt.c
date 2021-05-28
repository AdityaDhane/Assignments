#include<stdio.h>
#include<string.h>
#define BASE 1000000000
#define N 9
#define M 310/N

typedef enum {minus,plus} sign;
typedef struct
{
    long long a[M];                             //array to store big int 
    int n;                                      //no. of indexes used of a[] by big int 
    sign sn;                                    //stores sign of big int
}big;

void print(big);                          
big input();                    
big add(big,big);               
big sub(big,big);
big mul(big,big);

int main()
{
    int i;
    big a,b,ans;
    printf("Enter   1 : Add(+)      2 : Sub(-)      3 : Mul(*)      4 : Exit   ___\b\b");
    scanf("%d",&i);
    while(i!=4)
    {
        printf("Enter 2 BIG ints :\n");
        a=input();
        b=input();
        if(i==1)
            ans=add(a,b);
        else if(i==2)
            ans=sub(a,b);
        else
            ans=mul(a,b);
        printf("RESULT :-\n");
        print(ans);
        printf("Enter   1 : Add(+)      2 : Sub(-)      3 : Mul(*)      4 : Exit   ___\b\b");
            scanf("%d",&i);
    }
    return 0;
}

big input()
{
    sign sn=plus;
    char s[310];
    big p;
    int length,i,j,k,n,m,cond;
    scanf("%s",s);
    if(s[0]=='-')
    {
        sn=minus;
        for(i=0;s[i]!='\0';i++)
            s[i]=s[i+1];
    }
    length=strlen(s);
    m=length%N;
    n=(m==0)?(length/N):(length/N+1);
    cond=(m==0)?n:n-1;
    for(i=0,j=length-N;i<cond;i++)
    {
        p.a[i]=0;
        for(k=0;k<N;k++,j++)
        {
            p.a[i]=10*p.a[i]+(s[j]-48);
        }
        j=j-2*N;
    }
    p.a[i]=0;
    for(j=0;j<m;j++)
    {
        p.a[i]=10*p.a[i]+(s[j]-48);
    }
    while(i++<M)
        p.a[i]=0;
    p.n=n;
    p.sn=sn;
    return p;
}

big add(big p,big q)                                        //return addition of p + q
{
    big ans;
    sign x,y;
    if(p.sn==q.sn)                                          //if signs are equal then add else sub
    {
        long long carry=0;
        int i=0;
        ans.n=(p.n>q.n)?p.n:q.n;
        while(i<ans.n)
        {
            ans.a[i]=p.a[i]+q.a[i]+carry;
            carry=ans.a[i]/BASE;
            ans.a[i]=ans.a[i]%BASE;
            i++;  
        }
        if(carry!=0)
        {
            ans.a[i]=carry;
            ans.n++;
        }
        ans.sn=p.sn;
    }
    else
    {
        x=p.sn;
        y=q.sn;
        q.sn=p.sn=plus;
        ans=sub(p,q);
        if( (p.n<q.n) || (p.a[p.n]<q.a[p.n]) )
            ans.sn=y;
        else
            ans.sn=x;
    }
    return ans;
}

big sub(big p,big q)                                        //return addition of p - q
{
    big ans,temp;
    int f=0;
    if(p.sn==q.sn)                                          //if signs are equal sub else add
    {
        if( (p.n<q.n) || (p.a[p.n]<q.a[p.n]) )              //if(q is greater than p)
        {
            temp=p;
            p=q;
            q=temp;
            f=1;
        }
        int i=0;
        ans.n=(p.n>q.n)?p.n:q.n;
        while(i<ans.n)
        {
            if(p.a[i]<q.a[i])
            {
                p.a[i]+=BASE;
                p.a[i+1]--;
            }
            ans.a[i]=p.a[i]-q.a[i];
            i++;
        }
        ans.sn=(f==0)?plus:minus;
    }
    else
    {
        q.sn=p.sn;
        ans=add(p,q);
    }
    return ans;
}

big mul(big p,big q)                                        //return addition of p * q   
{
    big ans,temp;
    long long carry=0;
    int i,j;
    for(j=0;j<M;j++)
        ans.a[j]=0;
    ans.n=0;
    for(i=0;i<q.n;i++)
    {
        temp.n=i;
        carry=0;
        for(j=0;j<M;j++)
            temp.a[j]=0;
        for(j=0;j<p.n;j++)
        {
            temp.a[j+i]=p.a[j]*q.a[i]+carry;
            carry=temp.a[j+i]/BASE;
            temp.a[j+i]=temp.a[j+i]%BASE;
            temp.n++;
        }
        if(carry!=0)
        {
            temp.a[j+i]=carry;
            temp.n++;
        }
        ans=add(ans,temp);
    }
    ans.sn=(p.sn==q.sn)?plus:minus;
    return ans;
}

void print(big p)                                        //prints p
{
    int i;
    if(p.sn==minus)
        printf("%c",'-');
    printf("%lld",p.a[p.n-1]);
    for(i=p.n-2;i>=0;i--)
        printf("%09lld",p.a[i]);
    printf("\n");
    return;
}
