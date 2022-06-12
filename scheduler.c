#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include "item.h"

void list();
void add();
void delete();
void update();
int compareDate(int x, int y);
void load();
void save();

struct Item* gStart = NULL;
int items = 0;

int main()
{
    load();
    
   int input;  /* variable */


    while (input != 'q')
    {
        printf("Enter a command - l(ist), a(dd), d(elete), u(pdate), q(uit): ");
        input = getc(stdin);

        switch( input ) 
        {
        case 'l':
            list();
            break;
        case 'a':
            add();
            break;
        case 'd':
            delete();
            break;
        case 'u':
            update();
            break;
        }
        getc(stdin);
    }
	     
    save();
}

void  list(){
    struct Item* temp = (struct Item*) malloc(sizeof(struct Item));
    int counter = 1;
    temp = gStart;
    while(temp != NULL){
        printf("[%d] %08d: %s\n", counter,temp->date, temp->sched);
        temp = temp->next;
        counter++;
    }
    if(items == 0){
        printf("Your schedule is empty\n");
    }
}


void  add(){
    int* input = (int*) malloc(sizeof(int) * 100);
    char* todo = (char*) malloc(sizeof(char) * 100);

    do{
        printf("Enter date\n");
        scanf("%d", input);
    } while((*input/1000000) > 12 || (*input/1000000) < 0 || ((*input/10000)%100) > 31 
            || ((*input/10000)%100) < 0 || *input%10000 < 2000);


    struct Item* newItem = (struct Item*) malloc(sizeof(struct Item));
    newItem->date = *input;

    do{
        printf("Enter todo\n");
        scanf("%s", todo);
    } while(strlen(todo) > 80);

    newItem->sched = todo;


    //check if gStart is null
    if(gStart == NULL){
        gStart = newItem;
        newItem->next = NULL;
    }
    else{
        struct Item* curr = (struct Item*) malloc(sizeof(struct Item));
        curr = gStart;

        if(compareDate(curr->date,newItem->date) == 0){
            gStart = newItem;
            gStart->next = curr;
        }
        else{
            for(int i = 0; i < items; i++){
                if(curr->next == NULL){
                    curr->next = newItem;
                    newItem->next = NULL;
                } else{
                    if(compareDate(curr->next->date,newItem->date) == 0){

                        newItem->next = curr->next;
                        curr->next = newItem;
                        break;
                        }
                   }   
                curr = curr->next;
             }

        }
    }



    
    items++;

}

void delete(){
    int* input = (int*) malloc(sizeof(int) * 100);

    do{
        printf("Enter item to delete: ");
        scanf("%d", input);
    } while((*input <1) || (*input> items));

    struct Item* temp = (struct Item*) malloc(sizeof(struct Item));
    temp = gStart;

    if(items ==  0){
        printf("Add to schedule first");
    }
    else if(*input == 1){
        gStart = gStart->next;
    }
    else{
        for(int i = 0; i<(*input)-2;i++){
            temp = temp->next;
        }
        temp->next = temp->next->next;
    }

    
    items--;

}

void update(){
    int* input = (int*) malloc(sizeof(int) * 100);

    do{
        printf("Enter item to update: ");
        scanf("%d", input);
    } while((*input <1) || (*input> items));

    struct Item* temp = (struct Item*) malloc(sizeof(struct Item));
    temp = gStart;

    for(int i = 1; i<*input; i++){
        temp = temp->next;
    }

    char* todo = (char*) malloc(sizeof(char) * 100);
    do{
        printf("Enter todo\n");
        scanf("%s", todo);
    } while(strlen(todo) > 80);

    temp->sched = todo;
   
}


int compareDate(int x, int y){
    int xtotal = (x%10000)*365 + (x/1000000)*31 + ((x/10000)%100);
    int ytotal = (y%10000)*365 + (y/1000000)*31 + ((y/10000)%100);

    if(xtotal>ytotal){
        return 0;
    }
    else{
        return 1;
    }


}


void load(){
    FILE* fptr;
    if((fptr = fopen("/Users/robinyang/schedule.db", "r")) != NULL){
        struct Item* curr;
        gStart = curr = NULL;

        char* line = NULL;
        size_t linecap = 9;
        ssize_t linelen;
        while((linelen = getline(&line, &linecap, fptr)) >0){
            int* input = (int*) malloc(sizeof(int) * 100);
            char* todo = (char*) malloc(sizeof(char) * 100);
            struct Item* temp = (struct Item*) malloc(sizeof(struct Item));
            sscanf(line,"%d %s", input, todo);
            temp->date = *input;
            temp->sched = todo;
            temp->next = NULL;

            if(gStart == NULL){
                curr = gStart = temp;
                items = 1;
            }
            else{
                curr = curr->next = temp;
                items++;
            }

        }   

    fclose(fptr);
    }

}

void save(){
    FILE* fptr;
    fptr = fopen("/Users/robinyang/schedule.db", "w");
    struct Item* item = (struct Item*) malloc(sizeof(struct Item));
    item = gStart;
    for(int i = 0; i < items; i++){
        fprintf(fptr, "%d %s\n", item->date, item->sched);
        item = item->next;
    }
    fclose(fptr);
    
}

