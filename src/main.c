#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#define PARENTHESES 1
#define DIVISION 2
#define UNRECOGNISED 3
#define INVALID 4
#define operationFollowedByOperation i==0||input[i-1]=='+'||input[i-1]=='-'||input[i-1]=='*'||input[i-1]=='/'||input[i-1]=='('||input[i-1]=='^'

#ifdef JITCALC
#include "rawtui.h"
#endif
 
struct actionStruct
{
    int id1;
    int id2;
    char action;
    int priority;
};

uint8_t error;

typedef struct actionStruct action;

double calculate(char *input)
{
	error = 0;
	double* arr = malloc(8);
	action* act = malloc(sizeof(action));
	int operationsQty = 0, nested = 0, maxnested = 0, digitsafterdecimal = 1, hasdecimal = 0, totalcalculations, parsQty = 0;// parsQty - how many pars. were detected, operationsQty - how many actions besides pars. are there
	for (int i = 0;i < strlen(input); ++i)
	{
		switch (input[i])
		{
			case '+':
			{
				if (operationFollowedByOperation)
				{
					error = INVALID;
					return 0.0;
				}

				act[parsQty+operationsQty].action = '+';
				arr[operationsQty]/=digitsafterdecimal;
				digitsafterdecimal = 1;
				hasdecimal = 0;
				act[parsQty+operationsQty].id1 = operationsQty;
				act[parsQty+operationsQty].id2 = operationsQty+1;
				act[parsQty+operationsQty].priority = 1;
				++operationsQty;
				arr = realloc(arr, 8*(operationsQty+1));
				act = realloc(act, sizeof(action)*(parsQty+operationsQty+1));
				arr[operationsQty] = 0;
				break;
			}
			case '-':
			{
				if (operationFollowedByOperation)
				{
					error = INVALID;
					return 0.0;
				}
				act[parsQty+operationsQty].action = '-';
				arr[operationsQty]/=digitsafterdecimal;
				digitsafterdecimal = 1;
				hasdecimal = 0;
				if (i==0||input[i-1]=='(')
				{
					act[parsQty+operationsQty].id1=-2;
					act[parsQty+operationsQty].id2 = operationsQty;
				} 
				else
				{
					act[parsQty+operationsQty].id1 = operationsQty;
					act[parsQty+operationsQty].id2 = operationsQty+1;
				}
				act[parsQty+operationsQty].priority = 1;
				if (act[parsQty+operationsQty].id1!=-2)
				{
					++operationsQty;
					arr = realloc(arr, 8*(operationsQty+1));
					arr[operationsQty] = 0;
				}
				else ++parsQty;
				act = realloc(act, sizeof(action)*(parsQty+operationsQty+1));
				break;
			}
			case '*':
			{
				if (operationFollowedByOperation)
				{
					error = INVALID;
					return 0.0;
				}
				act[parsQty+operationsQty].action = '*';
				arr[operationsQty]/=digitsafterdecimal;
				digitsafterdecimal = 1;
				hasdecimal = 0;
				act[parsQty+operationsQty].id1 = operationsQty;
				act[parsQty+operationsQty].id2 = operationsQty+1;
				act[parsQty+operationsQty].priority = 3;
				++operationsQty;
				arr = realloc(arr, 8*(operationsQty+1));
				act = realloc(act, sizeof(action)*(parsQty+operationsQty+1));
				arr[operationsQty] = 0;
				break;
			}
			case '/':
			{
				if (operationFollowedByOperation)
				{
					error = INVALID;
					return 0.0;
				}
				act[parsQty+operationsQty].action = '/';
				arr[operationsQty]/=digitsafterdecimal;
				digitsafterdecimal = 1;
				hasdecimal = 0;
				act[parsQty+operationsQty].id1 = operationsQty;
				act[parsQty+operationsQty].id2 = operationsQty+1;
				act[parsQty+operationsQty].priority = 3;
				++operationsQty;
				arr = realloc(arr, 8*(operationsQty+1));
				act = realloc(act, sizeof(action)*(parsQty+operationsQty+1));
				arr[operationsQty] = 0;
				break;
			}
			case '(':
			{
				if (arr[operationsQty])
				{
					act = realloc(act, sizeof(action)*(parsQty+operationsQty+2));
					act[parsQty+operationsQty].action = '*';
					act[parsQty+operationsQty].id1 = operationsQty;
					act[parsQty+operationsQty].id2 = operationsQty+1;
					act[parsQty+operationsQty].priority = 3;
					++operationsQty;
					arr = realloc(arr, 8*(operationsQty+1));
					arr[operationsQty] = 0;
				}

				act[parsQty+operationsQty].action = '(';
				act[parsQty+operationsQty].id1 = operationsQty;
				act[parsQty+operationsQty].id2 = -1;
				act[parsQty+operationsQty].priority = 6+nested;
				++nested;
				++parsQty;
				act = realloc(act, sizeof(action)*(parsQty+operationsQty+1));
				break;
			}
			case ')':
			{
				if (operationFollowedByOperation)
				{
					error = INVALID;
					return 0.0;
				}
				act[parsQty+operationsQty].action = ')';
				arr[operationsQty]/=digitsafterdecimal;
				digitsafterdecimal = 1;
				hasdecimal = 0;
				--nested;
				act[parsQty+operationsQty].id1 = -1;
				act[parsQty+operationsQty].id2 = -1;
				act[parsQty+operationsQty].priority = 6+nested;
				++parsQty;
				act = realloc(act, sizeof(action)*(parsQty+operationsQty+1));
				break;
			}
			case '^':
			{
				if (operationFollowedByOperation)
				{
					error = INVALID;
					return 0.0;
				}
				act[parsQty+operationsQty].action = '^';
				arr[operationsQty]/=digitsafterdecimal;
				digitsafterdecimal = 1;
				hasdecimal = 0;
				act[parsQty+operationsQty].id1 = operationsQty;
				act[parsQty+operationsQty].id2 = operationsQty+1;
				act[parsQty+operationsQty].priority = 4;
				++operationsQty;
				arr = realloc(arr, 8*(operationsQty+1));
				act = realloc(act, sizeof(action)*(parsQty+operationsQty+1));
				arr[operationsQty] = 0;
				break;
			}
			case ' ': break;
			case 's':
			{
				act[parsQty+operationsQty].action = 's';
				act[parsQty+operationsQty].id1 = -3;
				act[parsQty+operationsQty].id2 = operationsQty;
				act[parsQty+operationsQty].priority = 5;
				++parsQty;
				act = realloc(act, sizeof(action)*(parsQty+operationsQty+1));
				break;
			}
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
				arr[operationsQty]*=10;
				arr[operationsQty]+=(input[i]-48);
				if (hasdecimal) digitsafterdecimal*=10;
				break;
			}
			case '.': hasdecimal = 1; break;
			default: 
			{
				error = UNRECOGNISED;
				return 0.0;
			}
		}
		if (nested>maxnested) maxnested = nested;
		if (nested<0)
		{
			error = PARENTHESES;
			return 0.0;
		}
	}
	arr[operationsQty]/=digitsafterdecimal;

