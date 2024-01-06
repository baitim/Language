#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../Math/Math.h"
#include "Output.h"
#include "Tree.h"

enum TypeTreeNodeColor {
    NODE_COLOR_WHITE = 0,
    NODE_COLOR_BLACK = 1,
};

static TreeErrorCode subtree_destroy(TreeNode* node, node_destroy destroy);
static TreeErrorCode subtree_verify (TreeNode* node, node_verify node_verify, int* color, int num);
static TreeErrorCode node_get_depth (TreeNode* node, size_t* depth);

TreeErrorCode tree_init(Tree** tree, char* name, node_init init,
                        node_destroy destroy, node_verify verify,
                        node_dump_cmd dump_cmd, node_dump_svg dump_svg)
{
    *tree = calloc(1, sizeof(Tree));
    if(!(*tree)) return TREE_ERROR_ALLOC_FAIL;

    **tree = (Tree){NULL, NULL, init, destroy, verify, dump_cmd, dump_svg, (OutputInfo){1, 1}};

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

static TreeErrorCode subtree_destroy(TreeNode* node, node_destroy destroy)
{
    if (!node) return TREE_ERROR_NO;

    TreeErrorCode error = TREE_ERROR_NO;

    if (node->left)  error = subtree_destroy(node->left,  destroy);
    if (error) return error;

    if (node->right) error = subtree_destroy(node->right, destroy);
    if (error) return error;

    int node_error = destroy(node->data);
    if (node_error) return TREE_ERROR_TREE_NODE_DATA;

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

static TreeErrorCode subtree_verify(TreeNode* node, node_verify verify, int* color, int num)
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

TreeNode* tree_node_init(Tree* tree, void* data,
                         TreeNode* left, TreeNode* right, TreeNode* parent,
                         TreeErrorCode* tree_error)
{
    TreeNode* node = calloc(1, sizeof(TreeNode));
    if(!node) { 
        *tree_error = TREE_ERROR_ALLOC_FAIL; 
        return NULL; 
    }

    node->parent =   parent;
    node->left =     left;
    node->right =    right;

    *tree_error = tree->node_init(&node->data, data);
    if (*tree_error) return NULL; 

    for (TreeNode* node_ = node; node_; node_ = node_->parent) {
        *tree_error = node_get_depth(node_, &node_->depth);
        if (*tree_error) return NULL;
    }

    if (node->left)  node->left->parent =  node;
    if (node->right) node->right->parent = node;

    *tree_error = TREE_ERROR_NO;
    return NULL;
}

static TreeErrorCode node_get_depth(TreeNode* node, size_t* depth)
{
    size_t left_depth =  0;
    size_t right_depth = 0;
    if (node->left)  left_depth =  node->left->depth;
    if (node->right) right_depth = node->right->depth;
    (*depth) = MAX(left_depth, right_depth) + 1;

    return TREE_ERROR_NO;
}