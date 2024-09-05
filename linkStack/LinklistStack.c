/*
 * @Date: 2023-11-21 16:22:43
 * @LastEditors: yanglei67
 * @LastEditTime: 2023-11-21 16:50:40
 * @Description: Do not edit
 */
#include <linux/kthread.h>
#include <linux/gfp.h>
#include <linux/slab.h>

#include "LinklistStack.h"



/**
 * @brief 初始化栈顶指针
 * @return 空指针
 */
StackData_t* InitStack(void)
{
    return NULL;
}

/**
 * @brief 将目标元素压栈
 * @param {StackData_t*} StackTop栈顶指针
 * @param {void*} Data 需要压栈的元素地址
 * @param {size_t} DataSize 压栈元素的大小
 * @return {*} 新的栈顶指针
 */
StackData_t* PushBackToStack(StackData_t* StackTop, void* Data, size_t DataSize)
{
    StackData_t* LastTop = StackTop;
    kzalloc(DataSize,GFP_KERNEL);
    StackTop = kzalloc(sizeof(StackData_t),GFP_KERNEL);
    StackTop->Data = kzalloc(DataSize,GFP_KERNEL);
    memcpy(StackTop->Data, Data, DataSize);
    StackTop->Next = LastTop;
    return StackTop;
}

/**
 * @brief 判断栈是否为空 
 * @param {StackData_t*} StackTop栈顶指针
 * @return {*}true表示为空栈，否则不为空栈
 */
bool JudgeStackEmpty(StackData_t* StackTop)
{
    if(!StackTop){
        return true;
    }else{
        return false;
    }
}

/**
 * @brief 从栈顶出栈 
 * @param {StackData_t*} StackTop栈顶指针
 * @return {*}新的栈顶指针
 */
StackData_t* PopFromStack(StackData_t* StackTop, void*Data_Addr)
{
    if(JudgeStackEmpty(StackTop)){
        *((char**)Data_Addr) = NULL;
        return StackTop;
    }
    StackData_t* PopElement = StackTop;
    StackTop = PopElement->Next;
    *((char**)Data_Addr) = PopElement->Data;
    kfree(PopElement);
    return StackTop;
}

/**
 * @brief 清空栈的内容 
 * @param {StackData_t *} StackTop 栈顶指针
 * @return {*}空指针
 */
StackData_t* ClearStack(StackData_t * StackTop)
{
    StackData_t * NextTop;
    while (StackTop)
    {
        NextTop = StackTop->Next;
        if(StackTop->Data) kfree(StackTop->Data);
        kfree(StackTop);
        StackTop = NextTop;
    }
    return NULL;
}