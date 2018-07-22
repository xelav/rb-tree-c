#include "rb_tree.h"

Node* initNode(char data[], Node* parent){
	//printf("Node initialized!  %s  %d\n", data, parent);
	Node* self = malloc(sizeof(Node));
	strcpy(self->data , data);
	self->color = RED;
	self->left  = NULL;
	self->right = NULL;
	self->parent = parent;
	return self;
};

RBTree* initTree(){
	RBTree* self = malloc(sizeof(RBTree));
	self->root = NULL;
	printf("New tree initialized!\n");
	return self;
}

void _deleteNode(Node* self, RBTree* tree){
	if(self){
		if (self->parent != NULL){
			if (self->parent->left == self)
				self->parent->left = NULL;
			else 
				self->parent->right = NULL;
		} else
			tree->root = NULL;
		free(self);
	}
}

void _cleanNode(Node* self, RBTree* tree){
	
	if (self == NULL) return;
	_cleanNode(self->left, tree);
	_cleanNode(self->right, tree);
	
	_deleteNode(self, tree);
}

void cleanTree(RBTree* self){
	printf("Cleaning the working tree...\n");
	_cleanNode(self->root, self);
	printf("Clean complete!\n");
}

void deleteTree(RBTree* self){
	printf("Deleting the working tree...\n");
	cleanTree(self);
	free(self);
	printf("Deletion complete!\n");
}

void rotateLeft(RBTree* tree, Node *x) {

    Node *y = x->right;

    x->right = y->left;
    if (y->left != NULL) y->left->parent = x;

    if (y != NULL) y->parent = x->parent;
    if (x->parent) {
        if (x == x->parent->left)
            x->parent->left = y;
        else
            x->parent->right = y;
    } else {
        tree->root = y;
    }

    y->left = x;
    if (x != NULL) x->parent = y;
}

void rotateRight(RBTree* tree, Node *x) {
	
    Node *y = x->left;
    
    if (y == NULL) return;
    
    x->left = y->right;
    if (y->right != NULL) y->right->parent = x;
	printf("a");
    if (y != NULL) y->parent = x->parent;
    printf("b");
    if (x->parent) {
        if (x == x->parent->right)
            x->parent->right = y;
        else
            x->parent->left = y;
    } else {
        tree->root = y;
    }

	printf("d");
    y->right = x;
    if (x != NULL) x->parent = y;
}

Direction getNodeDirection(Node* self){
	if (self->parent)
		return (self == self->parent->right) ? RIGHT : LEFT;
}

void rotateToDirection(RBTree* tree, Node *n, Direction dir){
	(dir == RIGHT) ? rotateRight(tree, n) : rotateLeft(tree, n);
}

Node* getChildByDirection(Node* self, Direction dir){
	return (dir == RIGHT) ? self->right : self->left;
}

Node* getGrandparent(Node* n){
	if ((n != NULL) && (n->parent != NULL))
		return n->parent->parent;
}

Node* getUncle(Node* n){
	Node* g = getGrandparent(n);
	if (g == NULL)
		return; // No grandparent means no uncle
	return (n->parent == g->left) ? g->right : g->left;
}

Node* getSibling(Node* n){
	if (n->parent == NULL)
		return;
	return (n == n->parent->left) ? n->parent->right : n->parent->left;
}

void insertFixup(RBTree* tree, Node* n){
	
	// if new node is the the root just paint it black
	if (n == tree->root){
		n->color= BLACK;
	}
	
	while (n != tree->root && n->parent->color == RED){
		Node* grandparent = getGrandparent(n);
		if (grandparent == NULL){
			return;
		}
		Node* uncle = getUncle(n);
		
		// 0 - parent is a left child 
		// 1 - parent is a right child 
		char parent_direction = getNodeDirection(n->parent);
		if (uncle != NULL && uncle->color == RED){
			n->parent->color = BLACK;
			uncle->color = BLACK;
			grandparent->color = RED;
			n = grandparent;
		} else {
			char node_direction = getNodeDirection(n);
			if (node_direction != parent_direction) {
                n = n->parent;
                rotateToDirection(tree, n, parent_direction);
            }
            // recolor and rotate 
            n->parent->color = BLACK;
            n->parent->parent->color = RED;
            rotateToDirection(tree, n->parent->parent, !parent_direction);
		}
	}
	
	if (tree->root != NULL)
		tree->root->color = BLACK;
	
}

