#include <stdio.h>
#include <string.h>
#include <stdlib.h>
 
struct Action
{
    int id1;
    int id2;
    char action;
    int priority;
};
 
typedef struct Action action;


int main(int argc, char* argv[])
{
    char input[200];
    if (argc>1) {strcpy(input, argv[1]);}
    else {fgets(input, 200, stdin);}
    double* arr = malloc(8);
    action* act = malloc(9);
    int currindex_num = 0, nested = 0, maxnested = 0, digitsafterdecimal = 1, hasdecimal = 0, totalcalculations = 0;
    for (int i = 0; i < strlen(input); ++i) //parser
    {
        switch (input[i])
        {
            
            case '+': {action a; a.action = '+'; arr[currindex_num]/=digitsafterdecimal;  digitsafterdecimal = 1; hasdecimal = 0; a.id1 = currindex_num; a.id2 = currindex_num+1; a.priority = 1; act[currindex_num] = a; ++currindex_num; arr=realloc(arr, 8*(currindex_num+1)); act=realloc(act, sizeof(action)*(currindex_num+1)); arr[currindex_num] = 0; break;}
            case '-': {action a; a.action = '-'; arr[currindex_num]/=digitsafterdecimal;  digitsafterdecimal = 1; hasdecimal = 0; a.id1 = currindex_num; a.id2 = currindex_num+1; a.priority = 1; act[currindex_num] = a; ++currindex_num; arr=realloc(arr, 8*(currindex_num+1)); act=realloc(act, sizeof(action)*(currindex_num+1)); arr[currindex_num] = 0; break;}
            case '*': {action a; a.action = '*'; arr[currindex_num]/=digitsafterdecimal;  digitsafterdecimal = 1; hasdecimal = 0; a.id1 = currindex_num; a.id2 = currindex_num+1; a.priority = 3; act[currindex_num] = a; ++currindex_num; arr=realloc(arr, 8*(currindex_num+1)); act=realloc(act, sizeof(action)*(currindex_num+1)); arr[currindex_num] = 0; break;}
            case '/': {action a; a.action = '/'; arr[currindex_num]/=digitsafterdecimal;  digitsafterdecimal = 1; hasdecimal = 0; a.id1 = currindex_num; a.id2 = currindex_num+1; a.priority = 3; act[currindex_num] = a; ++currindex_num; arr=realloc(arr, 8*(currindex_num+1)); act=realloc(act, sizeof(action)*(currindex_num+1)); arr[currindex_num] = 0; break;}
            case '(': {action a; a.action = '('; arr[currindex_num]/=digitsafterdecimal;  digitsafterdecimal = 1; hasdecimal = 0; a.id1 = -1; a.id2 = -1; a.priority = 5+nested; ++nested; act[currindex_num] = a; ++currindex_num; arr=realloc(arr, 8*(currindex_num+1)); act=realloc(act, sizeof(action)*(currindex_num+1)); arr[currindex_num] = 0; break;}
            case ')': {action a; a.action = ')'; arr[currindex_num]/=digitsafterdecimal;  digitsafterdecimal = 1; hasdecimal = 0; --nested; a.id1 = -1; a.id2 = -1; a.priority = 5+nested; act[currindex_num] = a; ++currindex_num; arr=realloc(arr, 8*(currindex_num+1)); act=realloc(act, sizeof(action)*(currindex_num+1)); arr[currindex_num] = 0; break;}
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
            case '0':
            {
                arr[currindex_num]*=10;
                arr[currindex_num]+=(input[i]-48);
                if(hasdecimal) digitsafterdecimal*=10;
                break;
            }
            case '.': hasdecimal = 1; break;
            default: printf("unrecognised characted"); return 1;
        }
        if(nested>maxnested) maxnested = nested;
    }
    arr[currindex_num]/=digitsafterdecimal;

    if (nested)
    {
        printf("not all parentesis were closed");
        return 1;
    }

    int *priority_list = malloc(currindex_num*4);
    int currpriority = 0, isinparenthesis = 0;
    int ran = 0;

    for (int c = maxnested-1; c >= 0; --c)
    {
        for (int i = 0; i < currindex_num; ++i) //search for 5+ priority (parentesis' searcher) 
        {
            if(act[i].priority==5+c) 
            {
                for (int x = 1;x < currindex_num-i-1;++x) // multiplication / division
                {
                    if (act[i+x].action==')') break; //if the parenthesis closes
                    if (act[i+x].action=='(') 
                    {
                        ++nested;
                        while (nested>0)
                        {
                            ++x;
                            if (act[i+x].action==')') --nested;
                            else if (act[i+x].action=='(') ++nested;
                        }
                    }

                    if (act[i+x].priority==3)
                    {
                        ran++;
                        priority_list[currpriority++] = i+x;
                    }
                }

                for (int x = 1;x < currindex_num-i-1;++x) // addition / subtraction
                {
                    if (act[i+x].action==')') break; //if the parenthesis closes
                    if (act[i+x].action=='(') 
                    {
                        ++nested;
                        while (nested>0)
                        {
                            ++x;
                            if (act[i+x].action==')') --nested;
                            else if (act[i+x].action=='(') ++nested;
                        }
                    }

                    if (act[i+x].priority==1)
                    {
                        ran++;
                        priority_list[currpriority++] = i+x;
                    }
                }
            }
        }
    }

    for (int i = 0; i < currindex_num; ++i) //search for 3 priority multiplication and division outside of parenthesis'
    {
        if (act[i].action=='(') {isinparenthesis = 1; continue;}
        if (act[i].action==')') {isinparenthesis = 0; continue;}
        
        if (!isinparenthesis)
        {
            if(act[i].priority==3) {ran++; priority_list[currpriority++] = i;}
        }
    }

    for (int i = 0; i < currindex_num; ++i) //search for 1 priority addition and subtraction outside of parenthesis'
    {
        if (act[i].action=='(') {isinparenthesis = 1; continue;}
        if (act[i].action==')') {isinparenthesis = 0; continue;}
        if (!isinparenthesis)
        {
            if(act[i].priority==1) {ran++; priority_list[currpriority++] = i;}
        }
    }

    for (int i = 0; i < ran; ++i) // calculation
    {
        if(ran)
        {
            switch (act[priority_list[i]].action)
            {           
                case '+': arr[act[priority_list[i]].id1]+=arr[act[priority_list[i]].id2]; if (priority_list[i]<priority_list[i+1]) {act[priority_list[i+1]].id1 = act[priority_list[i]].id1;} else {act[priority_list[i+1]].id2 = act[priority_list[i]].id1;} break;
                case '-': arr[act[priority_list[i]].id1]-=arr[act[priority_list[i]].id2]; if (priority_list[i]<priority_list[i+1]) {act[priority_list[i+1]].id1 = act[priority_list[i]].id1;} else {act[priority_list[i+1]].id2 = act[priority_list[i]].id1;} break;
                case '*': arr[act[priority_list[i]].id1]*=arr[act[priority_list[i]].id2]; if (priority_list[i]<priority_list[i+1]) {act[priority_list[i+1]].id1 = act[priority_list[i]].id1;} else {act[priority_list[i+1]].id2 = act[priority_list[i]].id1;} break;
                case '/': if (arr[act[priority_list[i]].id2]==0) {printf("division by 0"); return 1;} arr[act[priority_list[i]].id1]/=arr[act[priority_list[i]].id2]; if (priority_list[i]<priority_list[i+1]) {act[priority_list[i+1]].id1 = act[priority_list[i]].id1;} else {act[priority_list[i+1]].id2 = act[priority_list[i]].id1;} break;
                default: printf("unpredicted behaviour"); return 1;  // this shouldn't be possible, in the perfect world
            }
        }
        else
        {
            printf("%lf", arr[0]);
        }
    }

    printf("%lf\n", arr[act[priority_list[ran]].id1], ran);
}