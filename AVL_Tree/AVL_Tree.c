/*
 * @Date: 2023-11-09 18:36:39
 * @LastEditors: yanglei67
 * @LastEditTime: 2023-11-14 19:26:39
 * @Description: Do not edit
 */
#include "AVL_Tree.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


/**
 * @brief 生成单个结点，主要是将左右子树，和搜索时比对的元素，数据，
 * 以及相应的初始高度，父节点未进行操作
 * @param {int} Element 在搜索的时候进行比对的元素
 * @param {void*} Data  每个结点存储的元素地方
 * @param {int} Datasize 结点存储的元素的大小
 * @return 所生成的结点
 */
AVL_Tree* CreateSingleNode(int Element, void* Data, int Datasize)
{
    AVL_Tree* Node = (AVL_Tree*)malloc(sizeof(AVL_Tree));
    // printf("sizeof(AVL_Tree) =%ld\r\n",sizeof(AVL_Tree));
    void * MallocData = malloc(Datasize);
    memcpy(MallocData, Data, Datasize);
    Node->Data = MallocData;
    Node->Element = Element;
    Node->Height = 1;
    Node->LeftSubTree = NULL; 
    Node->RightSubTree = NULL;
    
    return Node;
}

#define Max(a,b) (a>b?a:b)

int AVL_TreeHeight(AVL_Tree* Tree)
{
    if(Tree == NULL){
        return 0;
    }else{
        return Tree->Height;
    }
}

/**
 * @brief 进行左旋操作，当右子树高时进行的操作
 * @param {AVL_Tree*} RootNode 进行旋转的根节点
 * @return 最新的根节点
 */
AVL_Tree* Left_LeftRotate(AVL_Tree* RootNode)
{
    /*将原根节点的右子树作为当前子树的新根节点*/
    AVL_Tree* NewRootNode = RootNode -> RightSubTree;
    
    /*将新根节点的左子树放置到原根节点的右子树*/
    RootNode ->RightSubTree = NewRootNode ->LeftSubTree;
    
    /*将新根节点的右子树设置为原根节点*/
    NewRootNode->LeftSubTree = RootNode;
    
    RootNode->Height = Max(AVL_TreeHeight(RootNode->LeftSubTree), AVL_TreeHeight(RootNode->RightSubTree)) + 1;

    NewRootNode->Height = Max(AVL_TreeHeight(NewRootNode->LeftSubTree), AVL_TreeHeight(RootNode->RightSubTree)) + 1;
    
    return NewRootNode;
}

/**
 * @brief 进行右旋操作, 当左子树高时进行的操作
 * @param {AVL_Tree*} RootNode 进行旋转的根节点
 * @return 最新的根节点
 */
AVL_Tree* Right_RightRotate(AVL_Tree* RootNode)
{
    /*将原根节点的左子树作为当前子树的新根节点*/
    AVL_Tree* NewRootNode = RootNode -> LeftSubTree;
    
    /*将新根节点的右子树放置到原根节点的左子树*/
    RootNode ->LeftSubTree = NewRootNode-> RightSubTree;
    
    /*将新根节点的右子树设置为原根节点*/
    NewRootNode->RightSubTree = RootNode;
    
    RootNode->Height = Max(AVL_TreeHeight(RootNode->LeftSubTree), AVL_TreeHeight(RootNode->RightSubTree)) + 1;

    NewRootNode->Height = Max(AVL_TreeHeight(NewRootNode->LeftSubTree), AVL_TreeHeight(NewRootNode->RightSubTree)) + 1;
    
    return NewRootNode;
}


/**
 * @brief AVL树平衡操作
 * @param {AVL_Tree*} RootNode 待平衡的根节点
 * @return {*}返回平衡后的根节点
 */
