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
    void EditMax(Node* curr)
    {

        //Node* prev = nullptr;
        if (curr==nullptr)
        {
            return;
        }
        EditMax(curr->Left);
        EditMax(curr->Right);
        if((curr->Left==nullptr) && (curr->Right==nullptr))
        {
           curr->Max=curr->interval->High;

        }
        else if(curr->Left==nullptr )
        {
            curr->Max=max(curr->interval->High,curr->Right->Max);
        }
        else if(curr->Right==nullptr)
        {
            curr->Max=max(curr->interval->High,curr->Left->Max);
        }
        else if((curr->Left!=nullptr) &&(curr->Right!=nullptr))
        {
            curr->Max=max(curr->interval->High,max(curr->Left->Max,curr->Right->Max));

        }

    }

    void DeleteInterval(Interval* interval)
    {
        if(Root->interval == interval && Root->Left == nullptr && Root->Right == nullptr)
        {
            Node* curr = Root;
            Root = nullptr;
            free(curr);
            return;
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
                return;
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
                if(curr == Root)
                {
                    Root = Ncurr;
                    EditMax(Root);
                    free(curr);
                    return;
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
        else
        {
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

    }
    bool overlap(Interval* it1,Interval* it2)
    {
        if((it1->Low<=it2->High) && (it2->Low<=it1->High))
        {
            return true;
        }
        return false;
    }
    void Print(Interval it[],int c)
    {
        for(int i=0;i<c;i++)
        {
            cout<<"[ "<< it[i].Low<<","<<it[i].High<<" ]"<<endl;
        }

    }
    Interval* Search( Node* node,Interval* interval)
    {
        if(node == nullptr)
        {
            return nullptr;
        }
        if(overlap(interval,node->interval))
        {
            return node->interval;
        }
        if(interval->Low > node->Left->Max)
        {
            return Search(node->Right, interval);
        }
        return Search(node->Left,interval);

    }
    void SearchInterval(Interval* it)
    {
            Interval overlaped[1000];
            int c=0;
            Interval* found= Search(Root,it);
            while( found != nullptr)
            {
                overlaped[c] = *found;
                DeleteInterval(found);
                c++;
                found=Search(Root, it);
            }
        Print(overlaped,c);
        for(int i=0;i<c;i++)
        {
            auto* interval = new Interval(overlaped[i].Low,overlaped[i].High);
            InsertInterval(interval);
        }


    }



};
int main()
{

    auto* ii = new Interval(5,300);
    auto* i = new Interval(15,20);
    IntervalTree t1(i);
    auto* i2 = new Interval(17,19);
    t1.InsertInterval(i2);
    auto* i3 = new Interval(30,40);
    t1.InsertInterval(i3);
    auto* i4 = new Interval(10,30);
    t1.InsertInterval(i4);
    auto* i5 = new Interval(5,20);
    t1.InsertInterval(i5);
    auto* i6 = new Interval(12,15);
    t1.InsertInterval(i6);
    auto* i7 = new Interval(18,120);
    t1.InsertInterval(i7);
    auto* i8 = new Interval(16,20);
    t1.InsertInterval(i8);
    t1.SearchInterval(ii);
    cout<<"--------------------"<<endl;
    t1.SearchInterval(ii);
    cout<<"--------------------"<<endl;
    t1.SearchInterval(i2);





    return 0;
}
