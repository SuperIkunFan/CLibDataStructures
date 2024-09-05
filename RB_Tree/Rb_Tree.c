#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>


typedef enum{
    Red = 1,
    Black = 2,
}Color;

typedef enum{
    Pending_e = 0,
    LeftSubtree_e = 1,
    RightSubtree_e = 2,
}SubTreeDir;

typedef union
{
    unsigned long long Result;
    struct{
        /*共有信息，方向*/
        unsigned long long DoubleBlackNodeDir :3;
        unsigned long long UncleColor         :3;

        /*差异化信息*/
        unsigned long long LeftSubTreeColor   :3;
        unsigned long long RightSbuTreeColor  :3;
    };
}DoubleBlackType_Union;


typedef struct Rb_Tree_t Rb_Tree;
struct Rb_Tree_t
{
    void * Data;
    Color color;
    int Element;
    Rb_Tree* Parent;
    Rb_Tree* LeftSubTree;
    Rb_Tree* RightSubTree;
};

void Rb_TreeInorder(Rb_Tree* RootNode);
Rb_Tree* CreateNode(int Element, void * Data, size_t DataSize)
{
    Rb_Tree* Node = (Rb_Tree*)malloc(sizeof(Rb_Tree));
    void * mallocdata = malloc(DataSize);
    memcpy(mallocdata, Data, DataSize);
    Node->Data = mallocdata;
    Node->Element = Element;
    Node->color = Red;//插入节点为红色
    Node->Parent = NULL;
    Node->LeftSubTree = NULL;
    Node->RightSubTree = NULL;
    return Node;
}

/**
 * @brief 删除一个独立的节点
 * @param {Rb_Tree*} Node 被删除的节点
 */
void Rb_Tree_DestroyNode(Rb_Tree* Node)
{
    free(Node->Data);
    free(Node);
}

/**
 * @brief 检验一个节点是位于父节点的左子树还是右子树
 * @param {Rb_Tree*} Node 检验的节点
 * @return 方向
 */
SubTreeDir checkNodeOrientation(Rb_Tree* Node)
{
    if(Node->Parent == NULL){
        return Pending_e;
    }else if(Node->Parent->LeftSubTree == Node){
        return LeftSubtree_e;
    }else{
        return RightSubtree_e;
    }
}

/**
 * @brief 将一个已经连接于树上的叶子节点删除，涉及解除父节点索引
 * @param {Rb_Tree*} Node 进行删除的根节点
 * @return 父节点
 */
Rb_Tree* Rb_Tree_RemoveNodeFromTree(Rb_Tree* Node)
{
    Rb_Tree* FatherNode = NULL;
    /*将待删除节点的父节点中除去*/
    switch (checkNodeOrientation(Node))
    {
    case LeftSubtree_e:
        Node->Parent->LeftSubTree = NULL;
        break;
    case RightSubtree_e:
        Node->Parent->RightSubTree = NULL;
        break;
    default:
        break;
    }
    FatherNode = Node->Parent;
    free(Node->Data);
    free(Node);
    
    return FatherNode;
}

/**
 * @brief 进行左旋操作，当右子树高时进行的操作
 * @param {Rb_Tree*} RootNode 进行旋转的根节点
 * @return 最新的根节点
 */
Rb_Tree* Left_LeftRotate(Rb_Tree* RootNode)
{
    SubTreeDir Direction = checkNodeOrientation(RootNode);
    /*将原根节点的右子树作为当前子树的新根节点*/
    Rb_Tree* NewRootNode = RootNode -> RightSubTree;
    
    /*将旧根节点的父节点转移到新根节点的父节点上*/
    NewRootNode->Parent = RootNode ->Parent;

    /*将新根节点的左子树转移到旧根节点的右子树上*/
    RootNode ->RightSubTree = NewRootNode->LeftSubTree;
    
    /*再将旧根节点的新左子树（即原来的新根节点的右子树）的父节点绑定为旧节点*/
    if(RootNode ->RightSubTree != NULL) RootNode ->RightSubTree->Parent = RootNode;

    /*将旧根节点的插入到新根节点的左节点上*/
    NewRootNode->LeftSubTree = RootNode;

    /*将旧根节点的父节点设置为新根节点*/
    RootNode->Parent = NewRootNode;

    /*新根节点所代表的子树重新挂靠到父节点下*/
    switch (Direction)
    {
    case LeftSubtree_e:
        NewRootNode->Parent->LeftSubTree = NewRootNode;
        break;
    case RightSubtree_e:
        NewRootNode->Parent->RightSubTree = NewRootNode;
        break;
    default:
        break;
    }
    
    return NewRootNode;
}

/**
 * @brief 进行右旋操作, 当左子树高时进行的操作
 * @param {Rb_Tree*} RootNode 进行旋转的根节点
 * @return 最新的根节点
 */