AVL_Tree* AVL_TreeBalanceOperation(AVL_Tree* RootNode)
{
    int BalanceFator;
    /*计算平衡因子*/
    BalanceFator = AVL_TreeHeight(RootNode->LeftSubTree) - AVL_TreeHeight(RootNode->RightSubTree);

    /*判断旋转类型*/
    if((BalanceFator >= 0 ? BalanceFator: -BalanceFator) > 1){
        // printf("BalanceFator = %d\r\n",BalanceFator);
        if(BalanceFator > 1){  //左子树高
            int LeftLeftTreeHeight = AVL_TreeHeight(RootNode->LeftSubTree->LeftSubTree);
            int LeftRightTreeHeight = AVL_TreeHeight(RootNode->LeftSubTree->RightSubTree);
            
            if(LeftLeftTreeHeight >= LeftRightTreeHeight){  //RR旋转
                RootNode = Right_RightRotate(RootNode);
            }else if(LeftLeftTreeHeight < LeftRightTreeHeight){  //RL旋转
                RootNode->LeftSubTree = Left_LeftRotate(RootNode->LeftSubTree);
                RootNode = Right_RightRotate(RootNode);
            }else{
                printf("error height occur!, LeftLeftTreeHeight =%d, LeftRightTreeHeight%d\r\n",LeftLeftTreeHeight, LeftRightTreeHeight);
            }
        }else if(BalanceFator < -1){   //右子树高时
            int RightRightTreeHeight = AVL_TreeHeight(RootNode->RightSubTree->RightSubTree);
            int RightLeftTreeHeight = AVL_TreeHeight(RootNode->RightSubTree->LeftSubTree);

            if(RightRightTreeHeight >= RightLeftTreeHeight){  //LL旋转，右右高
                RootNode = Left_LeftRotate(RootNode);
            }else if(RightRightTreeHeight < RightLeftTreeHeight){  //LR旋转，右左高
                RootNode->RightSubTree = Right_RightRotate(RootNode->RightSubTree);
                RootNode = Left_LeftRotate(RootNode);
            }else{
                printf("error height occur!,RightRightTreeHeight=%d,RightLeftTreeHeight=%d\r\n",RightRightTreeHeight,RightLeftTreeHeight);
            }
        }
    }
    /*计算当前节点的高度*/
    RootNode ->Height = Max(AVL_TreeHeight(RootNode->LeftSubTree), AVL_TreeHeight(RootNode->RightSubTree)) + 1;
    return RootNode;
}


/**
 * @brief 二叉树节点插入函数 
 * @param {AVL_Tree*} RootNode 根节点
 * @param {int} Element 二叉树查找时进行对比的元素
 * @param {void*} Data  二叉树获取的信息的头指针
 * @param {int} Datasize 存储二叉树信息的大小
 * @return {AVL_Tree*} 返回递归调用后的相对根节点
 */
AVL_Tree* AVL_TreeInsertNode(AVL_Tree* RootNode,int Element, void* Data, int Datasize)
{
    /*根节点为空*/
    if(RootNode == NULL){
        RootNode = CreateSingleNode(Element, Data, Datasize);
    }else if(RootNode->Element > Element){  //进入左子树
        RootNode->LeftSubTree = AVL_TreeInsertNode(RootNode->LeftSubTree, Element, Data, Datasize);
    }else if(RootNode->Element < Element){  //进入右子树
        RootNode->RightSubTree = AVL_TreeInsertNode(RootNode->RightSubTree, Element, Data, Datasize);
    }else{
        return RootNode;
    }
   
    return AVL_TreeBalanceOperation(RootNode);
}

/**
 * @brief 根据二叉树排序元素来查询相应的元素首地址
 * @param {AVL_Tree*} RootNode 根节点
 * @param {int} element 进行二叉树排序时的元素
 * @return 元素对应的数据的头指针, 如果没有查找到相应的元素，返回空指针
 */
void * AVL_TreeSearch(AVL_Tree* RootNode, int element)
{
    if(RootNode == NULL){
        return NULL;
    }else if(RootNode->Element == element){
        return RootNode->Data;
    }else if(element < RootNode->Element){
        return AVL_TreeSearch(RootNode->LeftSubTree, element);
    }else{
        return AVL_TreeSearch(RootNode->RightSubTree, element);
    }
}

/**
 * @brief 寻找最深的左子树 
 * @param {AVL_Tree*} RootNode 根节点
 * @return 最深的左子树节点
 */
