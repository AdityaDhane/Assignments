#include"database.h"

database db[3];
char carnames[][20] = {"Honda City","Mercedes Benz","Jaguar XE","Toyota Yaris","Hyundai Venue","Suzuki Brezza","TATA Nexon","Jeep Compass","Suzuki Alto","Suzuki Swift","TATA Tiago","Ford Figo"};

// **************A***************
void initialize();

carnode* get_car(carnode* root,int vin)
{
    carnode* c=NULL;
    if(root)
    {
        if(root->data.VIN>vin)
            c = get_car(root->left,vin);
        else if(root->data.VIN<vin)
            c = get_car(root->right,vin);
        else if(root->data.VIN==vin)
            c = root;
    }
    return c;
}
spernode* get_sper(spernode* root,int id)
{
    spernode* c;
    if(root->data.id>id)
        c = get_sper(root->left,id);
    else if(root->data.id<id)
        c = get_sper(root->right,id);
    else 
        c = root;
    return c;
}



custnode* get_vin2(custnode* root,int vin)
{
    custnode* c=NULL;
    if(root)
    {
        if(root->data.carData.VIN>vin)
            c = get_vin2(root->left,vin);
        else if(root->data.carData.VIN<vin)
            c = get_vin2(root->right,vin);
        else 
            c = root;
    }
    return c;
}
custnode* get_vin1(spernode* root,int vin)
{
    if(root)
    {
        custnode* c;
        c = get_vin2(root->data.customers,vin);
        if(!c)
            c = get_vin1(root->left,vin);
        if(!c)
            c = get_vin1(root->right,vin);
        return c;
    }
    return NULL;
}

// *************G***************
void get_vin(int vin)
{
    if(vin/1000>23)
        printf("\nCar doesn't exist\n");
    else
    {
        int i=vin/1000-1;
        carnode* root = get_car(db[i].cars,vin);
        printf("\n------------------------------------------------------------------------\n");
        if(root)
        {
            char s1[10],s2[10];
            ftype(root->data.fuel,s1);
            ctype(root->data.type,s2);
            printf("\nCar is present in Showroom %d\n",i+1);
            printf("VIN:%d   Name:%s  color:%s   price:%d   fuel:%s   type:%s\n",root->data.VIN,root->data.name,root->data.color,root->data.price,s1,s2);
        }
        else
        {
            custnode* c;
            spernode* s;
            char s3[10];
            char s1[10],s2[10];
            c = get_vin1(db[i].spersons,vin);
            ftype(c->data.carData.fuel,s1);
            ctype(c->data.carData.type,s2);
            ptype(c->data.paytype,s3);
            s = get_sper(db[i].spersons,c->data.id);
            printf("\nCar was Sold to %s by %s paytype:%s\n",c->data.name,s->data.name,s3);
            printf("VIN:%d  Name:%s  color:%s   price:%d   fuel:%s   type:%s\n",c->data.carData.VIN,c->data.carData.name,c->data.carData.color,c->data.carData.price,s1,s2);
        }
        printf("\n------------------------------------------------------------------------\n");
    }
}

// *************H***************
void get_vinrange(int vin1,int vin2)
{
    int i;
    for(i=vin1;i<=vin2;i++)
        get_vin(i);
}

void car_count1(custnode* root,int* a)
{
    if(root)
    {
        int i = root->data.carData.VIN%100;
        a[i]++;
        car_count1(root->left,a);
        car_count1(root->right,a);
    }
}
void car_count(spernode* root,int* a)
{
    if(root)
    {
        car_count1(root->data.customers,a);
        car_count(root->left,a);
        car_count(root->right,a);
    }
}
// *************C***************
void most_pop_car()
{
    int i,a[24]={0},max=0,vin;
    char s[10];
    for(i=0;i<3;i++)
        car_count(db[i].spersons,a);
    for(i=0;i<24;i++)
    {
        if(max<a[i])
        {
            max=a[i];
            vin=i;
        }
    }
    i=vin;
    if(i>11)
    {
        i-=12;    
        ftype(diesel,s);
    }
    else 
        ftype(petrol,s);
    printf("\n------------------------------------------------------------------------\n");
    printf("\nMost popular car in among all Showrooms is **%s** - %s version\n",carnames[i],s);
    printf("\n------------------------------------------------------------------------\n");
}