Rb_Tree* Right_RightRotate(Rb_Tree* RootNode)
{
    SubTreeDir Direction = checkNodeOrientation(RootNode);
    /*将原根节点的左子树作为当前子树的新根节点*/
    Rb_Tree* NewRootNode = RootNode -> LeftSubTree;
    
    /*将旧根节点的父节点转移到新根节点的父节点上*/
    NewRootNode->Parent = RootNode ->Parent;

    /*将新根节点的右子树转移到旧根节点的左子树上*/
    RootNode ->LeftSubTree = NewRootNode->RightSubTree;
    
    /*再将旧根节点的新左子树（即原来的新根节点的右子树）的父节点绑定为旧节点*/
    if(RootNode ->LeftSubTree != NULL) RootNode ->LeftSubTree->Parent = RootNode;

    /*将旧根节点的插入到新根节点的左节点上*/
    NewRootNode->RightSubTree = RootNode;

    /*将旧根节点的父节点设置为新根节点*/
    RootNode->Parent = NewRootNode;

    /*新根节点所代表的子树重新挂靠到父节点下*/
    switch (Direction)
    {
    case LeftSubtree_e:
        NewRootNode->Parent->LeftSubTree = NewRootNode;
        break;
    case RightSubtree_e:
        NewRootNode->Parent->RightSubTree = NewRootNode;
        break;
    default:
        break;
    }
    
    return NewRootNode;
}
/**
 * @brief Get the Node Color object
 * 
 * @param Node 
 * @return Color 
 */
Color getNodeColor(Rb_Tree* Node)
{
    return Node == NULL ? Black:Node->color;
}

/**
 * @brief 向红黑树中插入节点的函数
 * 
 * @param RootNode 根节点
 * @param Element 红黑树排序标准
 * @param Data 数据指针
 * @param DataSize 数据大小
 * @return Rb_Tree* 最新的根节点
 */
Rb_Tree* Rb_Tree_InsertNode(Rb_Tree* RootNode,int Element, void * Data, size_t DataSize)
{
    if(RootNode == NULL){
        RootNode = CreateNode(Element, Data, DataSize);
        RootNode->color = Black;//根节点为黑色
        return RootNode;
    }else{
        Rb_Tree* Node = CreateNode(Element, Data, DataSize);
        bool AdjustFlag = true;
        
        Rb_Tree* MoveRootNode = RootNode;
        Rb_Tree* Direction = NULL;
        Rb_Tree* NewRootNode = NULL;
        

        SubTreeDir DirectionOfNodeInFatherNode = Pending_e;
        SubTreeDir DirectionOfFatherNodeInGrandfather = Pending_e;
        
        /*寻找最小子树的根节点*/
        do{
            /*插入元素大于当前根节点，进入右子树*/
            if(MoveRootNode->Element < Node->Element){
                Direction = MoveRootNode->RightSubTree;
            }/*插入元素小于当前根节点，进入左子树*/
            else if(MoveRootNode->Element > Node->Element){
                Direction = MoveRootNode->LeftSubTree;
            }else{
                printf("Elemnt is equal!!! please check element!\r\n");
                Rb_Tree_DestroyNode(Node);
                return RootNode;
            }
            
            if(Direction != NULL){
                MoveRootNode = Direction;
            }else{
                /*插入元素*/
                if(MoveRootNode->Element < Node->Element){
                    MoveRootNode->RightSubTree = Node;
                    Node->Parent = MoveRootNode;
                }else{
                    MoveRootNode->LeftSubTree = Node;
                    Node->Parent = MoveRootNode;
                }
                break;
            }
            
        }while(1);
        while (getNodeColor(Node->Parent) == Red)
        {
            /*获取当前节点在父节点中的位置*/
            DirectionOfNodeInFatherNode = checkNodeOrientation(Node);
            if(DirectionOfNodeInFatherNode == Pending_e){
                printf("%d, element = %d\r\n",__LINE__,Element);
                break;
            }
            
            DirectionOfFatherNodeInGrandfather = checkNodeOrientation(Node->Parent);
            if(DirectionOfFatherNodeInGrandfather == Pending_e){
                printf("%d,element = %d\r\n",__LINE__,Element);
                break;
            }
            printf("%d, element = %d, excuted adjust, %d,%d\r\n",__LINE__,Element,DirectionOfNodeInFatherNode,DirectionOfFatherNodeInGrandfather);
            switch (DirectionOfNodeInFatherNode|DirectionOfFatherNodeInGrandfather<<16)
            {
            case (LeftSubtree_e|LeftSubtree_e<<16):
                {
                    if(getNodeColor(Node->Parent->Parent->RightSubTree) == Red){
                        Node->Parent->color = Black;
                        Node->Parent->Parent->RightSubTree->color = Black;
                        Node->Parent->Parent->color = Red;
                        /*将红色节点置于祖父节点，检验是否破坏不能连续红色的规则*/
                        Node = Node->Parent->Parent;
                    }else{
                        Node->Parent->color = Black;
                        Node->Parent->Parent->color = Red;
                        printf("%d,element = %d\r\n",__LINE__,Element);
                        
                        NewRootNode = Right_RightRotate(Node->Parent->Parent);
                        Node = NewRootNode;
                    }
                }
                break;
            case (RightSubtree_e|LeftSubtree_e<<16):
                {
                    if(getNodeColor(Node->Parent->Parent->RightSubTree) == Red){
                        Node->Parent->color = Black;
                        Node->Parent->Parent->RightSubTree->color = Black;
                        Node->Parent->Parent->color = Red;
                        
                        Node = Node->Parent->Parent;
                    }else{
                        NewRootNode = Left_LeftRotate(Node->Parent);
                        
                        NewRootNode->color = Black;
                        NewRootNode->Parent->color = Red;
                        
                        NewRootNode = Right_RightRotate(NewRootNode->Parent);
                        Node = NewRootNode;
                    }
                }
                break;
            case (RightSubtree_e|RightSubtree_e<<16):
                {
                    if(getNodeColor(Node->Parent->Parent->LeftSubTree) == Red){
                        Node->Parent->color = Black;
                        Node->Parent->Parent->LeftSubTree->color = Black;
                        Node->Parent->Parent->color = Red;
                        
                        Node = Node->Parent->Parent;
                    }else{
                        Node->Parent->color = Black;
                        Node->Parent->Parent->color = Red;
                        printf("%d,element = %d\r\n",__LINE__,Element);
                        
                        NewRootNode = Left_LeftRotate(Node->Parent->Parent);
                        Node = NewRootNode;
                    }
                }
                break;
            case (LeftSubtree_e|RightSubtree_e<<16):
                {
                    if(getNodeColor(Node->Parent->Parent->LeftSubTree) == Red){
                        Node->Parent->color = Black;
                        Node->Parent->Parent->LeftSubTree->color = Black;
                        Node->Parent->Parent->color = Red;
                        
                        Node = Node->Parent->Parent;
                    }else{
                        NewRootNode = Right_RightRotate(Node->Parent);
                        
                        NewRootNode->color = Black;
                        NewRootNode->Parent->color = Red;
                        
                        NewRootNode = Left_LeftRotate(NewRootNode->Parent);
                        Node = NewRootNode;
                    }
                }
                break;
            default:
                break;
            }
        }
        if(NewRootNode!= NULL) {
            if(NewRootNode->Parent == NULL) RootNode = NewRootNode;
        }
        RootNode->color = Black;
        return RootNode;
    }
}