Node* insertValue(RBTree* tree, char* data){
	
	printf("Inserting value:  %s\n", data);
	
	Node* current_node = NULL;
	Node* parent = NULL;
	
	current_node = tree->root;
	
	while(current_node != NULL){
		if (current_node->data == data)
			return current_node;
		parent = current_node;
		current_node = compLT(data, current_node->data) ?
			current_node->left : current_node->right;
	}
	
	Node* n = initNode(data, parent);
	if (parent){
		if (compLT(data, parent->data))
			parent->left = n;
		else 
			parent->right = n;
	} else {
		tree->root = n;
	}
	insertFixup(tree, n);
	return(n);
}

Node* insertValueWithoutFixup(RBTree* tree, char* data){
	Node* current_node = NULL;
	Node* parent = NULL;
	
	current_node = tree->root;
	
	while(current_node != NULL){
		if (current_node->data == data)
			return current_node;
		parent = current_node;
		current_node = compLT(data, current_node->data) ?
			current_node->left : current_node->right;
	}
	
	Node* n = initNode(data, parent);
	if (parent){
		if (compLT(data, parent->data))
			parent->left = n;
		else 
			parent->right = n;
	} else 
		tree->root = n;
	return n;
}

int _countNode(Node* n){
	
	if (n == NULL) return 0;
	
	int c = 1;
	c += _countNode(n->left);
	c += _countNode(n->right);
	
	return c;
	
}
int getElementCount(RBTree* tree){
	return _countNode(tree->root);
}

Node* findElement(RBTree* tree, char* data){
	
	printf("Try to find value:   %s\n",data);
	
	Node* current_node = tree->root;
	char* current_value;
	
	do {
		if (current_node == NULL) {
			printf("ELement %s does not exist\n", data);
			return;
		}
		current_value = current_node->data;
		if (compEQ(current_value, data)){
			printf("Element %s found!\n", data);
			return current_node;
		}
		if (compGT(current_value, data))
			current_node = current_node->left;
		else
			current_node = current_node->right;
	} while (1);
}


void removeFixup(RBTree* tree, Node *n){
	
	// case 1
	if (tree->root == n)
		return;
		
	Node* s = getSibling(n);
	Direction node_dir = getNodeDirection(n);
	
	// case 2
	if (s->color == RED) {
		n->parent->color = RED;
		s->color = BLACK;
		rotateToDirection(tree, n->parent, node_dir);
		s = getSibling(n);
	}
	
	// case 3, 4, 5
	if (s->color == BLACK){
		// case 3, 4
		if ((s->left == NULL || s->left->color == BLACK) &&
	    	(s->right == NULL || s->right->color == BLACK))
		{
			s->color = RED;
			// case 3
			if (n->parent->color == BLACK) 
				removeFixup(tree, n->parent);
			// case 4
			else 
				n->parent->color = BLACK;
		}
		// case 5
		else {
			Node* dir_child = getChildByDirection(s, node_dir);
			Node* not_dir_child = getChildByDirection(s, !node_dir);
			if ((dir_child != NULL && dir_child->color == RED) &&
			   (not_dir_child == NULL || not_dir_child == BLACK)) {
			   		s->color = RED;
			   		dir_child->color = BLACK;
			   		rotateToDirection(tree, s, !node_dir);
			}
			// case 6
			s = getSibling(n);
			s->color = n->parent->color;
			n->parent->color = BLACK;
			not_dir_child = getChildByDirection(s, !node_dir);
			if (not_dir_child != NULL)
				getChildByDirection(s, !node_dir)->color = BLACK;
			rotateToDirection(tree, n->parent, node_dir);
		}
	}
	
}

void  removeNode(RBTree* tree, Node* n){
	
	if (n == NULL) return;
	
	Node* y;
	
	if (n->left == NULL || n->right == NULL) {
        // y has a NIL node as a child 
        y = n;
    } else {
        // find tree successor with a NIL node as a child 
        y = n->right;
        while (y->left != NULL) y = y->left;
    }
    
    
	if (y != n) assign(n->data, y->data);

	Node* child = (y->right == NULL) ? y->left : y->right;

	if (child != NULL) {
		assign(y->data, child->data);
		if (y->color == BLACK) {
			if (child->color == RED)
				child->color = BLACK;
			else
				removeFixup(tree, child);
		}
		_deleteNode(child, tree);
	} else {
		if (y->color == BLACK)
			removeFixup(tree, y);
		_deleteNode(y, tree);
	}

}

void removeValue(RBTree* tree, char* data){
	
	Node* n = findElement(tree, data);
	if (n != NULL){
		removeNode(tree, n);
		printf("Element deleted!\n");
	}
	
}


void _writeNodeToFile(Node* n, FILE* f){
	
	fprintf(f, "%s\n", n->data);
	
	if (n->left)
		_writeNodeToFile(n->left, f);
	if (n->right)
		_writeNodeToFile(n->right, f);
	
	fprintf(f, "!\n");
	
}

