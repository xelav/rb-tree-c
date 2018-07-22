#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define _GNU_SOURCE
#include <errno.h>
#include <windows.h>

#define compLT(a,b) (strcmp(a, b) < 0)
#define compGT(a,b) (strcmp(a, b) > 0)
#define compEQ(a,b) (strcmp(a, b) == 0)
#define assign(a,b) (strcpy(a,b))

// ANSI colors
#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"

#define MAX_STRING 255

struct Node;
struct RBTree;

typedef enum { BLACK, RED } nodeColor;
typedef enum {
	LEFT = 0,
	RIGHT = 1
	} Direction;

typedef struct Node{
	
	struct Node* parent;
	struct Node* left;
	struct Node* right;
	
	nodeColor color;
	char data[];
} Node;

typedef struct RBTree{
	
	Node* root;
	int count;
	
} RBTree;

// function declarations

RBTree* initTree();
Node* initNode();

void simple_hlr_print(Node* n);
void print_t(Node* tree);