/**
 * @brief 红黑树中寻找特定元素
 * 
 * @param RootNode 根节点
 * @param Element 搜索的元素大小
 * @return Rb_Tree* 搜索到的元素地址
 */
Rb_Tree* Rb_Tree_FindNode(Rb_Tree* RootNode,int Element)
{
    Rb_Tree* TargetPoint = NULL;
    while (RootNode != NULL)
    {
        if(RootNode->Element == Element){
            TargetPoint = RootNode;
            break;
        }else if(RootNode->Element > Element){
            RootNode = RootNode->LeftSubTree;
        }else{
            RootNode = RootNode->RightSubTree;
        }
    }
    return TargetPoint;
}
/**
 * @brief 寻找最小的节点
 * 
 * @param RootNode 相对的根节点
 * @param Element 排序元素大小
 * @return Rb_Tree* 寻找到的节点元素
 */
Rb_Tree* Rb_Tree_FindMinNode(Rb_Tree* RootNode,int Element)
{
    if( RootNode == NULL) return NULL;

    while (RootNode->LeftSubTree != NULL)
    {
        RootNode = RootNode->LeftSubTree;
    }
    return RootNode;
}

Rb_Tree* Rb_Tree_FindMaxNode(Rb_Tree* RootNode,int Element)
{
    if( RootNode == NULL) return NULL;

    while (RootNode->RightSubTree!= NULL)
    {
        RootNode = RootNode->RightSubTree;
    }
    return RootNode;
}
/**
 * @brief 平衡双黑节点的函数
 * 
 * @param DoubleBlackNode 双黑节点的地址
 * @return Rb_Tree* 相对根节点
 */
