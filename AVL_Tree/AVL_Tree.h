/*
 * @Date: 2023-11-09 16:42:12
 * @LastEditors: yanglei67
 * @LastEditTime: 2023-11-10 09:56:32
 * @Description: Do not edit
 */
typedef struct AVL_Tree_t AVL_Tree;


struct AVL_Tree_t
{
    char* Data;
    int Height;
    int Element;
    AVL_Tree* LeftSubTree;
    AVL_Tree* RightSubTree;
};


AVL_Tree* AVL_TreeDestroyWholeTree(AVL_Tree* RootNode);
AVL_Tree* AVL_TreeDeleteNode(AVL_Tree* RootNode, int element);
AVL_Tree* AVL_TreeInsertNode(AVL_Tree* RootNode,int Element, void* Data, int Datasize);
AVL_Tree* CreateSingleNode(int Element, void* Data, int Datasize);
void AVL_TreeInorder(AVL_Tree* RootNode);