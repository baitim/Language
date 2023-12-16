#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../Math/Math.h"
#include "Output.h"
#include "Tree.h"

enum TypeNodeColor {
    NODE_COLOR_WHITE = 0,
    NODE_COLOR_BLACK = 1,
};

static TreeErrorCode subtree_destroy(Node* node, node_destroy destroy);
static TreeErrorCode subtree_verify (Node* node, node_verify node_verify, int* color, int num);

static TreeErrorCode subtree_insert(Node** node, int x, Node* parent);

TreeErrorCode tree_insert(Tree* tree, int x)
{
    return subtree_insert(&tree->root, x, 0);
}

static TreeErrorCode subtree_insert(Node** node, int x, Node* parent)
{
    TreeErrorCode error = TREE_ERROR_NO;

    if (!(*node)) {
        *node = malloc(sizeof(Node));
        if(!(*node)) return TREE_ERROR_ALLOC_FAIL;

        **node = (Node){NULL, 1, NULL, NULL, parent};

        (*node)->data = malloc(sizeof(int));
        if(!(*node)->data) return TREE_ERROR_ALLOC_FAIL;
        *(int*)(*node)->data = x;
        
        return TREE_ERROR_NO;
    }
    
    if (x <= *(int*)(*node)->data)  error = subtree_insert(&(*node)->left,  x, *node);
    else                            error = subtree_insert(&(*node)->right, x, *node);
    if (error) return error;

    size_t left_depth =  ((*node)->left)  ? (*node)->left->depth  : 0;
    size_t right_depth = ((*node)->right) ? (*node)->right->depth : 0;
    (*node)->depth = MAX(left_depth, right_depth) + 1;

    return TREE_ERROR_NO;
}

TreeErrorCode tree_init(Tree** tree, char* name, node_destroy destroy, 
                        node_verify verify, node_dump_cmd dump_cmd,
                        node_dump_svg dump_svg)
{
    *tree = malloc(sizeof(Tree));
    if(!(*tree)) return TREE_ERROR_ALLOC_FAIL;

    **tree = (Tree){NULL, NULL, destroy, verify, dump_cmd, dump_svg, (OutputInfo){1, 1}};

    (*tree)->name = strdup(name);
    if (!(*tree)->name) return TREE_ERROR_ALLOC_FAIL;

    TreeErrorCode error = prepare_dump_dir(*tree);
    if (error) return error;

    return TREE_ERROR_NO;
}

TreeErrorCode tree_destroy(Tree* tree)
{
    int error = subtree_destroy(tree->root, tree->node_destroy);
    if (error) return error;

    free(tree->name);
    free(tree);
    return TREE_ERROR_NO;
}

static TreeErrorCode subtree_destroy(Node* node, node_destroy destroy)
{
    TreeErrorCode error = TREE_ERROR_NO;

    if (node->left)  error = subtree_destroy(node->left,  destroy);
    if (error) return error;

    if (node->right) error = subtree_destroy(node->right, destroy);
    if (error) return error;

    destroy(node->data);

    free(node);

    return TREE_ERROR_NO;
}

TreeErrorCode tree_verify(Tree* tree)
{
    if (!tree->root) return TREE_ERROR_NO;

    size_t count_nodes = (1 << tree->root->depth);
    int* color = (int*)calloc(count_nodes, sizeof(int));
    if (!color) return TREE_ERROR_ALLOC_FAIL;

    TreeErrorCode error = subtree_verify(tree->root, tree->node_verify, color, 1);
    if (error) return error;

    free(color);
    return TREE_ERROR_NO;
}

static TreeErrorCode subtree_verify(Node* node, node_verify verify, int* color, int num)
{
    if (!node) return TREE_ERROR_NO;

    int error = TREE_ERROR_NO;

    if (color[num] == NODE_COLOR_BLACK) return TREE_ERROR_LOOP;
    color[num] = NODE_COLOR_BLACK;

    size_t next_depth = 0;
    if (node->left)  next_depth = MAX(node->left->depth,  next_depth);
    if (node->right) next_depth = MAX(node->right->depth, next_depth);
    next_depth++;

    if (next_depth != node->depth) return TREE_ERROR_DEPTH;

    if (node->left)  error = subtree_verify(node->left,  verify, color, num * 2);
    if (error) return error;
    if (node->right) error = subtree_verify(node->right, verify, color, num * 2 + 1);
    if (error) return error;

    int node_error = verify(node->data);
    if (node_error) return node_error;

    return TREE_ERROR_NO;
}

TreeErrorCode tree_dump(Tree* tree)
{
    int error = TREE_ERROR_NO;

    error = tree_cmd_dump(tree);
    if (error) return error;

    error = tree_svg_dump(tree);
    if (error) return error;

    error = tree_html_dump(tree);
    if (error) return error;

    return TREE_ERROR_NO;
}