Rb_Tree* Rb_Tree_BalanceDoubleBlackNode(Rb_Tree* DoubleBlackNode)
{
    SubTreeDir DirectionOfNodeInFatherNode = Pending_e;
    SubTreeDir DirectionOfFatherNodeInGrandfather = Pending_e;
    DoubleBlackType_Union RelaticveRoot = {0};
    Rb_Tree* WholeTreeNewRooTNode = NULL;
    bool ExitFlag = true;

    Rb_Tree* NewRootNode = NULL;

    while (1)
    {
        static const DoubleBlackType_Union Type1 = {
            .DoubleBlackNodeDir = LeftSubtree_e,
            .UncleColor         = Black,
            .LeftSubTreeColor   = Black,
            .RightSbuTreeColor  = Black,
        };
        
        static const DoubleBlackType_Union Type2 = {
            .DoubleBlackNodeDir = LeftSubtree_e,
            .UncleColor         = Red,
            .LeftSubTreeColor   = Black,
            .RightSbuTreeColor  = Black,
        };

        static const DoubleBlackType_Union Type3 = {
            .DoubleBlackNodeDir = LeftSubtree_e,
            .UncleColor         = Black,
            .LeftSubTreeColor   = Red,
            .RightSbuTreeColor  = Black,
        };
        static const DoubleBlackType_Union Type4 = {
            .DoubleBlackNodeDir = LeftSubtree_e,
            .UncleColor         = Black,
            .LeftSubTreeColor   = Black,
            .RightSbuTreeColor  = Red,
        };
        static const DoubleBlackType_Union Type5 = {
            .DoubleBlackNodeDir = LeftSubtree_e,
            .UncleColor         = Black,
            .LeftSubTreeColor   = Red,
            .RightSbuTreeColor  = Red,
        };
        
        static const DoubleBlackType_Union Type6 = {
            .DoubleBlackNodeDir = RightSubtree_e,
            .UncleColor         = Black,
            .LeftSubTreeColor   = Black,
            .RightSbuTreeColor  = Black,
        };
        
        static const DoubleBlackType_Union Type7 = {
            .DoubleBlackNodeDir = RightSubtree_e,
            .UncleColor         = Red,
            .LeftSubTreeColor   = Black,
            .RightSbuTreeColor  = Black,
        };

        static const DoubleBlackType_Union Type8 = {
            .DoubleBlackNodeDir = RightSubtree_e,
            .UncleColor         = Black,
            .LeftSubTreeColor   = Red,
            .RightSbuTreeColor  = Black,
        };
        static const DoubleBlackType_Union Type9 = {
            .DoubleBlackNodeDir = RightSubtree_e,
            .UncleColor         = Black,
            .LeftSubTreeColor   = Black,
            .RightSbuTreeColor  = Red,
        };
        static const DoubleBlackType_Union Type10 = {
            .DoubleBlackNodeDir = RightSubtree_e,
            .UncleColor         = Black,
            .LeftSubTreeColor   = Red,
            .RightSbuTreeColor  = Red,
        };

        ExitFlag = true;

        /*获取当前节点在父节点中的位置*/
        DirectionOfNodeInFatherNode = checkNodeOrientation(DoubleBlackNode);
        if(DirectionOfNodeInFatherNode == Pending_e){
            break;
        }
        
        RelaticveRoot.Result = 0;
        
        RelaticveRoot.DoubleBlackNodeDir = DirectionOfNodeInFatherNode;
        Rb_Tree* BrotherNode = DirectionOfNodeInFatherNode == LeftSubtree_e?
            DoubleBlackNode->Parent->RightSubTree:
            DoubleBlackNode->Parent->LeftSubTree;
        RelaticveRoot.UncleColor = getNodeColor(BrotherNode);
        RelaticveRoot.LeftSubTreeColor = getNodeColor(BrotherNode->LeftSubTree);
        RelaticveRoot.RightSbuTreeColor = getNodeColor(BrotherNode->RightSubTree);
        printf("%d:element = %d, result = %s,%s,%s,%s\r\n",__LINE__, DoubleBlackNode->Element,
        RelaticveRoot.DoubleBlackNodeDir== LeftSubtree_e ?"LeftSubtree_e":RelaticveRoot.DoubleBlackNodeDir == RightSubtree_e?"RightSubtree_e":"pending",
        RelaticveRoot.UncleColor == Red? "Red":"Black",
            RelaticveRoot.LeftSubTreeColor == Red? "Red":"Black"  ,
            RelaticveRoot.RightSbuTreeColor== Red? "Red":"Black" );
        /*双黑节点在左子树，且兄弟节点和侄儿节点全是黑色*/
        if(RelaticveRoot.Result == Type1.Result){
            DoubleBlackNode->Parent->RightSubTree->color = Red;
            if(DoubleBlackNode->Parent->color == Red){
                DoubleBlackNode->Parent->color = Black;
            }else{
                ExitFlag = false;
                DoubleBlackNode = DoubleBlackNode->Parent;
            }
        }/*黑点在左子树，叔叔节点是红色，侄儿节点全是黑色*/
        else if(RelaticveRoot.Result == Type2.Result){
            DoubleBlackNode->Parent->color = Red;
            DoubleBlackNode->Parent->RightSubTree->color = Black;

            NewRootNode = Left_LeftRotate(DoubleBlackNode->Parent);
            /*调整后双黑节点的侄子们的边上有一个是红色的*/
            if(getNodeColor(NewRootNode->LeftSubTree->RightSubTree->RightSubTree) == Red){
                NewRootNode->LeftSubTree->RightSubTree->RightSubTree->color = Black;
                NewRootNode->LeftSubTree->color = Black;
                NewRootNode->LeftSubTree->RightSubTree->color = Red;
                NewRootNode->LeftSubTree = Left_LeftRotate(DoubleBlackNode->Parent);
            }else if(getNodeColor(NewRootNode->LeftSubTree->RightSubTree->LeftSubTree) == Red){
                /*先将红色节点旋转至最右侧，并切换颜色*/
                NewRootNode->LeftSubTree->RightSubTree->color = Red; //先交换颜色，原本是红色的，但是由于存在双黑节点，将红色取代后变为黑色
                NewRootNode->LeftSubTree->RightSubTree->LeftSubTree->color = Black;
                NewRootNode->LeftSubTree->RightSubTree = Right_RightRotate(NewRootNode->LeftSubTree->RightSubTree);

                NewRootNode->LeftSubTree->RightSubTree->RightSubTree->color = Black;
                NewRootNode->LeftSubTree->color = Black;
                NewRootNode->LeftSubTree->RightSubTree->color = Red;
                NewRootNode->LeftSubTree = Left_LeftRotate(DoubleBlackNode->Parent);
            }else{
                NewRootNode->LeftSubTree->color =Black;
                NewRootNode->LeftSubTree->RightSubTree->color = Red;
            }
        }/*双黑节点在左子树，叔叔节点是黑色，左侄儿节点是红色，右侄儿节点是黑色*/
        else if(RelaticveRoot.Result == Type3.Result){
            DoubleBlackNode->Parent->RightSubTree->color = Red;
            DoubleBlackNode->Parent->RightSubTree->LeftSubTree->color = Black;
            DoubleBlackNode->Parent->RightSubTree = Right_RightRotate(DoubleBlackNode->Parent->RightSubTree);
            
            DoubleBlackNode->Parent->RightSubTree->RightSubTree->color = Black;
            DoubleBlackNode->Parent->RightSubTree->color = DoubleBlackNode->Parent->color;
            DoubleBlackNode->Parent->color = Black;
            NewRootNode = Left_LeftRotate(DoubleBlackNode->Parent);
            
        }/*双黑节点在左子树，叔叔节点是黑色，左侄儿节点是黑色，右侄儿节点是红色
        或者是 双黑节点在左子树，叔叔节点是黑色，左侄儿节点是红色，右侄儿节点是红色
        */
        else if( RelaticveRoot.Result == Type4.Result ||
            RelaticveRoot.Result == Type5.Result ){
            DoubleBlackNode->Parent->RightSubTree->RightSubTree->color = Black;
            DoubleBlackNode->Parent->RightSubTree->color = DoubleBlackNode->Parent->color;
            DoubleBlackNode->Parent->color = Black;
            NewRootNode = Left_LeftRotate(DoubleBlackNode->Parent);
        }/*黑点在右子树，且兄弟节点和侄儿节点全是黑色*/
        else if(RelaticveRoot.Result == Type6.Result){
            DoubleBlackNode->Parent->LeftSubTree->color = Red;
            if(DoubleBlackNode->Parent->color == Red){
                DoubleBlackNode->Parent->color = Black;
            }else{
                ExitFlag = false;
                DoubleBlackNode = DoubleBlackNode->Parent;
            }
        }/*双黑节点在右子树，叔叔节点是红色，侄儿节点全是黑色*/
        else if(RelaticveRoot.Result == Type7.Result){
            //todo, 还有问题, 仔细检查
            DoubleBlackNode->Parent->color = Red;
            DoubleBlackNode->Parent->LeftSubTree->color = Black;
            
            NewRootNode = Right_RightRotate(DoubleBlackNode->Parent);
            /*调整后双黑节点的侄子们的边上有一个是红色的*/
            if(getNodeColor(NewRootNode->RightSubTree->LeftSubTree->LeftSubTree) == Red){
                NewRootNode->RightSubTree->LeftSubTree->LeftSubTree->color = Black;
                NewRootNode->RightSubTree->color = Black;
                NewRootNode->RightSubTree->LeftSubTree->color = Red;
                NewRootNode->RightSubTree = Right_RightRotate(DoubleBlackNode->Parent);
            }else if(getNodeColor(NewRootNode->RightSubTree->LeftSubTree->RightSubTree) == Red){
                /*先将红色节点旋转至最左侧，并切换颜色*/
                NewRootNode->RightSubTree->LeftSubTree->color = Red; //先交换颜色，原本是红色的，但是由于存在双黑节点，将红色取代后变为黑色
                NewRootNode->RightSubTree->LeftSubTree->RightSubTree->color = Black;
                NewRootNode->RightSubTree->LeftSubTree = Left_LeftRotate(NewRootNode->RightSubTree->LeftSubTree);

                NewRootNode->RightSubTree->LeftSubTree->LeftSubTree->color = Black;
                NewRootNode->RightSubTree->color = Black;
                NewRootNode->RightSubTree->LeftSubTree->color = Red;
                NewRootNode->RightSubTree = Right_RightRotate(DoubleBlackNode->Parent);
            }else{
                NewRootNode->RightSubTree->color =Black;
                NewRootNode->RightSubTree->LeftSubTree->color = Red;
            }
        }/*双黑节点在右子树，叔叔节点是黑色，左侄儿节点是黑色，右侄儿节点是红色*/
        else if(RelaticveRoot.Result == Type9.Result){
            DoubleBlackNode->Parent->LeftSubTree->color = Red;
            DoubleBlackNode->Parent->LeftSubTree->RightSubTree->color = Black;
            DoubleBlackNode->Parent->LeftSubTree = Left_LeftRotate(DoubleBlackNode->Parent->LeftSubTree);
            
            DoubleBlackNode->Parent->LeftSubTree->LeftSubTree->color = Black;
            DoubleBlackNode->Parent->LeftSubTree->color = DoubleBlackNode->Parent->color;
            DoubleBlackNode->Parent->color = Black;
            NewRootNode = Right_RightRotate(DoubleBlackNode->Parent);

        }/*双黑节点在右子树，叔叔节点是黑色，左侄儿节点是红色，右侄儿节点是红色
        或者是 双黑节点在右子树，叔叔节点是黑色，左侄儿节点是红色，右侄儿节点是黑色
        */
        else if( RelaticveRoot.Result == Type8.Result ||
            RelaticveRoot.Result == Type10.Result ){
            
            DoubleBlackNode->Parent->LeftSubTree->LeftSubTree->color = Black;
            DoubleBlackNode->Parent->LeftSubTree->color = DoubleBlackNode->Parent->color;
            DoubleBlackNode->Parent->color = Black;
            NewRootNode = Right_RightRotate(DoubleBlackNode->Parent);

        }else{
            printf("wrong type, please check !\r\n");
        }
        if(ExitFlag == true){
            break;
        }
    }

    if(NewRootNode != NULL && NewRootNode->Parent == NULL){
        WholeTreeNewRooTNode = NewRootNode;
    }
    return WholeTreeNewRooTNode;
}
/**
 * @brief 交换两个节点的位置关系，以及颜色
 * 
 * @param A 
 * @param B 
 */
