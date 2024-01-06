#ifndef TREE_TREE_H
#define TREE_TREE_H

#include <stdio.h>

#include "Errors.h"

typedef struct _TreeNode {
    void* data;
    size_t depth;
    struct _TreeNode* left;
    struct _TreeNode* right;
    struct _TreeNode* parent;
} TreeNode;

typedef struct _OutputInfo {
    size_t number_svg_dump;
    size_t number_html_dump;
} OutputInfo;

typedef int (*node_init)    (void** data_dest, void* data_src);
typedef int (*node_destroy) (void* data);
typedef int (*node_verify)  (const void* data);
typedef int (*node_dump_cmd)(const void* data);
typedef int (*node_dump_svg)(const void* data, char** output_buffer);

typedef struct _Tree {
    char* name;
    TreeNode* root;
    node_init node_init;
    node_destroy node_destroy;
    node_verify node_verify;
    node_dump_cmd node_dump_cmd;
    node_dump_svg node_dump_svg;
    OutputInfo output_info;
} Tree;

TreeErrorCode tree_init     (Tree** tree, char* name, node_init init,
                             node_destroy destroy, node_verify verify,
                             node_dump_cmd dump_cmd, node_dump_svg dump_svg);
TreeErrorCode tree_destroy  (Tree* tree);
TreeErrorCode tree_verify   (Tree* tree);
TreeErrorCode tree_dump     (Tree* tree);

TreeNode* tree_node_init    (Tree* tree, void* data,
                             TreeNode* left, TreeNode* right, TreeNode* parent,
                             TreeErrorCode* tree_error);

#endif // TREE_TREE_H