#ifndef JITCALC
	if (nested)
	{
		error = PARENTHESES;
		return 0.0;
	}
#else
	if (nested>0)
	{
		act = realloc(act, sizeof(action)*(parsQty+operationsQty+nested));
		for (; nested>0; --nested, ++parsQty)
		{
			act[parsQty+operationsQty].id1 = -1;
			act[parsQty+operationsQty].id2 = -1;
			act[parsQty+operationsQty].action = ')';
			act[parsQty+operationsQty].priority = 6+nested;
		}
	}
#endif

	int isinparenthesis = 0, lastaction = 0;
	totalcalculations = parsQty+operationsQty;
	if (!totalcalculations) return arr[0];
	for (int c = maxnested-1;c >= 0; --c)
	{
		for (int i = 0; i <= totalcalculations; ++i) //search for 6+ priority (parentheses' searcher) 
		{
			if (act[i].priority==6+c&&act[i].action=='(')
			{
				for (int x = 1; x < totalcalculations-i;++x) // negative numbers
				{
					if (act[i+x].action==')')
					{
						if (act[i-1-(act[i-1].action=='s')].priority<act[i+x+1].priority) 
						{
							if(i+x+2<totalcalculations)
							{
								act[i+x+1].id1 = act[i].id1<0?act[i].id2:act[i].id1;
							}
						} // if the action after the par. is higher priority

						else
						{
							if (i+x+1<totalcalculations)
							{
								for (int f = i; f>0; --f)
								{
									if (act[f].priority<5)
									{
										act[i+x+1].id1 = act[f].id1<0?act[f].id2:act[f].id1;
										break;
									}
								}
							}
						} // if there's nothing after the parenthesis, skip
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
					} // skip the parentheses with higher/lower priority

					if (act[i+x].id1==-2)
					{
						arr[act[i+x].id2] = 0-arr[act[i+x].id2];
						lastaction = act[i+x].id2;
					} // negative numbers
				}

				for (int x = 1;x < totalcalculations-i;++x) // square root
				{
					if (act[i+x].action==')') break;//if the parenthesis closes
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

				for (int x = 1;x < totalcalculations-i;++x) // exponents
				{
					if (act[i+x].action==')') break;
					//if the parenthesis closes
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
						lastaction = act[i+x].id1;
						for (int f = i+x+1; f < totalcalculations; ++f)
						{
							if (act[f].priority<=4)
							{
								act[f].id1 = act[i+x].id1;
								break;
							}
						}
					}
				}

				for (int x = 1;x < totalcalculations-i;++x) // multiplication / division
				{
					if (act[i+x].action==')') break;
					//if the parenthesis closes
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
						arr[act[i+x].id1] = act[i+x].action=='*'?arr[act[i+x].id1]*arr[act[i+x].id2]:arr[act[i+x].id2]==0?-1, error = DIVISION:arr[act[i+x].id1]/arr[act[i+x].id2];
						lastaction = act[i+x].id1;
						for (int f = i+x+1; f < totalcalculations; ++f)
						{
							if (act[f].action=='(') ++nested;
							else if (act[f].action==')') --nested;
							if ((act[f].priority<=3&&!nested)||(act[f].priority<=4&&nested<0))
							{
								act[f].id1 = act[i+x].id1;
								break;
							}
						} // change index after the parentheses
					}
				}

				for (int x = 1;x < totalcalculations-i;++x) // addition / subtraction
				{
					if (act[i+x].action==')') break;
					//if the parenthesis closes
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
						arr[act[i+x].id1]+=act[i+x].action=='+'?arr[act[i+x].id2]:0-arr[act[i+x].id2];
						lastaction = act[i+x].id1;
						for (int f = i+x+1;f < totalcalculations; ++f)
						{
							if (act[f].action=='(') ++nested;
							else if (act[f].action==')') --nested;
							if ((act[f].priority<=1&&!nested)||(act[f].priority<=4&&nested<0))
							{
								act[f].id1 = act[i+x].id1;
								break;
							}
						}
					}
				}
			}
		}
	}
	if (act[0].id1==-2) 
	{
		arr[0] = 0-arr[0];
		lastaction = 0;
	}

	for (int i = 0; i < totalcalculations; ++i) //search for 5 priority square root calculations outside of parenthesis'
	{
		if (act[i].action=='(') {++isinparenthesis; continue;}
		if (act[i].action==')') {--isinparenthesis; continue;}

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
		if (act[i].action=='(') {++isinparenthesis; continue;}
		if (act[i].action==')') {--isinparenthesis; continue;}
		if (!isinparenthesis)
		{
			if(act[i].priority==4)
			{
				arr[act[i].id1] = pow(arr[act[i].id1], arr[act[i].id2]);
				lastaction = act[i].id1;
				for (int f = i+1;f < totalcalculations; ++f)
				{
					if (act[f].action=='(') ++nested;
					else if (act[f].action==')') --nested;
					if (act[f].priority<=4&&!nested)
					{
						act[f].id1 = act[i].id1;
						break;
					}
				}
			}
		}
	}

	for (int i = 0; i < totalcalculations; ++i) //search for 3 priority multiplication and division outside of parenthesis'
	{
		if (act[i].action=='(') {++isinparenthesis; continue;}
		if (act[i].action==')') {--isinparenthesis; continue;}
		if (!isinparenthesis)
		{
			if(act[i].priority==3)
			{
				arr[act[i].id1] = act[i].action=='*'?arr[act[i].id1]*arr[act[i].id2]:arr[act[i].id2]==0?-1, error = DIVISION:arr[act[i].id1]/arr[act[i].id2];
				lastaction = act[i].id1;
				for (int f = i+1;f < totalcalculations; ++f)
				{
					if (act[f].action=='(') ++nested;
					else if (act[f].action==')') --nested;
					if (act[f].priority<=3&&!nested)
					{
						act[f].id1 = act[i].id1;
						break;
					}
				}
			}
		}
	}

	for (int i = 0; i < totalcalculations; ++i) //search for 1 priority addition and subtraction outside of parenthesis'
	{
		if (act[i].action=='(') {++isinparenthesis; continue;}
		if (act[i].action==')') {--isinparenthesis; continue;}
		if (!isinparenthesis)
		{
			if(act[i].priority==1&&act[i].id1!=-2) 
			{
				arr[act[i].id1] += act[i].action=='+'?arr[act[i].id2]:0-arr[act[i].id2];
				lastaction = act[i].id1;
				for (int f = i+1;f < totalcalculations; ++f)
				{
					if (act[f].action=='(') ++nested;
					else if (act[f].action==')') --nested;
					if (act[f].priority<=1&&!nested)
					{
						act[f].id1 = act[i].id1;
						break;
					}
				}
			}
		}
	}
	double result = arr[lastaction];
	free(arr);
	free(act);
	return result;
}

