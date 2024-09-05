#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "AVL_Tree.h"

int main()
{
    AVL_Tree* RootNode = NULL;
    char data[10] = "dsada";
    for(int i = 0; i < 100;i++){
        RootNode = AVL_TreeInsertNode(RootNode,i,data,sizeof(data));
    }
    AVL_TreeInorder(RootNode);
    RootNode = AVL_TreeDeleteNode(RootNode,51);
    RootNode = AVL_TreeDeleteNode(RootNode,23);
    RootNode = AVL_TreeDeleteNode(RootNode,91);
    RootNode = AVL_TreeDeleteNode(RootNode,16);
    printf("-------------------\r\n");
    AVL_TreeInorder(RootNode);
    AVL_TreeDestroyWholeTree(RootNode);
    return 0;
}