#include <stdio.h>

#include "../ANSI_colors.h"
#include "../Errors/Errors.h"
#include "../Frontend/ProcessCmd.h"
#include "../Frontend/Tree.h"
#include "Input.h"
#include "Output.h"
#include "../ProcessCmd/ProcessCmd.h"
#include "../Tree/Tree.h"

int main(int argc, const char *argv[])
{
    printf(print_lblue("# Implementation of language front-end.\n"
                       "# (c) BAIDUSENOV TIMUR, 2023\n\n"));

    TreeErrorCode tree_error = TREE_ERROR_NO;
    CmdErrorCode  cmd_error =  CMD_ERROR_NO;
    FrontendErrorCode  frontend_error =  FRONTEND_ERROR_NO;
    Tree* tree = NULL;
    FrontendCmdInputData cmd_data = {};
    FrontendInputData* input_data = NULL;

    cmd_error = input_cmd(argc, argv, &cmd_data, TYPE_CMD_OPTIONS_FRONTEND);
    if (cmd_error) goto cmd_error;

    if (cmd_data.is_help) {
        cmd_error = print_help();
        if (cmd_error) goto cmd_error;
    }

    cmd_error = cmd_data_verify(&cmd_data);
    if (cmd_error) goto cmd_error;

    /////////////////////////////////////////////////////////

    frontend_error = input_data_init(&input_data, cmd_data.name_data_file);
    if (frontend_error) goto frontend_error;

    frontend_error = input_data_dump(input_data);
    if (frontend_error) goto frontend_error;

    /////////////////////////////////////////////////////////

    tree_error = tree_init(&tree, "frontend_tree", data_destroy, 
                                                   data_verify, data_dump_cmd, data_dump_svg);
    if (tree_error) goto tree_error;

    frontend_error = tree_input(tree);
    if (frontend_error) goto frontend_error;

    tree_error = tree_dump(tree);
    if (tree_error) goto tree_error;

    /////////////////////////////////////////////////////////

    goto finally;

cmd_error:
    err_dump(cmd_error, ERROR_CODE_CMD);
    goto finally;

frontend_error:
    err_dump(frontend_error, ERROR_CODE_FRONTEND);
    goto finally;

tree_error:
    err_dump(tree_error, ERROR_CODE_TREE);
    goto finally;

finally:
    cmd_data_delete(&cmd_data);
    input_data_delete(input_data);
    tree_destroy(tree);

    return 0;
}