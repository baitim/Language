#ifndef TREE_TREE_H
#define TREE_TREE_H

#include "Errors.h"

typedef struct _Node {
    void* data;
    size_t depth;
    struct _Node* left;
    struct _Node* right;
    struct _Node* parent;
} Node;

typedef struct _OutputInfo {
    size_t number_svg_dump;
    size_t number_html_dump;
} OutputInfo;

typedef void (*node_destroy) (void* data);
typedef int  (*node_verify)  (const void* data);
typedef void (*node_dump_cmd)(const void* data);
typedef void (*node_dump_svg)(const void* data, char** output_buffer);

typedef struct _Tree {
    char* name;
    Node* root;
    node_destroy node_destroy;
    node_verify node_verify;
    node_dump_cmd node_dump_cmd;
    node_dump_svg node_dump_svg;
    OutputInfo output_info;
} Tree;

TreeErrorCode tree_init     (Tree** tree, char* name, node_destroy destroy, 
                             node_verify verify, node_dump_cmd dump_cmd,
                             node_dump_svg dump_svg);
TreeErrorCode tree_destroy  (Tree* tree);
TreeErrorCode tree_verify   (Tree* tree);
TreeErrorCode tree_dump     (Tree* tree);

TreeErrorCode tree_insert   (Tree* tree, int x);

#endif // TREE_TREE_H