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
 * @brief ���ɵ�����㣬��Ҫ�ǽ�����������������ʱ�ȶԵ�Ԫ�أ����ݣ�
 * �Լ���Ӧ�ĳ�ʼ�߶ȣ����ڵ�δ���в���
 * @param {int} Element ��������ʱ����бȶԵ�Ԫ��
 * @param {void*} Data  ÿ�����洢��Ԫ�صط�
 * @param {int} Datasize ���洢��Ԫ�صĴ�С
 * @return �����ɵĽ��
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
 * @brief ������������������������ʱ���еĲ���
 * @param {AVL_Tree*} RootNode ������ת�ĸ��ڵ�
 * @return ���µĸ��ڵ�
 */
AVL_Tree* Left_LeftRotate(AVL_Tree* RootNode)
{
    /*��ԭ���ڵ����������Ϊ��ǰ�������¸��ڵ�*/
    AVL_Tree* NewRootNode = RootNode -> RightSubTree;
    
    /*���¸��ڵ�����������õ�ԭ���ڵ��������*/
    RootNode ->RightSubTree = NewRootNode ->LeftSubTree;
    
    /*���¸��ڵ������������Ϊԭ���ڵ�*/
    NewRootNode->LeftSubTree = RootNode;
    
    RootNode->Height = Max(AVL_TreeHeight(RootNode->LeftSubTree), AVL_TreeHeight(RootNode->RightSubTree)) + 1;

    NewRootNode->Height = Max(AVL_TreeHeight(NewRootNode->LeftSubTree), AVL_TreeHeight(RootNode->RightSubTree)) + 1;
    
    return NewRootNode;
}

/**
 * @brief ������������, ����������ʱ���еĲ���
 * @param {AVL_Tree*} RootNode ������ת�ĸ��ڵ�
 * @return ���µĸ��ڵ�
 */
AVL_Tree* Right_RightRotate(AVL_Tree* RootNode)
{
    /*��ԭ���ڵ����������Ϊ��ǰ�������¸��ڵ�*/
    AVL_Tree* NewRootNode = RootNode -> LeftSubTree;
    
    /*���¸��ڵ�����������õ�ԭ���ڵ��������*/
    RootNode ->LeftSubTree = NewRootNode-> RightSubTree;
    
    /*���¸��ڵ������������Ϊԭ���ڵ�*/
    NewRootNode->RightSubTree = RootNode;
    
    RootNode->Height = Max(AVL_TreeHeight(RootNode->LeftSubTree), AVL_TreeHeight(RootNode->RightSubTree)) + 1;

    NewRootNode->Height = Max(AVL_TreeHeight(NewRootNode->LeftSubTree), AVL_TreeHeight(NewRootNode->RightSubTree)) + 1;
    
    return NewRootNode;
}


/**
 * @brief AVL��ƽ�����
 * @param {AVL_Tree*} RootNode ��ƽ��ĸ��ڵ�
 * @return {*}����ƽ���ĸ��ڵ�
 */
AVL_Tree* AVL_TreeBalanceOperation(AVL_Tree* RootNode)
{
    int BalanceFator;
    /*����ƽ������*/
    BalanceFator = AVL_TreeHeight(RootNode->LeftSubTree) - AVL_TreeHeight(RootNode->RightSubTree);

    /*�ж���ת����*/
    if((BalanceFator >= 0 ? BalanceFator: -BalanceFator) > 1){
        // printf("BalanceFator = %d\r\n",BalanceFator);
        if(BalanceFator > 1){  //��������
            int LeftLeftTreeHeight = AVL_TreeHeight(RootNode->LeftSubTree->LeftSubTree);
            int LeftRightTreeHeight = AVL_TreeHeight(RootNode->LeftSubTree->RightSubTree);
            
            if(LeftLeftTreeHeight >= LeftRightTreeHeight){  //RR��ת
                RootNode = Right_RightRotate(RootNode);
            }else if(LeftLeftTreeHeight < LeftRightTreeHeight){  //RL��ת
                RootNode->LeftSubTree = Left_LeftRotate(RootNode->LeftSubTree);
                RootNode = Right_RightRotate(RootNode);
            }else{
                printf("error height occur!, LeftLeftTreeHeight =%d, LeftRightTreeHeight%d\r\n",LeftLeftTreeHeight, LeftRightTreeHeight);
            }
        }else if(BalanceFator < -1){   //��������ʱ
            int RightRightTreeHeight = AVL_TreeHeight(RootNode->RightSubTree->RightSubTree);
            int RightLeftTreeHeight = AVL_TreeHeight(RootNode->RightSubTree->LeftSubTree);

            if(RightRightTreeHeight >= RightLeftTreeHeight){  //LL��ת�����Ҹ�
                RootNode = Left_LeftRotate(RootNode);
            }else if(RightRightTreeHeight < RightLeftTreeHeight){  //LR��ת�������
                RootNode->RightSubTree = Right_RightRotate(RootNode->RightSubTree);
                RootNode = Left_LeftRotate(RootNode);
            }else{
                printf("error height occur!,RightRightTreeHeight=%d,RightLeftTreeHeight=%d\r\n",RightRightTreeHeight,RightLeftTreeHeight);
            }
        }
    }
    /*���㵱ǰ�ڵ�ĸ߶�*/
    RootNode ->Height = Max(AVL_TreeHeight(RootNode->LeftSubTree), AVL_TreeHeight(RootNode->RightSubTree)) + 1;
    return RootNode;
}