void encodeTree(RBTree* tree, char* filename){
	
	printf("Saving tree to the file %s.txt...\n",filename);
	
	//char buf[100];
	//snprintf(buf, sizeof(buf), "%s.txt", filename);
	
	FILE *f = fopen(filename, "w+");
	if (f == NULL)
	{
	    printf("Error opening file!\n");
	    exit(1);
	}
	
	_writeNodeToFile(tree->root, f);
	close(f);
	printf("Tree successfuly saved!\n");
	
}


RBTree* decodeTreeFromFile(char* filename){
	
	printf("Loading tree from the file %s.txt...\n",filename);
	
	char line[255];
	char buf[100];
	snprintf(buf, sizeof(buf), "%s.txt", filename);
	
	FILE *f = fopen(buf, "r");
	if (f == NULL)
	{
	    printf("Error opening file!\n");
	    exit(1);
	}
	
	RBTree* new_tree = initTree();
	Node* current_node = new_tree->root;
	while (fgets (line, 255, f) != NULL) {
				
		if ( compEQ ("!\n",line) ){
			current_node = current_node->parent;
		} else {
			strtok(line, "\n"); // remove trailing newline
			current_node = insertValueWithoutFixup(new_tree, line);
			printf("%s\n", current_node->data);
		}
    }
	printf("Tree successfuly loaded!\n");
	return new_tree;
}

int _print_t(Node *tree, int is_left, int offset, int depth, char s[20][255])
{
    char b[20];
    const int PRINT_WIDTH = 3;
    int width = PRINT_WIDTH + 3;
    int i;

    if (!tree) return 0;

	if (tree->color == RED)
    	sprintf(b, "(% -*.*s)+", PRINT_WIDTH, PRINT_WIDTH, tree->data);
    else if (tree->color == BLACK)
    	sprintf(b, "(% -*.*s)-", PRINT_WIDTH, PRINT_WIDTH, tree->data);

    int left  = _print_t(tree->left,  1, offset,                depth + 1, s);
    int right = _print_t(tree->right, 0, offset + left + width, depth + 1, s);


    for (i = 0; i < width; i++)
        s[2 * depth][offset + left + i] = b[i];

	// printf("\n --%s   ",tree->data);
	// printf(" %d!%d   %d",depth, is_left, offset);
    if (depth && is_left) {
        for (i = 0; i < width + right; i++)
            s[2 * depth - 1][offset + left + width/2 + i] = '-';

        s[2 * depth - 1][offset + left + width/2] = '*';
        s[2 * depth - 1][offset + left + width + right + width/2] = '*';

    } else if (depth && !is_left) {
        for (i = 0; i < left + width; i++)
            s[2 * depth - 1][offset - width/2 + i] = '-';

        s[2 * depth - 1][offset + left + width/2] = '*';
        s[2 * depth - 1][offset - width/2 - 1] = '*';
    }

    return left + width + right;
}

void print_t(Node* tree)
{
    // prints tree ASCII-scheme
	int i;
    char s[20][255];
    for (i = 0; i < 20; i++)
        sprintf(s[i], "%60s", " ");

    _print_t(tree, 0, 0, 0, s);

	printf("\n\n");
    for (i = 0; i < 20; i++)
        printf("%s\n", s[i]);
}

void simple_hlr_print(Node* n){
	
	if (n->color == RED)
		printf(" %s+ ", n->data);
	else
		printf(" %s- ", n->data);
		
	if (n->left)
		simple_hlr_print(n->left);
	if (n->right)
		simple_hlr_print(n->right);
		
}

void printHelp(){
	
	printf("\n1 - initialize new tree\n"
			"2 - load tree from file\n"
			"3 - save tree to file\n"
			"4 - add node\n"
			"5 - remove node\n"
			"6 - clean tree\n"
			"7 - delete tree\n"
			"8 - find node\n"
			"9 - simple print tree\n"
			"10 - print element count\n"
			"11 - print ASCII tree\n"
			"0 - print this help\n"
			"-1 - exit program\n"
			);
}


