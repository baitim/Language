#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "../ANSI_colors.h"
#include "Output.h"

#define MAX_SIZE_NAME_DUMP      100
#define MAX_SIZE_COMMAND        500
#define MAX_SIZE_OUTPUT_BUFFER  100000

static TreeErrorCode tree_cmd_dump_             (Node* node, node_dump_cmd dump_cmd, int dep);
static TreeErrorCode tree_svg_dump_make_node    (Node* node, node_dump_svg dump_svg, char** output_buffer);
static TreeErrorCode tree_svg_dump_make_edge    (Node* node, char** output_buffer);
static TreeErrorCode str_attach                 (char* str1, const char *str2, char** new_str);

TreeErrorCode tree_cmd_dump(Tree* tree)
{
    TreeErrorCode err = tree_verify(tree);
    if (err) return err;

    fprintf(stderr, print_lmagenta("^^^^^^^^^^^^^^^^^^^^\n\n"));
    fprintf(stderr, print_lmagenta("Tree:\n"));

    tree_cmd_dump_(tree->root, tree->node_dump_cmd, 0);

    fprintf(stderr, print_lmagenta("vvvvvvvvvvvvvvvvvvvv\n\n"));

    return tree_verify(tree);
}

static TreeErrorCode tree_cmd_dump_(Node* node, node_dump_cmd dump_cmd, int dep)
{
    if (node->left) tree_cmd_dump_(node->left, dump_cmd, dep + 1);

    for (int i = 0; i < dep; i++) fprintf(stderr, "\t");
    dump_cmd(node->data);
    fprintf(stderr, "\n");

    if (node->right) tree_cmd_dump_(node->right, dump_cmd, dep + 1);

    return TREE_ERROR_NO;
}

TreeErrorCode prepare_dump_dir(Tree* tree)
{
    char command[MAX_SIZE_COMMAND] = "";
    snprintf(command, MAX_SIZE_COMMAND, "mkdir dump/%s", tree->name);
    int sys = system(command);

    snprintf(command, MAX_SIZE_COMMAND, "rm -r dump/%s/*", tree->name);
    sys = system(command);

    snprintf(command, MAX_SIZE_COMMAND, "mkdir dump/%s/dot ; "
                                        "mkdir dump/%s/svg ; "
                                        "mkdir dump/%s/html ;",
                                        tree->name, tree->name, tree->name);

    sys = system(command);
    if (sys) return TREE_ERROR_SYSTEM_COMMAND;

    return TREE_ERROR_NO;
}

TreeErrorCode tree_svg_dump(Tree* tree)
{
    TreeErrorCode err = tree_verify(tree);
    if (err) return err;

    char dot_path[MAX_SIZE_NAME_DUMP] = "";
    snprintf(dot_path, MAX_SIZE_NAME_DUMP, "dump/%s/dot/%s%zu", tree->name, tree->name, 
                                            tree->output_info.number_svg_dump);

    char *name_dot_file = NULL;
    err = str_attach(dot_path, ".dot", &name_dot_file);
    if (err) return err;
    
    FILE* dot_file = fopen(name_dot_file, "w");
    if (!dot_file) return TREE_ERROR_OPEN_FILE;

    char* output_buffer = calloc(MAX_SIZE_OUTPUT_BUFFER, sizeof(char));
    if (!output_buffer) return TREE_ERROR_ALLOC_FAIL;
    char* old_pointer_buffer = output_buffer;

    output_buffer += sprintf(output_buffer, "digraph {\n"
                        "\tgraph[label = \"%s\", labelloc = top, "
                        "labeljust = center, fontsize = 70, fontcolor = \"#e33e19\"];\n"
                        "\tgraph[dpi = 100];\n"
                        "\tbgcolor = \"#2F353B\";\n"
                        "\tedge[minlen = 3.5, arrowsize = 2.5, penwidth = 4];\n"
                        "\tnode[shape = \"rectangle\", style = \"rounded, filled\", height = 3, width = 2, "
                        "fillcolor = \"#ab5b0f\", width = 3, fontsize = 30, penwidth = 3.5, color = \"#941b1b\"]\n",
                        tree->name);

    err = tree_svg_dump_make_node(tree->root, tree->node_dump_svg, &output_buffer);
    if (err) return err;

    err = tree_svg_dump_make_edge(tree->root, &output_buffer);
    if (err) return err;

    output_buffer += sprintf(output_buffer, "}\n");

    fprintf(dot_file, "%s", old_pointer_buffer);
    free(old_pointer_buffer);
    fclose(dot_file);

    char svg_path[MAX_SIZE_NAME_DUMP] = "";
    snprintf(svg_path, MAX_SIZE_NAME_DUMP, "dump/%s/svg/%s%zu", tree->name, tree->name, 
                                            tree->output_info.number_svg_dump);

    char *name_svg_file = NULL;
    err = str_attach(svg_path, ".svg", &name_svg_file);
    if (err) return err;

    char command[MAX_SIZE_COMMAND] = "";
    snprintf(command, MAX_SIZE_COMMAND, "gvpack -u %s | dot -Tsvg -o %s", 
                                         name_dot_file, name_svg_file);
    int sys = system(command);
    if (sys) return TREE_ERROR_SYSTEM_COMMAND;

    tree->output_info.number_svg_dump++;
    free(name_dot_file);
    free(name_svg_file);
    return tree_verify(tree);
}