void SwapTwoNodeLocationWithColor(Rb_Tree* A, Rb_Tree* B)
{
    Rb_Tree* A_Father, *A_LeftSubTree, *A_RightSubTree;
    Color A_color;
    SubTreeDir A_InFatherDirection, B_InFatherDirection;
    
    A_InFatherDirection = checkNodeOrientation(A);
    B_InFatherDirection = checkNodeOrientation(B);
    
    /*交换AB*/
    if(B->Parent == A){
        Rb_Tree* Temp = A;
        A = B;
        B = Temp;
        A_InFatherDirection = checkNodeOrientation(A);
        B_InFatherDirection = checkNodeOrientation(B);
    }

    /*即交换的两个节点是互相连接的*/
    if(A->Parent == B){
        A->Parent = A;
        if(A_InFatherDirection == LeftSubtree_e){
            B->LeftSubTree = B;
        }else{
            B->RightSubTree = B;
        }
    }
    
    A_Father = A->Parent;
    A_LeftSubTree = A->LeftSubTree;
    A_RightSubTree= A->RightSubTree;
    A_color = A->color;

    A->Parent = B->Parent;
    /*将A节点挂靠到B的父节点的子树下*/
    switch (B_InFatherDirection)
    {
    case LeftSubtree_e:
        A->Parent->LeftSubTree = A;
        break;
    case RightSubtree_e:
        A->Parent->RightSubTree = A;
        break;
    default:
        break;
    }
    A->LeftSubTree = B->LeftSubTree;
    if(A->LeftSubTree != NULL) A->LeftSubTree->Parent = A;
    A->RightSubTree = B->RightSubTree;
    if(A->RightSubTree != NULL) A->RightSubTree->Parent = A;
    A->color = B->color;

    B->Parent = A_Father;
    /*将B节点挂靠到原A的父节点的子树下*/
    switch (A_InFatherDirection)
    {
    case LeftSubtree_e:
        B->Parent->LeftSubTree = B;
        break;
    case RightSubtree_e:
        B->Parent->RightSubTree = B;
        break;
    default:
        break;
    }
    
    B->LeftSubTree = A_LeftSubTree;
    if(B->LeftSubTree != NULL) B->LeftSubTree->Parent = B;
    B->RightSubTree = A_RightSubTree;
    if(B->RightSubTree != NULL) B->RightSubTree->Parent = B;
    B->color = A_color;
    printf("%d,A :color = %s\r\n",A->Element, A->color == Red? "Red":"Black");
    printf("%d,B: color = %s\r\n",B->Element, B->color == Red? "Red":"Black");
}