void max_sales(spernode* root,int* max,int* id)
{
    if(root)
    {
        if(root->data.sales>*max)
        {
            *max=root->data.sales;
            *id=root->data.id;
        }
        max_sales(root->left,max,id);
        max_sales(root->right,max,id);
    }
}
// **************D***************
void best_sper()
{
    int i,max=0,id;
    spernode* s;
    for(i=0;i<3;i++)
        max_sales(db[i].spersons,&max,&id);
    i=id/100-1;
    s = get_sper(db[i].spersons,id);

    printf("\n------------------------------------------------------------------------\n");
    printf("Most successful sales person is %s id:%d with sales:%d\n",s->data.name,s->data.id,s->data.sales);
    s->data.commission+= s->data.sales/100;
    printf("he is awarded extra incentive of %d\n",s->data.sales/100);
    printf("------------------------------------------------------------------------\n");
    
}



// **************E***************
void sellx(int i,int id,int vin,char* name,char* address,int mobile,paytype p)
{
    bool taller = false;
    char s1[10],s2[10];
    int downpay,months,interest;
    car c = get_car(db[i].cars,vin)->data;
    db[i].cars = remove_car(db[i].cars,vin,&taller);
    if(p==loan)
    {
        printf("Enter downpayment greater than %d :",20*c.price/100);
        printf("%d\n",downpay);
        printf("Enter no of months 36/60/84: ");
        scanf("%d",&months);
        if(months==36)
            interest=c.price*8.5/100;
        else if(months==60)
            interest=(int)(c.price*8.75)/100;
        else 
            interest=c.price*9/100;
    }
    else 
    {
        downpay=0;
        months=0;
        interest=0;
    }
    spernode* s = get_sper(db[i].spersons,id);
    s->data.sales+=c.price;
    s->data.commission = 2*s->data.sales/100;
    s->data.customers = insert_cust(s->data.customers,initialize_cust(c,name,address,mobile,p,interest,months,downpay,s->data.id),&taller);

    ftype(c.fuel,s1);
    ctype(c.type,s2);
    printf("\n------------------------------------------------------------------------\n");
    printf("*********  Purchase Successful :  *********\n");
    printf("Customer: %s | Mobile: %d\n",name,mobile);
    printf("Car: %s | price: %d | fuel: %s | type: %s\n",c.name,c.price,s1,s2);
    if(p==loan)
        printf("EMI:- Down payment: %d | Months: %d | Interest: %d\n",downpay,months,interest);
    else
        printf("Payment by Cash\n");
    printf("------------------------------------------------------------------------\n\n");
}
void sell()
{
    int i,id,vin,mobile,p;
    paytype pt,months,downpay;
    char name[10],address[10];

    printf("Enter Showroom Number : ");
    scanf("%d",&i);
    printf("\nCars available in Showroom :\n");
    print_cars(db[i-1].cars);
    printf("\nEnter Car VIN : ");
    scanf("%d",&vin);
    printf("\nSales persons in Showroom :\n");
    print_spers(db[i-1].spersons);
    printf("\nEnter Sales person ID : ");
    scanf("%d",&id);
    printf("\nEnter Customer Name : ");
    scanf("%s",&name);
    printf("\nEnter Customer Address : ");
    scanf("%s",&address);
    printf("\nEnter Customer Mobile : ");
    scanf("%d",&mobile);
    printf("\nChoose paytype 1:Cash  2:Loan   : ");
    scanf("%d",&p);
    if(p==1)
        pt=cash;
    else 
        pt=loan;
    sellx(i-1,id,vin,name,address,mobile,pt);
}



// **************B***************
void add_sales_personx(int i,char* name,int x)
{
    sper s;
    int id;
    bool taller=false;
    db[i].no_spersons++;
    id=(i+1)*100+db[i].no_spersons;
    s = initialize_sper(id,name,5000000,0,0,NULL);
    db[i].spersons = insert_sper(db[i].spersons,s,&taller);
    if(x)
    {
        printf("\n------------------------------------------------------------------------\n");
        printf("New Sales person %s  ID:%d\n",s.name,id);   
        printf("\n------------------------------------------------------------------------\n");
    }

}
void add_sales_person()
{
    int i;
    char name[10];
    printf("Enter Showroom Number : ");
    scanf("%d",&i);
    i--;
    printf("Enter name of sales person : ");
    scanf("%s",&name);
    add_sales_personx(i,name,1);
}


