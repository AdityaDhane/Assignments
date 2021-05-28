#include"sperson.h"

typedef struct Database
{
    carnode* cars;
    int sold;
    int available;
    spernode* spersons;
    int no_spersons;
}database;