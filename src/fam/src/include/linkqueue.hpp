/*
 * 基于链表的队列实现
 * Author:SHIELD_QI
 * Date:2018-06-07
 * ********************************************/
#ifndef LINKQUEUE_H
#define LINKQUEUE_H
#include <iostream>
using namespace std;

//---------------------创建节点类-----------------------/
template <typename T>
class Node
{
public:
    Node(T t):value(t),next(nullptr){}
    Node()=default;
    T value;
    Node<T>* next;

};
//------------------构建队列ADT数据类-------------------/
template <typename T>
class LinkQueue
{
public:
    LinkQueue();
    ~LinkQueue();
public:
    bool isEmpty();         //判断是否为空
    int getSize();          //获取队列大小
    bool pop();             //出队列
    void push(T t);         //进队列
    T getFront();           //获取队列头元素
    T getRear();            //获取队列尾元素
    void QueueTravers();    //遍历队列
    void DestoryQueue();    //销毁队列
    void ClearQueue();      //清空队列

    void operator=(LinkQueue<T> t);

private:
    int count;              //队列元素个数
    Node<T>* phead;         //队列头
    Node<T>* pend;          //队列尾
};
//-----------------重载 =------------------/
template <typename T>
void LinkQueue<T>::operator=(LinkQueue<T> t)
{
    Node<T>* pNode=new Node<T>();
    pNode=t.phead->next;

    while(pNode!=nullptr)
    {
        this->push(pNode->value);
        pNode=pNode->next;
    }
}

//----------------构造函数-----------------/
template <typename T>
LinkQueue<T>::LinkQueue():phead(nullptr),pend(nullptr),count(0)
{
    phead=new Node<T>();
    pend=phead;
    count=0;
}
//----------------析构函数-----------------/
template <typename T>
LinkQueue<T>::~LinkQueue()
{
    while(phead->next!=nullptr)
    {
        Node<T>* pNode=phead->next;
        phead->next=pNode->next;
        delete pNode;
    }
}
//---------------判断是否为空---------------/
template <typename T>
bool LinkQueue<T>::isEmpty()
{
    return count==0;
}
//---------------获取队列大小---------------/
template <typename T>
int LinkQueue<T>::getSize()
{
    return count;
}
//------------进队列：从队尾插入-------------/
template <typename T>
void LinkQueue<T>::push(T t)
{
    Node<T>* pNode=new Node<T>(t);
    pend->next=pNode;
    pend=pNode;
    count++;
}
//------------出队列：从队首弹出-------------/
template <typename T>
bool LinkQueue<T>::pop()
{
    if(count==0)
        return false;
    Node<T>* pNode=phead->next;
    phead->next=phead->next->next;
    delete pNode;
    count--;
    return true;
}
//-------------获取队首元素---------------/
template <typename T>
T LinkQueue<T>::getFront()
{
    return phead->next->value;
}
//-------------获取队尾元素---------------/
template <typename T>
T LinkQueue<T>::getRear()
{
    return pend->value;
}
//---------------遍历队列---------------/
template <typename T>
void LinkQueue<T>::QueueTravers()
{
    setlocale(LC_CTYPE, "zh_CN.utf8");
    setlocale(LC_ALL, "");
    Node<T>* pNode=new Node<T>();
    pNode=phead->next;
    while(pNode!=nullptr)
    {
        switch(pNode->value.index)
        {
        case 0:
            ROS_INFO("一横");
            break;
        case 1:
            ROS_INFO("一竖");
            break;
        case 2:
            ROS_INFO("左竖弯");
            break;
        case 3:
            ROS_INFO("右竖弯");
            break;
        case 4:
            ROS_INFO("一撇");
            break;
        case 5:
            ROS_INFO("一捺");
            break;
        }
        //std::cout<<pNode->value<<endl;
        pNode=pNode->next;
    }
}
//---------------销毁队列---------------/
template <typename T>
void LinkQueue<T>::DestoryQueue()
{
    while(phead->next!=nullptr)
    {
        Node<T>* pNode=phead->next;
        phead->next=phead->next->next;
        delete pNode;
    }
    phead=nullptr;
}
//---------------清空队列---------------/
template <typename T>
void LinkQueue<T>::ClearQueue()
{
    while(phead->next!=nullptr)
    {
        Node<T>* pNode=phead->next;
        phead->next=phead->next->next;
        delete pNode;
    }
    pend = phead;
}

#endif // LINKQUEUE_H