int main(int argc, char *argv[]) {
	
	int command;
	RBTree* working_tree = NULL;
	/*
	HANDLE  hConsole;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, 12);
	*/
	while(1){
		printf("\nPlease, enter command:\n");
		scanf("%d", &command);
		
		if (command > 2){
			if (working_tree == NULL) {
				printf("No tree has been selected!\n");
				 continue;
			}
		}
		
		// all commands with negative number are for testing
		switch (command){
			case 0:{ // print help
				printHelp();
				break;
			}
			case 1:{ // init new tree
				working_tree = initTree();
				break;
			}
			case 2:{ // load from file
				char filename[MAX_STRING];
				printf("Please, enter name of .txt file:\n");
				scanf("%s", &filename);
				working_tree = decodeTreeFromFile(filename);
				break;
			}
			case 3:{ // save to a file
				char filename[MAX_STRING];
				printf("Please, enter name of .txt file:\n");
				scanf("%s", &filename);
				encodeTree(working_tree, filename);
				break;
			}
			case 4:{ // add node
				char new_node_value[MAX_STRING];
				printf("Enter new node value:\n");
				scanf("%s",new_node_value);
				insertValue(working_tree, new_node_value);
				break;
			}
			case 5:{ // remove node
				char value[MAX_STRING];
				printf("Enter value to delete:\n");
				scanf("%s",value);
				removeValue(working_tree, value);
				break;
			}
			case 6:{ // clean tree
				cleanTree(working_tree);
				break;
			}
			case 7:{ // delete tree
				deleteTree(working_tree);
				working_tree = NULL;
				break;
			}
			case 8:{ // find node
				break;
			}
			case 9:{ // print tree
				simple_hlr_print(working_tree->root);
				break;
			}
			case 10:{
				printf("Count: %d\n", getElementCount(working_tree));
				break;
			}
			case 11:{
				print_t(working_tree->root);
				break;
			}
			case -8:{
                // rotateRight test
				char value[255];
				scanf("%s",value);
				Node* node = findElement(working_tree, value);
				rotateRight(working_tree, node);
				break;
			}
			case -9:{
                // rotateLeft test
				char value[255];
				scanf("%s",value);
				Node* node = findElement(working_tree, value);
				rotateLeft(working_tree, node);
				break;
			}
			case -3:{
				// findElement test
				Node* n1, *n2, *n3, *n4;
	
				RBTree* some_tree = initTree();
	
				n1 = insertValue(some_tree, "dddd");
				insertValue(some_tree, "cccc");
				n2 = insertValue(some_tree, "ssss");
				insertValue(some_tree, "zzzz");
				n3 = insertValue(some_tree, "hhhh");
				n4 = insertValue(some_tree, "ppppp");
				insertValue(some_tree, "ccoo");

				printf("%d  %d\n", n1, findElement(some_tree, "dddd"));
				printf("%d  %d\n", n2, findElement(some_tree, "ssss"));
				printf("%d  %d\n", n3, findElement(some_tree, "hhhh"));
				printf("%d  %d\n", n4, findElement(some_tree, "pppp"));
				break;
			}
			case -4:{
				// insert with fixup test
				RBTree* test_tree = initTree();
				int i;
				char s[2];
				for(i = 0; i < 18; i++){
					sprintf(s, "%c", 'a'+i);
					insertValue(test_tree, s);
					print_t(test_tree->root);
				}
				break;
			}
			case -5:{
				// deletion with fixup test
				RBTree* test_tree = initTree();
				insertValue(test_tree, "h");
				insertValue(test_tree, "b");
				insertValue(test_tree, "zbb");
				insertValue(test_tree, "zcc");
				insertValue(test_tree, "zba");
				insertValue(test_tree, "zaa");
				insertValue(test_tree, "zzz");
				print_t(test_tree->root);
				
				removeValue(test_tree, "zbb");
				print_t(test_tree->root);
				removeValue(test_tree, "zba");
				print_t(test_tree->root);
				removeValue(test_tree, "h");
				print_t(test_tree->root);
				
				
				break;
			}
			case -6:{
				// deletion and count test
				RBTree* test_tree = initTree();
				insertValue(test_tree, "h");
				insertValue(test_tree, "b");
				insertValue(test_tree, "zbb");
				insertValue(test_tree, "zcc");
				insertValue(test_tree, "zba");
				insertValue(test_tree, "zaa");
				insertValue(test_tree, "zzz");
				print_t(test_tree->root);
				
				removeValue(test_tree, "b");
				print_t(test_tree->root);
				
				printf("COUNT: %d", getElementCount(test_tree));
				break;
			}
			case -7:{
				RBTree* test_tree = initTree();
				insertValue(test_tree, "h");
				insertValue(test_tree, "b");
				insertValue(test_tree, "zbb");
				insertValue(test_tree, "zcc");
				insertValue(test_tree, "zba");
				insertValue(test_tree, "zaa");
				insertValue(test_tree, "zzz");
				
				encodeTree(test_tree, "newtest");
				break;
			}
			case -1:
				exit(1);
				break;
			default:
				printf("Incorrect command. Enter 0 for help.\n");
		}
	}
	
	return 0;
}
