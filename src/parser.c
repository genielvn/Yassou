#include <stdio.h>
#include <global.h>
#include <error.h>

// added your structure here
ParseNode *generateParseTree(Token *head)
{
    ParseNode *parse_tree = NULL;

    while (head != NULL)
    {
        
    }

    return parse_tree;
}

ParseNode *createNode(TokenType type)
{
    ParseNode *node = (ParseNode*)malloc(sizeof(ParseNode));
    MEMCHECK;

    node->token = type;
    node->first_child = NULL;
    node->sibling = NULL;

    return node;
}

void addChild(ParseNode *root, ParseNode *child)
{
    if (root->first_child == NULL) root->first_child = child;
    else
    {
        ParseNode *last_child = root->first_child;
        while (last_child != NULL) last_child = last_child->sibling;

        last_child->sibling = child;
    }
}