void pushback(char *string, int index, int stringlen)
{
	for (int i = index;i<stringlen; ++i)
	{
		string[i] = string[i+1];
	}
}

void pushforward(char *string, int index, int stringlen)
{
	for (int i = stringlen;i>index; --i)
	{
		string[i] = string[i-1];
	}
}

int main(int argc, char **argv)
{
	double result;
	if (argc>1)
	{
		for (int i = 1;i<argc; ++i)
		{
			result = calculate(argv[i]);
			switch (error)
			{
				case 0:
				{
					printf("%d: %.10g\n", i, result);
					break;
				}
				case PARENTHESES:
				{
					printf("%d: ERROR: Not all parentheses were closed\n", i);
					break;
				}
				case DIVISION:
				{
					printf("%d: ERROR: Division by 0\n", i);
					break;
				}
				case UNRECOGNISED:
				{
					printf("%d: ERROR: Unrecognised character\n", i);
					break;
				}
				case INVALID:
				{
					printf("ERROR: Two operations in a row\n");
					break;
				}
			}
		}
	}
	else
	{
#ifdef JITCALC
		printf("\n0\n");
		initinline();
		uint16_t x, y;
		getCursorPos(&y, &x);
		--y;
		saveCursorPos();
		move(y-1, 0);
		uint8_t keypressed;
		uint16_t currposition = 0, length = 0;
		char *input = malloc(length+1);
		while ((keypressed = inesc())!=3 && keypressed!=13 && keypressed!=10)
		{
			switch (keypressed)
			{
				case '0'...'9': case '(': case ')': case '+': case '-': case '*': case '/': case '^': case 's': case '.':
				{
					input = realloc(input, ++length+1);
					pushforward(input, currposition, length-1);
					input[currposition++] = keypressed;
					clearline();
					moveprint(y-1, 0, input);
					result = calculate(input);
					move(y, 0);
					clearline();
					switch (error)
					{
						case 0:
						{
							dprintf(STDOUT_FILENO, "%g", result);
							break;
						}
						case DIVISION: 
						{
							print("Division by 0");
							break;
						}
						case INVALID:
						{
							print("Two operations in a row");
							break;
						}
						case PARENTHESES:
						{
							print("More closing parenteses than opening ones");
							break;
						}
					}
					move(y-1, currposition);
					break;
				}
				case 191: // l arrow
				{
					currposition -= currposition>0;
					move(y-1, currposition);
					break;
				}
				case 190: // r arrow
				{
					currposition += currposition<length;
					move(y-1, currposition);
					break;
				}
				case 127: // backspace
				{
					if (length&&currposition)
					{
						pushback(input, --currposition, length);
						input = realloc(input, length--);
						move(y-1, 0);
						clearline();
						print(input);
						move(y, 0);
						clearline();
						result = calculate(input);
						switch (error)
						{
							case 0:
							{
								dprintf(STDOUT_FILENO, "%.10g", result);
								break;
							}
							case DIVISION:
							{
								print("Division by 0");
								break;
							}
							case INVALID:
							{
								print("Two operations in a row");
								break;
							}
							case PARENTHESES:
							{
								print("More closing parenteses than opening ones");
								break;
							}
						}
						move(y-1, currposition);
					}
					break;
				}
				default: break;
			}
		}
		free(input);
		loadCursorPos();
		cleartobot();
		deinit();
#else
		char input[200];
		printf("Input the equations, enter 'q' to exit\n");
		while (1)
		{
			fgets(input, 200, stdin);
			input[strlen(input)-1] = 0;
			if (input[0]=='q') return 0;
			result = calculate(input);
			switch (error)
			{
				case 0:
				{
					printf("%.10g\n", result);
					break;
				}
				case UNRECOGNISED:
				{
					printf("ERROR: Unrecognised character\n");
					break;
				}
				case DIVISION:
				{
					printf("ERROR: Division by 0\n");
					break;
				}
				case INVALID:
				{
					printf("ERROR: Two operations in a row\n");
					break;
				}
				case PARENTHESES:
				{
					printf("ERROR: The quantity of opened and closed parentheses doesn't match\n");
					break;
				}
			}
		}
#endif
	}
}
