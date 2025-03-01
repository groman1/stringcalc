#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
 
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
    if (argc>1) {strcpy(input, argv[1]);input[strlen(input)]='\n'; input[strlen(input)+1]='\0';}
    else {fgets(input, 200, stdin);}
    double* arr = malloc(8);
    action* act = malloc(sizeof(act));
    int currindex_num = 0, nested = 0, maxnested = 0, digitsafterdecimal = 1, hasdecimal = 0, totalcalculations = 0, ran = 0;
    for (int i = 0; i < strlen(input)-1; ++i) //parser, ignoring the \n and \0
    {
        switch (input[i])
        {
            
            case '+': {act[ran].action = '+'; arr[currindex_num]/=digitsafterdecimal; digitsafterdecimal = 1; hasdecimal = 0; act[ran].id1 = currindex_num; act[ran].id2 = currindex_num+1; act[ran].priority = 1; ++ran; ++currindex_num; arr=realloc(arr, 8*(currindex_num+1)); act=realloc(act, sizeof(action)*(ran+1)); arr[currindex_num] = 0; break;}
            case '-': {act[ran].action = '-'; arr[currindex_num]/=digitsafterdecimal; digitsafterdecimal = 1; hasdecimal = 0; act[ran].id1 = i==0||input[i-1]=='('?-2:currindex_num; act[ran].id2 = currindex_num+1; act[ran].priority = 1; if (act[ran].id1!=-2) {++currindex_num; arr=realloc(arr, 8*(currindex_num+1)); arr[currindex_num] = 0;} ++ran; act=realloc(act, sizeof(action)*(ran+1)); break;}
            case '*': {act[ran].action = '*'; arr[currindex_num]/=digitsafterdecimal; digitsafterdecimal = 1; hasdecimal = 0; act[ran].id1 = currindex_num; act[ran].id2 = currindex_num+1; act[ran].priority = 3; ++ran; ++currindex_num; arr=realloc(arr, 8*(currindex_num+1)); act=realloc(act, sizeof(action)*(ran+1)); arr[currindex_num] = 0; break;}
            case '/': {act[ran].action = '/'; arr[currindex_num]/=digitsafterdecimal; digitsafterdecimal = 1; hasdecimal = 0; act[ran].id1 = currindex_num; act[ran].id2 = currindex_num+1; act[ran].priority = 3; ++ran; ++currindex_num; arr=realloc(arr, 8*(currindex_num+1)); act=realloc(act, sizeof(action)*(ran+1)); arr[currindex_num] = 0; break;}
            case '(': {act[ran].action = '('; arr[currindex_num]/=digitsafterdecimal; digitsafterdecimal = 1; hasdecimal = 0; act[ran].id1 = currindex_num; act[ran].id2 = -1; act[ran].priority = 6+nested; ++nested; ++ran; act=realloc(act, sizeof(action)*(ran+1)); arr=realloc(arr, 8*(currindex_num+1)); break;}
            case ')': {act[ran].action = ')'; arr[currindex_num]/=digitsafterdecimal; digitsafterdecimal = 1; hasdecimal = 0; --nested; act[ran].id1 = -1; act[ran].id2 = -1; act[ran].priority = 6+nested; ++ran; act=realloc(act, sizeof(action)*(ran+1)); arr=realloc(arr, 8*(currindex_num+1)); break;}
            case '^': {act[ran].action = '^'; arr[currindex_num]/=digitsafterdecimal; digitsafterdecimal = 1; hasdecimal = 0; act[ran].id1 = currindex_num; act[ran].id2 = currindex_num+1; act[ran].priority = 5; ++ran; ++currindex_num; arr=realloc(arr, 8*(currindex_num+1)); act=realloc(act, sizeof(action)*(ran+1)); arr[currindex_num] = 0; break;}
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
            default: printf("unrecognised characted\n"); return 1;
        }
        if(nested>maxnested) maxnested = nested;
    }
    arr[currindex_num]/=digitsafterdecimal;

    if (nested)
    {
        printf("not all parentesis were closed\n");
        return 1;
    }

    int *priority_list = malloc(currindex_num*4);
    int currpriority = 0, isinparenthesis = 0;

    for (int c = maxnested-1; c >= 0; --c)
    {
        for (int i = 0; i <= ran; ++i) //search for 5+ priority (parentesis' searcher) 
        {
            if(act[i].priority==6+c&&act[i].action=='(') 
            {
                for (int x = 1;x < ran-i;++x) // negative numbers
                {
                    
                    if (act[i+x].action==')') 
                    { act[i+x+1].id1 = act[i].id1; break;} //if the parenthesis closes, the act manipulation is needed to assign id1 to the action after the par.
                    if (act[i+x].action=='(')
                    {
                        ++nested;
                        while (nested>0)
                        {
                            ++x;
                            printf("%d d\n", x);
                            if (act[i+x].action==')') --nested;
                            else if (act[i+x].action=='(') ++nested;
                        }
                    }

                    if (act[i+x].id1==-2)
                    {
                        priority_list[currpriority++] = i+x;
                    }
                }

                for (int x = 1;x < ran-i;++x) // exponents
                {
                    if (act[i+x].action==')') 
                    { break; } //if the parenthesis closes
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

                    if (act[i+x].priority==5)
                    {
                        priority_list[currpriority++] = i+x;
                    }
                }

                for (int x = 1;x < ran-i;++x) // multiplication / division
                {
                    if (act[i+x].action==')') 
                    {break;} //if the parenthesis closes
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
                        priority_list[currpriority++] = i+x;
                    }
                }

                for (int x = 1;x < ran-i;++x) // addition / subtraction
                {
                    if (act[i+x].action==')') 
                    {break;} //if the parenthesis closes
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

                    if (act[i+x].priority==1&&act[i+x].id1!=-2)
                    {
                        priority_list[currpriority++] = i+x;
                    }
                }
            }
        }
    }

    for (int i = 0; i < ran; ++i) //search for 5 priority exponentation outside of parenthesis'
    {
        if (act[i].action=='(') {isinparenthesis = 1; continue;}
        if (act[i].action==')') {isinparenthesis = 0; continue;}
        
        if (!isinparenthesis)
        {
            if(act[i].priority==5) 
            {priority_list[currpriority++] = i;}
        }
    }

    for (int i = 0; i < currindex_num; ++i) //search for 3 priority multiplication and division outside of parenthesis'
    {
        if (act[i].action=='(') {isinparenthesis = 1; continue;}
        if (act[i].action==')') {isinparenthesis = 0; continue;}
        
        if (!isinparenthesis)
        {
            if(act[i].priority==3)
            {priority_list[currpriority++] = i;}
        }
    }

    for (int i = 0; i < ran; ++i) //search for 1 priority addition and subtraction outside of parenthesis'
    {
        if (act[i].action=='(') {isinparenthesis = 1; continue;}
        if (act[i].action==')') {isinparenthesis = 0; continue;}
        if (!isinparenthesis)
        {
            if(act[i].priority==1) 
            {priority_list[currpriority++] = i;}
        }
    }

    for (int i = 0; i < currindex_num; ++i) // calculation
    {
        if(ran)
        {
            switch (act[priority_list[i]].action)
            {           
                case '+': arr[act[priority_list[i]].id1]+=arr[act[priority_list[i]].id2]; break;
                case '-': act[priority_list[i]].id1==-2?(arr[act[priority_list[i]].id1]=0-arr[act[priority_list[i]].id2, ++currindex_num]):(arr[act[priority_list[i]].id1]-=arr[act[priority_list[i]].id2]); break;
                case '*': arr[act[priority_list[i]].id1]*=arr[act[priority_list[i]].id2]; break;
                case '/': if (arr[act[priority_list[i]].id2]==0) {printf("division by 0"); return 1;} arr[act[priority_list[i]].id1]/=arr[act[priority_list[i]].id2]; break;
                case '^': arr[act[priority_list[i]].id1] = pow(arr[act[priority_list[i]].id1], arr[act[priority_list[i]].id2]); break;
                default: printf("unpredicted behaviour\n"); return 1;  // this shouldn't be possible, in the perfect world
            }
        }
        else
        {
            printf("%lf", arr[0]);
        }
    }
    printf("%.*g\n", arr[act[priority_list[currindex_num-1]].id1]);
}