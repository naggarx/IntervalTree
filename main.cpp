#include <iostream>
#include <cstdlib>
using namespace std;
//class Interval that has low,high
class Interval{
public:
    int Low;
    int  High;
    //Default Constructor initialize low,high with zero
    Interval()
    {
        Low = 0;
        High = 0;
    }
    // Parameterized Constructor to set low and high
    Interval(int l,int h)
    {
        Low = l;
        High = h;
    }
    bool operator == (Interval const &obj) const
    {
        return (obj.High == High && obj.Low == Low);
    }

};
//class node have Interval,left,right and max
class Node{
public:
    Interval interval;
    Node* Left;
    Node* Right;
    int Max;
    //Default Constructor that set parameters
    Node()
    {
        interval.High=0;
        interval.Low=0;
        Left = nullptr;
        Right = nullptr;
        Max = 0;
    }
    // Parameterized Constructor to set Interval
    Node(Interval interval1)
    {
        interval = interval1;
        Left = nullptr;
        Right = nullptr;
        Max = 0;
    }


};
// class Interval tree have InsertInterval(),DeletInterval(),SearchInterval() functions
class IntervalTree{
public:
    Node* Root;
    //Default Constructor set root to nullptr
    IntervalTree()
    {
        Root = nullptr;
    }
    // Parameterized Constructor to set root of the tree
    IntervalTree(Interval interval)
    {
        Root = new Node(interval);
    }
    // InsertInterval function that insert interval in Tree
    void InsertInterval(Interval interval)
    {

        //if Tree is empty,insert the interval as root
        if(Root == nullptr)
        {
            Root = new Node(interval);
            Root->Max=interval.High;
        }
        //if the Tree not empty,insert interval node using insertByRec()
        else
        {
            InsertByRec(Root,interval);
        }
    }
    //function that takes an interval and root node then find its insertion place in the tree to insert it
    Node* InsertByRec(Node* node,Interval interval)
    {
        //if node is empty , insert a new node with the given interval and set its Max
        if(node == nullptr)
        {
            Node* n=new Node(interval);
            n->Max=interval.High;
            return n;
        }
        // if interval low is <= current node interval low ,then recurse over the left of the node
        else if(interval.Low <= node->interval.Low)
        {
            node->Left = InsertByRec(node->Left,interval);
        }
        //else recurse over node Right
        else
        {
            node->Right = InsertByRec(node->Right,interval);
        }
        //set max of each inserted node
        if(node->Max < interval.High)
        {
            node->Max = interval.High;
        }
        return node;
    }
    // Function that recurse over the tree to adjust the node Max after deletion
    void EditMax(Node* curr)
    {

        //if curr = null
        if (curr== nullptr)
        {
            return;
        }
        //recurse over the tree using post order dfs
        EditMax(curr->Left);
        EditMax(curr->Right);
        //if curr has no children max equal interval high
        if((curr->Left==nullptr) && (curr->Right==nullptr))
        {
           curr->Max=curr->interval.High;
        }
        //if curr has no left child,max equal max of interval high and right node max
        else if(curr->Left==nullptr )
        {
            curr->Max=max(curr->interval.High,curr->Right->Max);
        }
        //if curr has no right child max equal max of interval high and left node max
        else if(curr->Right==nullptr)
        {
            curr->Max=max(curr->interval.High,curr->Left->Max);
        }
        //if curr has both children max equal max of interval high and left node max and right node max
        else if((curr->Left!=nullptr) &&(curr->Right!=nullptr))
        {
            curr->Max=max(curr->interval.High,max(curr->Left->Max,curr->Right->Max));

        }

    }
    // DeleteInterval function delete a node by its interval
    void DeleteInterval(Interval interval)
    {

        //the tree has only root
        if(Root->interval == interval && Root->Left == nullptr && Root->Right == nullptr)
        {
            Node* curr = Root;
            Root = nullptr;
            free(curr);
            return;
        }

        Node* curr = Root;
        Node* prev = nullptr;//hold to be deleted node parent
        //Search the tree to find the node with the interval to be deleted
        while (curr != nullptr)
        {
            if(curr->interval == interval)
                break;
            prev = curr;
            if (interval.Low <= curr->interval.Low)
            {
                curr = curr->Left;
            }
            else
            {
                curr = curr->Right;
            }
        }
        // if node not found return
        if (curr == nullptr)
        {
                cout << "interval [ " << interval.Low << ","<<interval.High<<" ] was not found "<<endl;
                return;
        }
        //if node has no children
        if(curr->Left == nullptr && curr->Right == nullptr )
        {
            //if node is root
            if(prev == nullptr)
            {
                Root = nullptr;
                free(curr);
                return;
            }
            //if curr was on left
            if (curr == prev->Left)
            {
                prev->Left = nullptr;
            }
            //if curr was on right
            else
            {
                prev->Right = nullptr;
            }
            EditMax(Root);
            free(curr);
        }
        //if node has one child
        else if((curr->Left == nullptr && curr->Right != nullptr ) || (curr->Left != nullptr && curr->Right == nullptr ))
        {
                Node* Ncurr;// hold child of node to be deleted
                //if left child =null
                if (curr->Left == nullptr)
                {
                    Ncurr = curr->Right;
                }
                //if right child =null
                else
                {
                    Ncurr = curr->Left;
                }
                //if node = root
                if(prev == nullptr)
                {
                    Root = Ncurr;
                    EditMax(Root);
                    free(curr);
                    return;
                }
                //if curr was on left
                if (curr == prev->Left)
                {
                    prev->Left = Ncurr;
                }
                //if curr was on right
                else
                {
                    prev->Right = Ncurr;
                }
                free(curr);
        }
        //if node has both children
        else
        {
                Node* prev2 = nullptr;// hold parent of node
                Node* temp;// temp hold node right
                temp = curr->Right;
                //get the left most of the node  and put it in temp
                while (temp->Left != nullptr)
                {
                    prev2 = temp;
                    temp = temp->Left;
                }
                //if prev2=null
                if (prev2 != nullptr)
                {
                    prev2->Left = temp->Right;
                }
                // else curr right = temp right
                else
                {
                    curr->Right = temp->Right;
                }
                curr->interval.Low = temp->interval.Low;
                curr->interval.High = temp->interval.High;
                free(temp);
            }
        // edit max of tree after deletion
        EditMax(Root);
    }
    //overlap function checks if 2 intervals overlap and return bool
    static bool overlap(Interval it1,Interval it2)
    {
        // if overlap return true
        if((it1.Low<=it2.High) && (it2.Low<=it1.High))
        {
            return true;
        }
        //else false
        return false;
    }
    //Print function prints array of intervals
    static void Print(Interval it[],int c)
    {
        for(int i=0;i<c;i++)
        {
            cout<<"[ "<< it[i].Low<<","<<it[i].High<<" ]"<<endl;
        }

    }
    //search function that take interval and returns the first interval overlapping with it
    Interval Search( Node* node,Interval interval)
    {

        //if node = null return
        if(node == nullptr)
        {
            Interval int1;
            return int1;
        }
        // if overlap return interval
        if(overlap(interval,node->interval))
        {
            return node->interval;
        }
        // if interval low >node left max search in node right
        if(node->Left != nullptr && interval.Low <= node->Left->Max )
        {
            return Search(node->Left, interval);
        }
        // else search in node left
        return Search(node->Right,interval);
    }
    //search function that take interval and prints all intervals overlapping with it
    void SearchInterval(Interval it)
    {
        Interval overlaped[1000];
        int c=0;
        Interval found= Search(Root,it);
            //while interval found not equal null put the matching interval in array of intervals
            // delete the interval and search for other intervals
            while( found.Low != 0 && found.High != 0)
            {
                overlaped[c] = found;
                DeleteInterval(found);
                c++;
                found=Search(Root, it);
            }
        //print overlaped intervals
        Print(overlaped,c);
        //insert all the deleted intervals again
        for(int i=0;i<c;i++)
        {
            InsertInterval(overlaped[i]);
        }
    }
};
int main()
{
    Interval ii(15,20);
    IntervalTree t1(ii);
    Interval ii1(17,19);
    t1.InsertInterval(ii1);
    Interval ii2 (30,40);
    t1.InsertInterval(ii2);
    Interval ii3 (10,30);
    t1.InsertInterval(ii3);
    Interval ii4 (5,20);
    t1.InsertInterval(ii4);
    Interval ii5 (12,15);
    t1.InsertInterval(ii5);
    Interval ii6 (18,120);
    t1.InsertInterval(ii6);
    Interval ii7 (16,20);
    t1.InsertInterval(ii7);
    Interval ii8 (9,50);
    t1.InsertInterval(ii8);

    cout<<"1"<<endl;
    cout<<"Overlap ["<< ii.Low<<","<<ii.High<<"]"<<endl;
    t1.SearchInterval(ii);
    cout<<"-------"<<endl;

    cout<<"2"<<endl;
    cout<<"Overlap ["<< ii1.Low<<","<<ii1.High<<"]"<<endl;
    t1.SearchInterval(ii1);
    cout<<"-------"<<endl;

    cout<<"3"<<endl;
    cout<<"Overlap ["<< ii2.Low<<","<<ii2.High<<" ]"<<endl;
    t1.SearchInterval(ii2);
    cout<<"-------"<<endl;

    cout<<"4"<<endl;
    cout<<"Overlap ["<< ii3.Low<<","<<ii3.High<<"]"<<endl;
    t1.SearchInterval(ii3);
    cout<<"-------"<<endl;

    cout<<"5"<<endl;
    cout<<"Overlap ["<< ii4.Low<<","<<ii4.High<<"]"<<endl;
    t1.SearchInterval(ii4);
    cout<<"-------"<<endl;

    cout<<"6"<<endl;
    cout<<"Overlap ["<< ii5.Low<<","<<ii5.High<<"]"<<endl;
    t1.SearchInterval(ii5);
    cout<<"-------"<<endl;

    cout<<"7"<<endl;
    cout<<"Overlap ["<< ii6.Low<<","<<ii6.High<<"]"<<endl;
    t1.SearchInterval(ii6);
    cout<<"-------"<<endl;

    cout<<"8"<<endl;
    cout<<"Overlap ["<< ii7.Low<<","<<ii7.High<<"]"<<endl;
    t1.SearchInterval(ii7);
    cout<<"-------"<<endl;

    cout<<"9"<<endl;
    cout<<"Overlap ["<< ii8.Low<<","<<ii8.High<<"]"<<endl;
    t1.SearchInterval(ii8);
    cout<<"-------"<<endl;

    cout<<"10"<<endl;
    Interval ii9 (2,300);
    cout<<"Overlap ["<< ii9.Low<<","<<ii9.High<<"]"<<endl;
    cout<<"-------"<<endl;

    cout<<"11"<<endl;
    t1.DeleteInterval(ii6);
    cout<<"Root Max :"<<endl;
    cout<<t1.Root->Max<<endl;

    cout<<"12"<<endl;
    t1.DeleteInterval(ii8);
    cout<<"Root Max :"<<endl;
    cout<<t1.Root->Max<<endl;

    cout<<"13"<<endl;
    t1.DeleteInterval(ii2);
    cout<<"Root Max :"<<endl;
    cout<<t1.Root->Max<<endl;

    cout<<"14"<<endl;
    t1.DeleteInterval(ii3);
    cout<<"Root Max :"<<endl;
    cout<<t1.Root->Max<<endl;

    cout<<"15"<<endl;
    t1.DeleteInterval(ii7);
    cout<<"Root Max :"<<endl;
    cout<<t1.Root->Max<<endl;

    Interval iii(19,22);
    IntervalTree t2(iii);
    Interval iii1(10,11);
    t2.InsertInterval(iii1);
    Interval iii2 (27,350);
    t2.InsertInterval(iii2);
    Interval iii3 (7,10);
    t2.InsertInterval(iii3);
    Interval iii4 (15,21);
    t2.InsertInterval(iii4);
    Interval iii5 (20,21);
    t2.InsertInterval(iii5);
    Interval iii6 (25,30);
    t2.InsertInterval(iii6);
    Interval iii7 (2,4);
    t2.InsertInterval(iii7);
    Interval iii8 (8,13);
    t2.InsertInterval(iii8);

    cout<<"16"<<endl;
    cout<<"Overlap ["<< iii.Low<<","<<iii.High<<"]"<<endl;
    t2.SearchInterval(iii);
    cout<<"-------"<<endl;

    cout<<"17"<<endl;
    cout<<"Overlap ["<< iii1.Low<<","<<iii1.High<<"]"<<endl;
    t2.SearchInterval(iii1);
    cout<<"-------"<<endl;

    cout<<"18"<<endl;
    cout<<"Overlap ["<< iii2.Low<<","<<iii2.High<<"]"<<endl;
    t2.SearchInterval(iii2);
    cout<<"-------"<<endl;

    cout<<"19"<<endl;
    cout<<"Overlap ["<< iii3.Low<<","<<iii3.High<<"]"<<endl;
    t2.SearchInterval(iii3);
    cout<<"-------"<<endl;

    cout<<"20"<<endl;
    cout<<"Overlap ["<< iii4.Low<<","<<iii4.High<<"]"<<endl;
    t2.SearchInterval(iii4);
    cout<<"-------"<<endl;

    cout<<"21"<<endl;
    cout<<"Overlap ["<< iii5.Low<<","<<iii5.High<<"]"<<endl;
    t2.SearchInterval(iii5);
    cout<<"-------"<<endl;

    cout<<"22"<<endl;
    cout<<"Overlap ["<< iii6.Low<<","<<iii6.High<<"]"<<endl;
    t2.SearchInterval(iii6);
    cout<<"-------"<<endl;

    cout<<"23"<<endl;
    cout<<"Overlap["<< iii7.Low<<","<<iii7.High<<"]"<<endl;
    t2.SearchInterval(iii7);
    cout<<"-------"<<endl;

    cout<<"24"<<endl;
    cout<<"Overlap ["<< iii8.Low<<","<<iii8.High<<"]"<<endl;
    t2.SearchInterval(iii8);
    cout<<"-------"<<endl;

    cout<<"25"<<endl;
    Interval iii9 (2,300);
    cout<<"Overlap ["<< iii9.Low<<","<<iii9.High<<"]"<<endl;
    t2.SearchInterval(iii9);
    cout<<"-------"<<endl;

    cout<<"26"<<endl;
    t2.DeleteInterval(iii2);
    cout<<"Root Max :"<<endl;
    cout<<t2.Root->Max<<endl;

    cout<<"27"<<endl;
    t2.DeleteInterval(iii6);
    cout<<"Root Max :"<<endl;
    cout<<t2.Root->Max<<endl;

    cout<<"28"<<endl;
    t2.DeleteInterval(iii);
    cout<<"Root Max :"<<endl;
    cout<<t2.Root->Max<<endl;

    cout<<"29"<<endl;
    t2.DeleteInterval(iii4);
    cout<<"Root Max :"<<endl;
    cout<<t2.Root->Max<<endl;

    cout<<"30"<<endl;
    t2.DeleteInterval(iii9);
    cout<<"Root Max :"<<endl;
    cout<<t2.Root->Max<<endl;
   return 0;
}
