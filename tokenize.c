
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>


#define ADD 1
#define MINUS 2
#define MULTIPLE 3 
#define REMAINDER 4
#define DIVIDE 5
#define OP 10
#define NUM 11
#define ERROR_NUM -10000

typedef  struct token{
	int type;
	int value;
	struct token *next;  
} token;

token *token_head = NULL;
token *token_tail = NULL;
token *t_k;
int change_error = 0;

static token *make_token(int val, int typ){
	t_k = (token *)malloc(sizeof(token));
	t_k->type = typ;
	t_k->value = val;
	t_k->next = NULL;
	return t_k;
}
	

static void add_to_list(token *tk){
	if(!token_head){
		token_head = tk;
		token_tail = tk;
	} else{
		token_tail->next = tk;
		token_tail = token_tail->next;
	}
}

/*
* private function 
* @params: takes a string
* @returns: nothing
* loops through the string and build tokens based on expression gramma
*/

static void  tokenize(char* source){
    char charTokens[] = "+-x/%";
    while(*source != '\0' ){
		if(strchr(charTokens,*source ) != NULL){
            if ( *source == '+'){
				add_to_list(make_token(ADD ,OP));
				source++;
			} else if(*source == '-'){    
				add_to_list(make_token(MINUS,OP)); 
				source++;
			} else if(*source == 'x'){
				add_to_list(make_token(MULTIPLE,OP));
				source++;
			} else if (*source == '/'){
				add_to_list(make_token(DIVIDE, OP));
				source++;
			} else if (*source == '%'){
				add_to_list(make_token(REMAINDER, OP));
				source++;
			}
		} else if (strncasecmp(source , "ADD", 3) == 0){
            add_to_list(make_token(ADD ,OP));
            source = source + 3;
        } else if (strncasecmp(source , "SUBSTRACT", 9) == 0){
			add_to_list(make_token(MINUS ,OP));
			source = source + 9;
        } else if (strncasecmp(source , "DIVIDE", 6) == 0){
			add_to_list(make_token(DIVIDE ,OP));
			source = source + 6;
        } else if (strncasecmp(source , "MULTIPLE", 8) == 0){
            add_to_list(make_token(MULTIPLE ,OP));
            source = source + 8;
        } else if(isspace(*source)){
			source++;
			while(isspace(*source)){ source++;}
		} else if (isdigit(*source)){
			int num = 0;
			num = num + *source - '0';
			source++;
			while(isdigit(*source)){
			    num = num * 10 + *source - '0';
				source++;
			}
			add_to_list(make_token(num, NUM));
		} else if(isalpha(*source)) {
			source++;
			while(isalpha(*source)){ 
				source++;
			}
		} else if (*source == '?'){
			source++;
		} else {
			source++;
		}
		
	}

}

/*
 *Private function
 * @Params :list of tokens
 * @Return : integer
 * 
 * Add 3 to 56?
 * Sum 6 and 6
 * This will only demand a bit of adjustment of solve_expression
*/


static int solve_expression(token **itokens){
	token *tokens = *itokens;
	token *test_tk;
	test_tk = tokens;
	tokens = tokens->next;
	int left;
	if(test_tk->type == 11){
		left = test_tk->value;
		test_tk = tokens;
		tokens = tokens->next;
		switch(test_tk->value){
			case ADD:
				test_tk = tokens;
				return left + test_tk->value;
			case MINUS:
				test_tk = tokens;
				return left - test_tk->value;
			case MULTIPLE:
				test_tk = tokens;
				return left * test_tk->value;
			case REMAINDER:
				test_tk = tokens;
				return left % test_tk->value;
			case DIVIDE:
				test_tk = tokens;
				return left / test_tk->value;
			default:
				change_error = 1;
				return ERROR_NUM;
		}
		
	} else if(test_tk->type == 10){
		int opcode = test_tk->value;
		//left = test_tk->value;
		test_tk = tokens;
		tokens = tokens->next;
		switch(opcode){
			case ADD:
				return test_tk->value + tokens->value;
			case MINUS:
				return test_tk->value - tokens->value;
			case MULTIPLE:
				return test_tk->value * tokens->value;
			case REMAINDER:
				return test_tk->value / tokens->value;
			case DIVIDE:
				return test_tk->value % tokens->value;
			default:
				change_error = 1;
				return ERROR_NUM;
		}
		
		
	}
	
	change_error = 1;		
	return ERROR_NUM;	
		
}


/*
* Params: string
* Examples of acceptle expressions
* Add 3 to 56?
* Sum 6 and 6
* What is 80 x 8?
* Returns integer which is result of expression computation
*/
		

int parse_input(char *statement){
	tokenize(statement);
	token *dd;
	int ans = solve_expression(&token_head);
	dd = token_head;
	while(token_head){
		token_head = token_head->next;
		free(dd);
		dd = token_head;
	}
	
	return ans;
}				
			


