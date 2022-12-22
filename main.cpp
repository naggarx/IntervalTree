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
        else if(interval->Low <= node->interval->Low)
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
        return node;
    }
    Node* DeleteNode(Interval* interval)
    {
        if(Root== nullptr)
        {
          return Root;
        }
            Node* curr = Root;
            Node* prev = nullptr;

            while (curr != nullptr &&( (curr->interval->Low !=interval->Low) &&(curr->interval->High!=interval->High) ))
            {
                cout << "curr interval [ " << curr->interval->Low << ","<<curr->interval->High<<" ] "<<endl;
                prev = curr;
                if (interval->Low <= curr->interval->Low)
                {
                    curr = curr->Left;
                }
                else
                {
                    curr = curr->Right;
                   // cout << "curr interval [ " << curr->interval->Low << ","<<curr->interval->High<<" ] "<<endl;
                }
            }
            if (curr == nullptr)
            {
                cout << "interval [ " << interval->Low << ","<<interval->High<<" ] was not found "<<endl;
                return Root;
            }

            if(curr->Left == nullptr || curr->Right == nullptr)
            { Node* Ncurr;
                if (curr->Left == nullptr)
                {
                    Ncurr = curr->Right;
                }
                else
                {
                    Ncurr = curr->Left;
                }
                if (prev == nullptr)
                {
                    return Ncurr;
                }

                if (curr == prev->Left)
                {
                    prev->Left = Ncurr;
                }
                else
                {
                    prev->Right = Ncurr;
                }
                free(curr);
            }
            else {
                Node* prev2 = nullptr;
                Node* temp;

                temp = curr->Right;
                while (temp->Left != nullptr)
                {
                    prev2 = temp;
                    temp = temp->Left;
                }
                if (prev2 != nullptr)
                {
                    prev2->Left = temp->Right;
                }
                else
                {
                    curr->Right = temp->Right;
                }

                curr->interval->Low = temp->interval->Low;
                curr->interval->High = temp->interval->High;
                free(temp);
            }
        return Root;

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
    t1.DeleteNode(i);
    cout<<t1.Root->interval->Low<<endl;
    return 0;
}
