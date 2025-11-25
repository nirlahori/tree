
typedef struct _avl_node avl_node;

struct _avl_node{
    int key;
    avl_node* left;
    avl_node* right;
    avl_node* parent;
    int height;
};

typedef struct _avl_tree avl_tree;

struct _avl_tree{
    avl_node* root;
};


void insert(avl_tree*, avl_node*);
int height(avl_tree*);

avl_node* avl_minimum(avl_node*);
avl_node* avl_successor(avl_node*);


