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
    int currindex_num = 0, nested = 0, maxnested = 0, digitsafterdecimal = 1, hasdecimal = 0, totalcalculations = 0, ran = 0; // ran - how many pars. were detected, currindex_num - how many actions besides pars. are there
    for (int i = 0; i < strlen(input)-1; ++i) //parser, ignoring the \n and \0
    {
        switch (input[i])
        {
            
            case '+': {act[ran+currindex_num].action = '+'; arr[currindex_num]/=digitsafterdecimal; digitsafterdecimal = 1; hasdecimal = 0; act[ran+currindex_num].id1 = currindex_num; act[ran+currindex_num].id2 = currindex_num+1; act[ran+currindex_num].priority = 1; ++currindex_num; arr=realloc(arr, 8*(currindex_num+1)); act=realloc(act, sizeof(action)*(ran+currindex_num+1)); arr[currindex_num] = 0; break;}
            case '-': {act[ran+currindex_num].action = '-'; arr[currindex_num]/=digitsafterdecimal; digitsafterdecimal = 1; hasdecimal = 0; if(i==0||input[i-1]=='('){ act[ran+currindex_num].id1=-2;act[ran+currindex_num].id2 = currindex_num; } else {act[ran+currindex_num].id1 = currindex_num; act[ran+currindex_num].id2 = currindex_num+1;} act[ran+currindex_num].priority = 1; if (act[ran+currindex_num].id1!=-2) { ++currindex_num; arr=realloc(arr, 8*(currindex_num+1)); arr[currindex_num] = 0; } act=realloc(act, sizeof(action)*(ran+currindex_num+1)); break;}
            case '*': {act[ran+currindex_num].action = '*'; arr[currindex_num]/=digitsafterdecimal; digitsafterdecimal = 1; hasdecimal = 0; act[ran+currindex_num].id1 = currindex_num; act[ran+currindex_num].id2 = currindex_num+1; act[ran+currindex_num].priority = 3; ++currindex_num; arr=realloc(arr, 8*(currindex_num+1)); act=realloc(act, sizeof(action)*(ran+currindex_num+1)); arr[currindex_num] = 0; break;}
            case '/': {act[ran+currindex_num].action = '/'; arr[currindex_num]/=digitsafterdecimal; digitsafterdecimal = 1; hasdecimal = 0; act[ran+currindex_num].id1 = currindex_num; act[ran+currindex_num].id2 = currindex_num+1; act[ran+currindex_num].priority = 3; ++currindex_num; arr=realloc(arr, 8*(currindex_num+1)); act=realloc(act, sizeof(action)*(ran+currindex_num+1)); arr[currindex_num] = 0; break;}
            case '(': {act[ran+currindex_num].action = '('; arr[currindex_num]/=digitsafterdecimal; digitsafterdecimal = 1; hasdecimal = 0; act[ran+currindex_num].id1 = currindex_num; act[ran+currindex_num].id2 = -1; act[ran+currindex_num].priority = 6+nested; ++nested; ++ran; act=realloc(act, sizeof(action)*(ran+currindex_num+1)); break;}
            case ')': {act[ran+currindex_num].action = ')'; arr[currindex_num]/=digitsafterdecimal; digitsafterdecimal = 1; hasdecimal = 0; --nested; act[ran+currindex_num].id1 = -1; act[ran+currindex_num].id2 = -1; act[ran+currindex_num].priority = 6+nested; ++ran; act=realloc(act, sizeof(action)*(ran+currindex_num+1)); break;}
            case '^': {act[ran+currindex_num].action = '^'; arr[currindex_num]/=digitsafterdecimal; digitsafterdecimal = 1; hasdecimal = 0; act[ran+currindex_num].id1 = currindex_num; act[ran+currindex_num].id2 = currindex_num+1; act[ran+currindex_num].priority = 4; ++currindex_num; arr=realloc(arr, 8*(currindex_num+1)); act=realloc(act, sizeof(action)*(ran+currindex_num+1)); arr[currindex_num] = 0; break;}
            case ' ': break;
            case 's': {act[ran+currindex_num].action = 's'; act[ran+currindex_num].id1 = -3; act[ran+currindex_num].id2 = currindex_num; act[ran+currindex_num].priority = 5; /*++sqrtused;*/ ++ran; act=realloc(act, sizeof(action)*(ran+currindex_num+1)); break;}
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
            default: printf("unrecognised character\n"); return 1;
        }
        if(nested>maxnested) maxnested = nested;
    }
    arr[currindex_num]/=digitsafterdecimal;

    if (nested)
    {
        printf("not all parentesis were closed\n");
        return 1;
    }

    int isinparenthesis = 0, error = 0, lastaction = 0;
    totalcalculations = ran+currindex_num;

    for (int c = maxnested-1; c >= 0; --c)
    {
        for (int i = 0; i <= totalcalculations; ++i) //search for 5+ priority (parentesis' searcher) 
        {
            if(act[i].priority==6+c&&act[i].action=='(') 
            {
                for (int x = 1;x < totalcalculations-i;++x) // negative numbers
                {
                    if (act[i+x].action==')')
                    {
                        if (act[i-1-(act[i-1].action=='s')].priority<act[i+x+1].priority) { act[i+x+1].id1 = act[i].id1; } // if the action after the par. is higher priority
                        else { if(i+x+2<totalcalculations) {act[i+x+1].id1 = act[i-1].id1;} } // if there's nothing after the parenthesis, skip
                        break;
                    } //if the parenthesis closes

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

                    if (act[i+x].id1==-2)
                    {
                        arr[act[i+x].id2] = -arr[act[i+x].id2];
                        lastaction = act[i+x].id1;
                    }
                }

                for (int x = 1;x < totalcalculations-i;++x) // exponents
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

                    if (act[i+x].priority==4)
                    {
                        arr[act[i+x].id1] = pow(arr[act[i+x].id1], arr[act[i+x].id2]);
                        if (act[i+x+1].priority<4)
                        {
                            act[i+x+1].id1 = act[i+x].id1;
                            lastaction = act[i+x].id1;
                        }
                    }
                }

                for (int x = 1;x < totalcalculations-i;++x) // multiplication / division
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
                        arr[act[i+x].id1] = act[i+x].action=='*'?arr[act[i+x].id1]*arr[act[i+x].id2]:arr[act[i+x].id2]==0?-1, error = 1:arr[act[i+x].id1]/arr[act[i+x].id2];
                        if (act[i+x+1].priority<3)
                        {
                            act[i+x+1].id1 = act[i+x].id1;
                            lastaction = act[i+x].id1;
                        }
                    }
                }

                for (int x = 1;x < totalcalculations-i;++x) // addition / subtraction
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
                        arr[act[i+x].id1]+=act[i+x].action=='+'?arr[act[i+x].id2]:-arr[act[i+x].id2];
                        lastaction = act[i+x].id1;
                    }
                }

                for (int x = 1;x < totalcalculations-i;++x) // square root
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

                    if (act[i+x].priority==5)
                    {
                        arr[act[i+x].id2] = sqrt(arr[act[i+x].id2]);
                        lastaction = act[i+x].id2;
                    }
                }
            }
        }
    }

    if (act[0].id1==-2) 
    {
        arr[0] = -arr[0];
        lastaction = 0;
    }

    for (int i = 0; i < totalcalculations; ++i) //search for 5 priority square root calculations outside of parenthesis'
    {
        if (act[i].action=='(') {isinparenthesis = 1; continue;}
        if (act[i].action==')') {isinparenthesis = 0; continue;}

        if (!isinparenthesis)
        {
            if (act[i].action=='s')
            {
                arr[act[i].id2] = sqrt(arr[act[i].id2]);
                lastaction = act[i].id2;
            }
        }
    }

    for (int i = 0; i < totalcalculations; ++i) //search for 4 priority exponentation outside of parenthesis'
    {
        if (act[i].action=='(') {isinparenthesis = 1; continue;}
        if (act[i].action==')') {isinparenthesis = 0; continue;}
        if (!isinparenthesis)
        {
            if(act[i].priority==4)
            { arr[act[i].id1] = pow(arr[act[i].id1], arr[act[i].id2]); lastaction = act[i].id1; }
            if (act[i+1].priority<4)
            {
                act[i+1].id1 = act[i].id1<0?act[i].id2:act[i].id1;
            }
        }
    }

    for (int i = 0; i < totalcalculations; ++i) //search for 3 priority multiplication and division outside of parenthesis'
    {
        if (act[i].action=='(') {isinparenthesis = 1; continue;}
        if (act[i].action==')') {isinparenthesis = 0; continue;}
        if (!isinparenthesis)
        {
            if(act[i].priority==3)
            { arr[act[i].id1] = act[i].action=='*'?arr[act[i].id1]*arr[act[i].id2]:arr[act[i].id2]==0?-1, error = 1:arr[act[i].id1]/arr[act[i].id2]; lastaction = act[i].id1; }
            if (act[i+1].priority<3)
            {
                act[i+1].id1 = act[i].id1<0?act[i].id2:act[i].id1;
            }
        }
    }

    for (int i = 0; i < totalcalculations; ++i) //search for 1 priority addition and subtraction outside of parenthesis'
    {
        if (act[i].action=='(') {isinparenthesis = 1; continue;}
        if (act[i].action==')') {isinparenthesis = 0; continue;}
        if (!isinparenthesis)
        {
            if(act[i].priority==1&&act[i].id1!=-2) 
            { arr[act[i].id1] += act[i].action=='+'?arr[act[i].id2]:-arr[act[i].id2]; lastaction = act[i].id1; }
        }
    }

    if (error)
    {
        printf("division by 0");
        return 1;
    }

    printf("%.10g\n", arr[lastaction]); //incase the action is 
}