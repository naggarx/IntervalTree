#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <cstring>
using namespace std;
class Interval{
public:
    int Low;
    int  High;
    Interval()
    {
        Low = 0;
        High = 0;
    }
    Interval(int l,int h)
    {
        Low = l;
        High = h;
    }
    void SetLow(int l)
    {
        Low = l;
    }
    void SetHigh(int H)
    {
        High = H;
    }
};
class Node{
public:
    Interval* interval;
    Node* Left;
    Node* Right;
    int Max;
    Node()
    {
        interval = nullptr;
        Left = nullptr;
        Right = nullptr;
        Max = 0;
    }
    Node(Interval* interval1)
    {
        interval = interval1;
        Left = nullptr;
        Right = nullptr;
        Max = 0;
    }


};
class IntervalTree{
public:
    Node* Root;
    IntervalTree()
    {
        Root = nullptr;
    }
    IntervalTree(Interval* interval)
    {
        Root = new Node(interval);
    }
    void InsertInterval(Interval* interval)
    {
        if(Root == nullptr)
        {
            Root = new Node(interval);
            Root->Max=interval->High;
        }
        else
        {
            InsertByRec(Root,interval);
        }
    }
    Node* InsertByRec(Node* node,Interval* interval)
    {
        if(node == nullptr)
        {
           return new Node(interval);
        }
        else if(interval->Low < node->interval->Low)
        {
            node->Left = InsertByRec(node->Left,interval);
        }
        else
        {
            node->Right = InsertByRec(node->Right,interval);
        }
        if(node->Max < interval->High)
        {
            node->Max = interval->High;
        }
        return nullptr;
    }

};
int main()
{
    Interval* i = new Interval(15,20);
    IntervalTree t1(i);
    Interval* i2 = new Interval(12,95);
    t1.InsertInterval(i2);
    Interval* i3 = new Interval(25,105);
    t1.InsertInterval(i3);
    Interval* i4 = new Interval(10,120);
    t1.InsertInterval(i4);
    cout<<t1.Root->Max;
    return 0;
}