/**
 * @brief 删除特定元素的节点
 * 
 * @param RootNode 整个树的根节点
 * @param Element 排序大小的值
 * @return Rb_Tree* 整个树的根节点
 */
Rb_Tree* Rb_Tree_DeleteNode(Rb_Tree* RootNode,int Element)
{
    Rb_Tree* DeletedNode = Rb_Tree_FindNode(RootNode, Element);
    Rb_Tree* DeletedNodeofMaxNode = NULL;
    Rb_Tree* DoubleBlackNode = NULL;
    Rb_Tree* TempRootNode = NULL;
    bool WholeTreeNullFlag = false;

    if(DeletedNode != NULL)
    {
        DeletedNodeofMaxNode = Rb_Tree_FindMaxNode(DeletedNode->LeftSubTree, Element);
    }else{
        printf(" NOT search target!\r\n");
        return RootNode;
    }
    printf("%d is run complete!, DeletedNodeofMaxNode = %p, element = %d\r\n",__LINE__, DeletedNodeofMaxNode, Element);
    if(DeletedNodeofMaxNode == NULL){
        /*被删除的左子树为空*/
        if(DeletedNode->RightSubTree == NULL){
            /*被删除的是叶子节点*/
            if(DeletedNode->color == Red){
                if(DeletedNode->Parent->color == Red) printf("wrong occur %d,please check!\r\n",__LINE__);
                Rb_Tree_RemoveNodeFromTree(DeletedNode);
            }else{
                /*先将待删除点定位双黑节点，调整完毕之后，再将其删除*/
                DoubleBlackNode = DeletedNode;
                TempRootNode = Rb_Tree_BalanceDoubleBlackNode(DoubleBlackNode);
                if(Rb_Tree_RemoveNodeFromTree(DeletedNode) == NULL){
                    WholeTreeNullFlag = true;
                }
            }
        }else{
            if(getNodeColor(DeletedNode->RightSubTree) != Red){
                printf(" %d, wrong color is set, please check code!\r\n", __LINE__);
            }else{
                DeletedNode->color = Red;
                DeletedNode->RightSubTree->color = Black;

                TempRootNode = Left_LeftRotate(DeletedNode);
                TempRootNode = Rb_Tree_RemoveNodeFromTree(TempRootNode->LeftSubTree);
            }
        }
    }else{
        if(DeletedNodeofMaxNode->LeftSubTree != NULL){
            if(DeletedNodeofMaxNode->LeftSubTree->color != Red) printf("%d, wrong color is setted\r\n", __LINE__);
            DeletedNodeofMaxNode->color = Red;
            DeletedNodeofMaxNode->LeftSubTree->color = Black;
            TempRootNode = Right_RightRotate(DeletedNodeofMaxNode);
            
            /*删除非根节点*/
            if(DeletedNode->Parent != NULL){
                SwapTwoNodeLocationWithColor(DeletedNode, TempRootNode->RightSubTree);
                Rb_Tree_RemoveNodeFromTree(DeletedNode);
            }else{
                TempRootNode = TempRootNode->RightSubTree;
                SwapTwoNodeLocationWithColor(DeletedNode, TempRootNode);
                Rb_Tree_RemoveNodeFromTree(DeletedNode);
            }
        }else{
            if(DeletedNodeofMaxNode->color ==Red){
                if(DeletedNode->Parent != NULL){
                    SwapTwoNodeLocationWithColor(DeletedNodeofMaxNode, DeletedNode);
                    Rb_Tree_RemoveNodeFromTree(DeletedNode);
                }else{
                    TempRootNode = DeletedNodeofMaxNode;
                    SwapTwoNodeLocationWithColor(DeletedNodeofMaxNode, DeletedNode);
                    Rb_Tree_RemoveNodeFromTree(DeletedNode);
                }
            }else{
                if(DeletedNode->Parent != NULL){
                    SwapTwoNodeLocationWithColor(DeletedNodeofMaxNode, DeletedNode);
                
                    /*先将待删除点定位双黑节点，调整完毕之后，再将其删除*/
                    DoubleBlackNode = DeletedNode;
                    
                    TempRootNode = Rb_Tree_BalanceDoubleBlackNode(DoubleBlackNode);
                    
                    Rb_Tree_RemoveNodeFromTree(DoubleBlackNode);
                }else{
                    Rb_Tree* TempRootNode1 = DeletedNodeofMaxNode;
                    SwapTwoNodeLocationWithColor(DeletedNodeofMaxNode, DeletedNode);
                
                    /*先将待删除点定位双黑节点，调整完毕之后，再将其删除*/
                    DoubleBlackNode = DeletedNode;
                    
                    TempRootNode = Rb_Tree_BalanceDoubleBlackNode(DoubleBlackNode);
                    
                    Rb_Tree_RemoveNodeFromTree(DoubleBlackNode);

                    /*如果双黑节点调整过程中没有出现换根节点的情况，那么就将待删除的节点设置为根节点*/
                    if(!(TempRootNode != NULL && TempRootNode->Parent == NULL)){
                        TempRootNode = TempRootNode1;
                    }
                }
            }
        }
    }

    if(TempRootNode != NULL && TempRootNode->Parent == NULL){
        RootNode = TempRootNode;
        printf("%d is run complete!\r\n",__LINE__);
    }
    if(WholeTreeNullFlag == false){
        RootNode->color = Black;
    }else{
        RootNode = NULL;
        printf("%d is run complete!\r\n",__LINE__);
    }
    return RootNode;
}

