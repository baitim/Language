#ifndef TREE_OUTPUT_H
#define TREE_OUTPUT_H

#include "Tree.h"

TreeErrorCode prepare_dump_dir  (Tree* tree);
TreeErrorCode tree_cmd_dump     (Tree* tree);
TreeErrorCode tree_svg_dump     (Tree* tree);
TreeErrorCode tree_html_dump    (Tree* tree);

#endif // TREE_OUTPUT_H