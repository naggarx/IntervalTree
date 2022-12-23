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
            Node* n=new Node(interval);
            n->Max=interval->High;
            return n;
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
    Node* EditMax(Node* curr)
    {


        //Node* prev = nullptr;
        if (curr==nullptr)
        {
            return curr;
        }
        EditMax(curr->Left);
        EditMax(curr->Right);

        if((curr->Left==nullptr) &&(curr->Right==nullptr))
        {
           curr->Max=curr->interval->High;


        }

       else if((curr->Left==nullptr) )
        {
            curr->Max=max(curr->interval->High,curr->Left->Max);
        }
        else if((curr->Right==nullptr))
        {
            curr->Max=max(curr->interval->High,curr->Right->Max);
        }
        else if((curr->Left!=nullptr) &&(curr->Right!=nullptr))
        {
            curr->Max=max(curr->interval->High,max(curr->Left->Max,curr->Right->Max));

        }

    }

    Node* DeleteInterval(Interval* interval)
    {
        if(Root== nullptr)
        {
          return Root;
        }
            Node* curr = Root;
            Node* prev = nullptr;

            while (curr != nullptr &&( (curr->interval->Low !=interval->Low) &&(curr->interval->High!=interval->High) ))
            {
                prev = curr;
                if (interval->Low <= curr->interval->Low)
                {
                    curr = curr->Left;
                }
                else
                {
                    curr = curr->Right;

                }
            }
            if (curr == nullptr)
            {
                cout << "interval [ " << interval->Low << ","<<interval->High<<" ] was not found "<<endl;
                return Root;
            }
            if(curr->Left == nullptr || curr->Right == nullptr)
            {
                Node* Ncurr;
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
                    Ncurr->Max=max(Ncurr->interval->High,max(Ncurr->Left->Max,Ncurr->Right->Max));
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
                    // to be changed to temp;
                }
                curr->interval->Low = temp->interval->Low;
                curr->interval->High = temp->interval->High;
                free(temp);
            }
        EditMax(Root);
        return Root;

    }
    Node* SearchInterval(Interval* interval)
    {

    }


};
int main()
{
    Interval* i = new Interval(15,20);
    IntervalTree t1(i);
    Interval* i2 = new Interval(17,19);
    t1.InsertInterval(i2);
    Interval* i3 = new Interval(30,40);
    t1.InsertInterval(i3);
    Interval* i4 = new Interval(10,30);
    t1.InsertInterval(i4);
    Interval* i5 = new Interval(5,20);
    t1.InsertInterval(i5);
    Interval* i6 = new Interval(12,15);
    t1.InsertInterval(i6);
    Interval* i7 = new Interval(18,12);
    t1.InsertInterval(i7);
    Interval* i8 = new Interval(16,20);
    t1.InsertInterval(i8);
    t1.DeleteInterval(i3);
    cout<<t1.Root->Max;
    return 0;
}