AVL_Tree* AVL_TreeDeleteLowestLeftSubTree(AVL_Tree* RootNode, AVL_Tree* DeleteNode)
{
    if(RootNode->LeftSubTree == NULL){
        AVL_Tree* RightSubTree = RootNode ->RightSubTree;
        /*释放删除节点存储的数据*/
        free(DeleteNode->Data);
        DeleteNode->Data = RootNode->Data;
        DeleteNode->Element = RootNode->Element;
        /*释放删除节点的空间*/
        free(RootNode);
        return RightSubTree;
    }else{
        RootNode->LeftSubTree = AVL_TreeDeleteLowestLeftSubTree(RootNode->LeftSubTree,DeleteNode);
        return AVL_TreeBalanceOperation(RootNode);
    }
}

/**
 * @brief 寻找最深的右子树 
 * @param {AVL_Tree*} RootNode 根节点
 * @return 最深的右子树节点
 */
AVL_Tree* AVL_TreeDeleteLowestRightSubTree(AVL_Tree* RootNode, AVL_Tree* DeleteNode)
{
    if(RootNode->RightSubTree == NULL){
        AVL_Tree* LeftSubTree = RootNode ->LeftSubTree;
        /*释放删除节点存储的数据*/
        free(DeleteNode->Data);
        DeleteNode->Data = RootNode->Data;
        DeleteNode->Element = RootNode->Element;
        /*释放删除节点的空间*/
        free(RootNode);
        return LeftSubTree;
    }else{
        RootNode->RightSubTree = AVL_TreeDeleteLowestRightSubTree(RootNode->RightSubTree,DeleteNode);
        return AVL_TreeBalanceOperation(RootNode);
    }
}

/**
 * @brief 删除一个特定的节点 
 * @param {AVL_Tree*} RootNode 递归调用的子树根节点，首次调用传入整个树的根节点
 * @param {int} element 进行查找时的键
 * @return {*} 当前子树的根节点
 */
AVL_Tree* AVL_TreeDeleteNode(AVL_Tree* RootNode, int element)
{
    if(RootNode == NULL){
        printf("element is not in AVL tree, fail to delete!\r\n");
        return NULL;
    }else if(RootNode ->Element == element){
        int RightSubTreeHeight = AVL_TreeHeight(RootNode->RightSubTree);
        int LeftSubTreeHeight  = AVL_TreeHeight(RootNode->LeftSubTree);
        if(RightSubTreeHeight == 0 && LeftSubTreeHeight == 0){
            free(RootNode->Data);
            free(RootNode);
            return NULL;
        }else if(RightSubTreeHeight >= LeftSubTreeHeight){
            RootNode->RightSubTree = AVL_TreeDeleteLowestLeftSubTree(RootNode->RightSubTree, RootNode);
            return AVL_TreeBalanceOperation(RootNode);
        }else{
            RootNode->LeftSubTree = AVL_TreeDeleteLowestLeftSubTree(RootNode->LeftSubTree, RootNode);
            return AVL_TreeBalanceOperation(RootNode);
        }
    }else if(element > RootNode ->Element){
        RootNode->RightSubTree = AVL_TreeDeleteNode(RootNode->RightSubTree, element);
        return AVL_TreeBalanceOperation(RootNode);
    }else if(element < RootNode ->Element){
        RootNode->LeftSubTree = AVL_TreeDeleteNode(RootNode->LeftSubTree, element);
        return AVL_TreeBalanceOperation(RootNode);
    }
}

/**
 * @brief 中序遍历AVL树
 * @param {AVL_Tree*} RootNode 递归调用的子树根节点，首次调用传入整个树的根节点
 * @return:
 */
void AVL_TreeInorder(AVL_Tree* RootNode)
{
    if(RootNode != NULL){
        AVL_TreeInorder(RootNode->LeftSubTree);
        printf("%d,",RootNode->Element);
        AVL_TreeInorder(RootNode->RightSubTree);
    }else{
        return;
    }
}

/**
 * @brief 销毁整个树 
 * @param {AVL_Tree*}RootNode，首次调用时传入整个二叉树的根节点
 * @return {*}返回当前子树的根节点
 */
AVL_Tree* AVL_TreeDestroyWholeTree(AVL_Tree* RootNode)
{
    if(RootNode ==NULL){
        return NULL;
    }
    /*释放左子树*/
    AVL_TreeDestroyWholeTree(RootNode->LeftSubTree);
    /*释放右子树*/
    AVL_TreeDestroyWholeTree(RootNode->RightSubTree);
    /*释放子树的根节点*/
    free(RootNode->Data);
    free(RootNode);
    return NULL;
}