static TreeErrorCode tree_svg_dump_make_node(Node* node, node_dump_svg dump_svg, char** output_buffer)
{
    if (node->left) tree_svg_dump_make_node(node->left, dump_svg, output_buffer);

    *output_buffer += sprintf(*output_buffer, "\t{ \n"
                                            "\t\tnode[shape = \"Mrecord\"];\n"
                                            "\t\tnode%p[label = \"{ ", node);

    dump_svg(node->data, output_buffer);

    *output_buffer += sprintf(*output_buffer, "\t}\n");

    if (node->right) tree_svg_dump_make_node(node->right, dump_svg, output_buffer);

    return TREE_ERROR_NO;
}

static TreeErrorCode tree_svg_dump_make_edge(Node* node, char** output_buffer)
{
    if (node->left)  tree_svg_dump_make_edge(node->left, output_buffer);

    if (node->left)  
        *output_buffer += sprintf(*output_buffer, "\tnode%p->node%p[color = yellow, labelangle = 45];\n", node, node->left);
    if (node->right) 
        *output_buffer += sprintf(*output_buffer, "\tnode%p->node%p[color = yellow, labelangle = 45];\n", node, node->right);
    
    if (node->right) tree_svg_dump_make_edge(node->right, output_buffer);

    if (node->parent) 
        *output_buffer += sprintf(*output_buffer, "\tnode%p->node%p[color = \"#b6ff1a\", labelangle = 45];\n", node, node->parent);

    return TREE_ERROR_NO;
}

TreeErrorCode tree_html_dump(Tree* tree)
{
    TreeErrorCode err = tree_verify(tree);
    if (err) return err;

    char buffer[MAX_SIZE_NAME_DUMP] = "";
    snprintf(buffer, MAX_SIZE_NAME_DUMP, "dump/%s/html/%s%zu.html", tree->name, tree->name, 
                                          tree->output_info.number_html_dump);

    FILE* html_file = fopen(buffer, "w");
    if (!html_file) return TREE_ERROR_OPEN_FILE;

    char* output_buffer = calloc(MAX_SIZE_OUTPUT_BUFFER, sizeof(char));
    if (!output_buffer) return TREE_ERROR_ALLOC_FAIL;
    char* old_pointer_buffer = output_buffer;

    output_buffer += sprintf(output_buffer, "<pre>\n");

    for (size_t i = 1; i < tree->output_info.number_svg_dump; i++) {
        output_buffer += sprintf(output_buffer, "<img src = \"../svg/%s%zu.svg\">\n", tree->name, i);
    }

    output_buffer += sprintf(output_buffer, "</pre>\n");

    fprintf(html_file, "%s", old_pointer_buffer);
    free(old_pointer_buffer);
    fclose(html_file);

    tree->output_info.number_html_dump++;

    return tree_verify(tree);
}

static TreeErrorCode str_attach(char* str1, const char *str2, char** new_str)
{
    size_t len_str1 = strlen(str1);
    size_t len_str2 = strlen(str2);
    size_t size_new_str = len_str1 + len_str2 + 1;
    *new_str = (char*)calloc(size_new_str, sizeof(char));
    if (!(*new_str)) return TREE_ERROR_ALLOC_FAIL;
    (*new_str) = (char*)memcpy((*new_str), str1, len_str1 * sizeof(char) + 1);
    if (!(*new_str)) return TREE_ERROR_ALLOC_FAIL;
    (*new_str) = strcat((*new_str), str2);
    if (!(*new_str)) return TREE_ERROR_ALLOC_FAIL;

    return TREE_ERROR_NO;
}