void Rb_TreeInorder(Rb_Tree* RootNode)
{
    if(RootNode != NULL){
        Rb_TreeInorder(RootNode->LeftSubTree);
        printf("%d,color = %s\r\n",RootNode->Element, RootNode->color == Red? "Red":"Black");
        printf("self = %p, left = %p, right = %p, \r\n",RootNode, RootNode->LeftSubTree, RootNode->RightSubTree);
        if(RootNode->LeftSubTree != NULL){
            printf("left is related: %s\r\n", RootNode->LeftSubTree->Parent == RootNode? "true":"error");
        }
        
        if(RootNode->RightSubTree != NULL){
            printf("Right is related: %s\r\n", RootNode->RightSubTree->Parent == RootNode? "true":"error");
        }

        Rb_TreeInorder(RootNode->RightSubTree);
    }else{
        return;
    }
}

void Rb_TreePreorder(Rb_Tree* RootNode)
{
    if(RootNode != NULL){
        printf("%d,color = %s\r\n",RootNode->Element, RootNode->color == Red? "Red":"Black");
        Rb_TreeInorder(RootNode->LeftSubTree);
        Rb_TreeInorder(RootNode->RightSubTree);
    }else{
        return;
    }
}
 

#include <time.h> 
void shuffle(int* arr, int len) { 
    // 使用time函数初始化随机数种子 
    srand((unsigned)time(NULL));  
    for (int i = len - 1; i > 0; i--) {  
        int j = rand() % (i + 1);  
        // 执行元素交换操作 
        int temp = arr[i]; 
        arr[i] = arr[j]; 
        arr[j] = temp; 
    }  
}