int main()
{
    initialize();
    int i,j,k;
    printf("\nChoose 1 option:\n1 : Print Avialable cars\n2 : Add sales person\n3 : Most popular car\n4 : Best Sales person\n5 : Sell a Car\n6 : Get a Car\n7:Get Range of cars\n");
    printf("your choice : ");
    scanf("%d",&i);
    while(i>0 && i<8)
    {
        if(i==1)
        {
            printf("Enter showroom no : ");
            scanf("%d",&j);
            print_cars(db[j].cars);
        }
        else if(i==2)
            add_sales_person();
        else if(i==3)
            most_pop_car();
        else if(i==4)
            best_sper();
        else if(i==5)
            sell();
        else if(i==6)
        {
            printf("Enter VIN : ");
            scanf("%d",&j);
            get_vin(j);
        }
        else 
        {
            printf("Enter VIN1 : ");
            scanf("%d",&j);
            printf("Enter VIN2 : ");
            scanf("%d",&k);
            get_vinrange(j,k);
        }
        printf("\nChoose 1 option:\n1 : Print Avialable cars\n2 : Add sales person\n3 : Most popular car\n4 : Best Sales person\n5 : Sell a Car\n6 : Get a Car\n7:Get Range of cars\n");
        printf("your choice : ");
        scanf("%d",&i);
    }
    
}