/**
 * @brief �������ڵ���뺯�� 
 * @param {AVL_Tree*} RootNode ���ڵ�
 * @param {int} Element ����������ʱ���жԱȵ�Ԫ��
 * @param {void*} Data  ��������ȡ����Ϣ��ͷָ��
 * @param {int} Datasize �洢��������Ϣ�Ĵ�С
 * @return {AVL_Tree*} ���صݹ���ú����Ը��ڵ�
 */
AVL_Tree* AVL_TreeInsertNode(AVL_Tree* RootNode,int Element, void* Data, int Datasize)
{
    /*���ڵ�Ϊ��*/
    if(RootNode == NULL){
        RootNode = CreateSingleNode(Element, Data, Datasize);
    }else if(RootNode->Element > Element){  //����������
        RootNode->LeftSubTree = AVL_TreeInsertNode(RootNode->LeftSubTree, Element, Data, Datasize);
    }else if(RootNode->Element < Element){  //����������
        RootNode->RightSubTree = AVL_TreeInsertNode(RootNode->RightSubTree, Element, Data, Datasize);
    }else{
        return RootNode;
    }
   
    return AVL_TreeBalanceOperation(RootNode);
}

/**
 * @brief ���ݶ���������Ԫ������ѯ��Ӧ��Ԫ���׵�ַ
 * @param {AVL_Tree*} RootNode ���ڵ�
 * @param {int} element ���ж���������ʱ��Ԫ��
 * @return Ԫ�ض�Ӧ�����ݵ�ͷָ��, ���û�в��ҵ���Ӧ��Ԫ�أ����ؿ�ָ��
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
 * @brief Ѱ������������� 
 * @param {AVL_Tree*} RootNode ���ڵ�
 * @return ������������ڵ�
 */
AVL_Tree* AVL_TreeDeleteLowestLeftSubTree(AVL_Tree* RootNode, AVL_Tree* DeleteNode)
{
    if(RootNode->LeftSubTree == NULL){
        AVL_Tree* RightSubTree = RootNode ->RightSubTree;
        /*�ͷ�ɾ���ڵ�洢������*/
        free(DeleteNode->Data);
        DeleteNode->Data = RootNode->Data;
        DeleteNode->Element = RootNode->Element;
        /*�ͷ�ɾ���ڵ�Ŀռ�*/
        free(RootNode);
        return RightSubTree;
    }else{
        RootNode->LeftSubTree = AVL_TreeDeleteLowestLeftSubTree(RootNode->LeftSubTree,DeleteNode);
        return AVL_TreeBalanceOperation(RootNode);
    }
}

/**
 * @brief Ѱ������������� 
 * @param {AVL_Tree*} RootNode ���ڵ�
 * @return ������������ڵ�
 */
AVL_Tree* AVL_TreeDeleteLowestRightSubTree(AVL_Tree* RootNode, AVL_Tree* DeleteNode)
{
    if(RootNode->RightSubTree == NULL){
        AVL_Tree* LeftSubTree = RootNode ->LeftSubTree;
        /*�ͷ�ɾ���ڵ�洢������*/
        free(DeleteNode->Data);
        DeleteNode->Data = RootNode->Data;
        DeleteNode->Element = RootNode->Element;
        /*�ͷ�ɾ���ڵ�Ŀռ�*/
        free(RootNode);
        return LeftSubTree;
    }else{
        RootNode->RightSubTree = AVL_TreeDeleteLowestRightSubTree(RootNode->RightSubTree,DeleteNode);
        return AVL_TreeBalanceOperation(RootNode);
    }
}

/**
 * @brief ɾ��һ���ض��Ľڵ� 
 * @param {AVL_Tree*} RootNode �ݹ���õ��������ڵ㣬�״ε��ô����������ĸ��ڵ�
 * @param {int} element ���в���ʱ�ļ�
 * @return {*} ��ǰ�����ĸ��ڵ�
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
 * @brief �������AVL��
 * @param {AVL_Tree*} RootNode �ݹ���õ��������ڵ㣬�״ε��ô����������ĸ��ڵ�
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
 * @brief ���������� 
 * @param {AVL_Tree*}RootNode���״ε���ʱ���������������ĸ��ڵ�
 * @return {*}���ص�ǰ�����ĸ��ڵ�
 */
AVL_Tree* AVL_TreeDestroyWholeTree(AVL_Tree* RootNode)
{
    if(RootNode ==NULL){
        return NULL;
    }
    /*�ͷ�������*/
    AVL_TreeDestroyWholeTree(RootNode->LeftSubTree);
    /*�ͷ�������*/
    AVL_TreeDestroyWholeTree(RootNode->RightSubTree);
    /*�ͷ������ĸ��ڵ�*/
    free(RootNode->Data);
    free(RootNode);
    return NULL;
}