int pathBlackNodes(Rb_Tree *root) { 
  if (root == NULL) { 
    // Leaf nodes are considered black 
    return 1; 
  } 
  // We add one for this node in case if it is black 
  int black = root->color == Black ? 1 : 0; 
  int left = pathBlackNodes(root->LeftSubTree); 
  int right = pathBlackNodes(root->RightSubTree); 
   
  // Paths contain unequal number of black nodes 
  // or left and right subtrees are not valid RB trees 
  if (left == 0 || right == 0 || left != right) { 
    return 0; 
  } 
   
  // Paths contain equal number of black nodes 
  return left + black; 
} 
 
 
int isValidRBTree(Rb_Tree *root) { 
  // Property 1: nodes are either red or black 
  // Property 2: the root is black 
  if (getNodeColor(root) != Black) { 
    return 0; 
  } 
 
 
  // Property 3: all leaves (null) are black is already considered during insertion 
 
 
  // Property 4: red nodes have black children is checked during insertion 
   
  // Property 5: all paths from root to leaves contain the same number of black nodes 
  return pathBlackNodes(root) > 0; 
} 

int main()
{
    Rb_Tree* RootNode = NULL;
    int Data = 1;
    int num = 300;
    int array[300];
    for(int j = 0; j < 100; j++){
        for(int i = 0; i < num; i++){
            RootNode = Rb_Tree_InsertNode(RootNode,i, &Data, sizeof(Data));
            array[i] = i;
            // printf("xxxxxxxxxxx\r\n");
            // Rb_TreeInorder(RootNode);
            // printf("preorder\r\n");
            // Rb_TreePreorder(RootNode);
            // printf("xxxxxxxxxxx\r\n");
        }
        if(!isValidRBTree(RootNode)){
            printf("is not valid\r\n");
            exit(0);
        }
        shuffle(array, sizeof(array)/sizeof(int));

        // printf("xxxxxxxxxxx\r\n");
        //     Rb_TreeInorder(RootNode);
        //     // printf("preorder\r\n");
        //     // Rb_TreePreorder(RootNode);
        //     printf("xxxxxxxxxxx\r\n");
        // RootNode = Rb_Tree_DeleteNode(RootNode,24);

        // RootNode = Rb_Tree_DeleteNode(RootNode,23);
        // RootNode = Rb_Tree_DeleteNode(RootNode,22);
        // printf("xxxxxxxxxxx\r\n");
        //     Rb_TreeInorder(RootNode);
        //     printf("xxxxxxxxxxx\r\n");
        printf("xxxxxxxxxxx\r\n");
        Rb_TreeInorder(RootNode);
        // printf("preorder\r\n");
        // // Rb_TreePreorder(RootNode);
        printf("%d,xxxxxxxxxxx\r\n",__LINE__);

        for(int i = 0; i < num; i++){
            RootNode = Rb_Tree_DeleteNode(RootNode,  array[i]);
            if(!isValidRBTree(RootNode)){
                printf("is not valid\r\n");
                return -1;
            }
            printf("%d,xxxxxxxxxxx\r\n",__LINE__);
            Rb_TreeInorder(RootNode);
            // printf("preorder\r\n");
            // // Rb_TreePreorder(RootNode);
            printf("xxxxxxxxxxx\r\n");
        }
    }
    // printf("insert is complete!\r\n");
    // RootNode = Rb_Tree_DeleteNode(RootNode,6);
    // printf("xxxxxxxxxxx\r\n");
    //     Rb_TreeInorder(RootNode);
    //     printf("xxxxxxxxxxx\r\n");

    // RootNode = Rb_Tree_DeleteNode(RootNode,5);
    // printf("xxxxxxxxxxx\r\n");
    //     Rb_TreeInorder(RootNode);
    //     printf("xxxxxxxxxxx\r\n");

    // RootNode = Rb_Tree_DeleteNode(RootNode,4);
    // printf("xxxxxxxxxxx\r\n");
    //     Rb_TreeInorder(RootNode);
    //     printf("xxxxxxxxxxx\r\n");

    // RootNode = Rb_Tree_DeleteNode(RootNode,3);
    // printf("xxxxxxxxxxx\r\n");
    //     Rb_TreeInorder(RootNode);
    //     printf("xxxxxxxxxxx\r\n");

    // RootNode = Rb_Tree_DeleteNode(RootNode,2);
    // printf("xxxxxxxxxxx\r\n");
    //     Rb_TreeInorder(RootNode);
    //     printf("xxxxxxxxxxx\r\n");

    // RootNode = Rb_Tree_DeleteNode(RootNode,1);
    // printf("xxxxxxxxxxx\r\n");
    //     Rb_TreeInorder(RootNode);
    //     printf("xxxxxxxxxxx\r\n");

    // RootNode = Rb_Tree_DeleteNode(RootNode,0);
    // printf("xxxxxxxxxxx\r\n");
    //     Rb_TreeInorder(RootNode);
    //     printf("xxxxxxxxxxx\r\n");

    return 0;
}