void initial_sell(int i,int id,int vin,char* name,char* address,int mobile,paytype p,int downpay,int months)
{
    bool taller = false;
    int interest;
    car c = get_car(db[i].cars,vin)->data;
    db[i].cars = remove_car(db[i].cars,vin,&taller);
    spernode* s = get_sper(db[i].spersons,id);
    s->data.sales+=c.price;
    s->data.commission = 2*s->data.sales/100;
    if(months==36)
        interest=c.price*8.5/100;
    else if(months==60)
        interest=(int)(c.price*8.75)/100;
    else 
        interest=c.price*9/100;
    
    s->data.customers = insert_cust(s->data.customers,initialize_cust(c,name,address,mobile,p,interest,months,downpay,id),&taller);
}
void initialize()
{
    int i;
    car c;
    bool taller=false;
    for(i=0;i<3;i++)
    {
        db[i].spersons=NULL;
        db[i].no_spersons=0;
        db[i].cars=NULL;
        db[i].sold=0;
        db[i].available=24;
    }

    for(i=0;i<3;i++)
    {
        int id=1000*(i+1);
        db[i].cars = insert_car(db[i].cars,initialize_car(id,"Honda City","silver",1000000,petrol,Sedan),&taller);   
        db[i].cars = insert_car(db[i].cars,initialize_car(id+1,"Mercedes Benz","black",3000000,petrol,Sedan),&taller);   
        db[i].cars = insert_car(db[i].cars,initialize_car(id+2,"Jaguar XE","red",4400000,petrol,Sedan),&taller);   
        db[i].cars = insert_car(db[i].cars,initialize_car(id+3,"Toyota Yaris","yellow",800000,petrol,Sedan),&taller);   
        db[i].cars = insert_car(db[i].cars,initialize_car(id+4,"Hyundai Venue","green",1100000,petrol,SUV),&taller);   
        db[i].cars = insert_car(db[i].cars,initialize_car(id+5,"Suzuki Brezza","gold",800000,petrol,SUV),&taller);   
        db[i].cars = insert_car(db[i].cars,initialize_car(id+6,"TATA Nexon","silver",600000,petrol,SUV),&taller);   
        db[i].cars = insert_car(db[i].cars,initialize_car(id+7,"Jeep Compass","black",1600000,petrol,SUV),&taller);   
        db[i].cars = insert_car(db[i].cars,initialize_car(id+8,"Suzuki Alto","green",300000,petrol,Hatchback),&taller);   
        db[i].cars = insert_car(db[i].cars,initialize_car(id+9,"Suzuki Swift","blue",500000,petrol,Hatchback),&taller);   
        db[i].cars = insert_car(db[i].cars,initialize_car(id+10,"TATA Tiago","silver",400000,petrol,Hatchback),&taller);   
        db[i].cars = insert_car(db[i].cars,initialize_car(id+11,"Ford Figo","black",600000,petrol,Hatchback),&taller);   
        db[i].cars = insert_car(db[i].cars,initialize_car(id+12,"Honda City","black",1000000,diesel,Sedan),&taller);   
        db[i].cars = insert_car(db[i].cars,initialize_car(id+13,"Mercedes Benz","silver",3000000,diesel,Sedan),&taller);   
        db[i].cars = insert_car(db[i].cars,initialize_car(id+14,"Jaguar XE","red",4400000,diesel,Sedan),&taller);   
        db[i].cars = insert_car(db[i].cars,initialize_car(id+15,"Toyota Yaris","pink",800000,diesel,Sedan),&taller);   
        db[i].cars = insert_car(db[i].cars,initialize_car(id+16,"Hyundai Venue","blue",1100000,diesel,SUV),&taller);   
        db[i].cars = insert_car(db[i].cars,initialize_car(id+17,"Suzuki Brezza","purple",800000,diesel,SUV),&taller);   
        db[i].cars = insert_car(db[i].cars,initialize_car(id+18,"TATA Nexon","silver",600000,diesel,SUV),&taller);   
        db[i].cars = insert_car(db[i].cars,initialize_car(id+19,"Jeep Compass","black",1600000,diesel,SUV),&taller);   
        db[i].cars = insert_car(db[i].cars,initialize_car(id+20,"Suzuki Alto","green",300000,diesel,Hatchback),&taller);   
        db[i].cars = insert_car(db[i].cars,initialize_car(id+21,"Suzuki Swift","black",500000,diesel,Hatchback),&taller);   
        db[i].cars = insert_car(db[i].cars,initialize_car(id+22,"TATA Tiago","silver",400000,diesel,Hatchback),&taller);   
        db[i].cars = insert_car(db[i].cars,initialize_car(id+23,"Ford Figo","black",600000,diesel,Hatchback),&taller);   
    }
    add_sales_personx(0,"Vijay",0);   
    add_sales_personx(0,"Arjun",0);   
    add_sales_personx(1,"Narendra",0);   
    add_sales_personx(1,"Amit",0);   
    add_sales_personx(2,"Rahul",0);   
    add_sales_personx(2,"Ram",0); 

    initial_sell(0,101,1015,"Aditya","Panvel",91439481,cash,0,0);  
    initial_sell(0,101,1019,"Varad","Nashik",91439372,cash,0,0);  
    initial_sell(0,101,1008,"Pushpy","UP",91423472,cash,0,0);  
    initial_sell(0,101,1023,"Sid","Nashik",91439373,loan,300000,36);  
    initial_sell(0,102,1002,"Prem","Pali",95609173,cash,0,0);  
    initial_sell(0,102,1013,"Yash","UK",90439263,cash,0,0);  
    initial_sell(0,102,1020,"Hardik","Pune",22347263,cash,0,0);  
    initial_sell(0,102,1010,"Vishal","US",90137263,loan,100000,84);  

    initial_sell(1,201,2003,"Sam","Pali",95609173,cash,0,0);  
    initial_sell(1,201,2013,"Bob","UK",90439263,cash,0,0);  
    initial_sell(1,201,2021,"Oggy","Pune",22347263,cash,0,0);  
    initial_sell(1,201,2018,"Will","US",90137263,loan,200000,60);  
    initial_sell(1,202,2016,"Sam","Pali",95609173,cash,0,0);  
    initial_sell(1,202,2022,"Bob","UK",90439263,cash,0,0);  
    initial_sell(1,202,2007,"Oggy","Pune",22347263,cash,0,0);  
    initial_sell(1,202,2010,"Will","US",90137263,loan,200000,84);  

    initial_sell(2,301,3001,"Luffy","Pali",95609173,cash,0,0);  
    initial_sell(2,301,3013,"Zoro","UK",90439263,cash,0,0);  
    initial_sell(2,301,3017,"Sanji","Pune",22347263,cash,0,0);  
    initial_sell(2,301,3019,"Nami","US",90137263,loan,200000,36);  
    initial_sell(2,302,3000,"Brook","Pali",95609173,cash,0,0);  
    initial_sell(2,302,3011,"Chopper","UK",90439263,cash,0,0);  
    initial_sell(2,302,3004,"Franky","Pune",22347263,cash,0,0);  
    initial_sell(2,302,3016,"Ussop","US",90137263,loan,200000,64);  
}
