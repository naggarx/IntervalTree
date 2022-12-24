#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <cstring>
using namespace std;
//class Interval that has low and high
class Interval{
public:
    int Low;
    int  High;
    //Default Constructor that initialize low and high with zero
    Interval()
    {
        Low = 0;
        High = 0;
    }
    // Parameterized Constructor to set the value of low and high
    Interval(int l,int h)
    {
        Low = l;
        High = h;
    }

};
//class node that have Interval , left  , right and max
class Node{
public:
    Interval* interval;
    Node* Left;
    Node* Right;
    int Max;
    //Default Constructor that set parameters to nullptr and max to zero
    Node()
    {
        interval=nullptr;
        Left = nullptr;
        Right = nullptr;
        Max = 0;
    }
    // Parameterized Constructor to set Interval , left  , right and max
    Node(Interval* interval1)
    {
        interval = interval1;
        Left = nullptr;
        Right = nullptr;
        Max = 0;
    }


};
// class Interval tree that have  InsertInterval() , DeletInterval() , SearchInterval() functions
class IntervalTree{
public:
    Node* Root;
    //Default Constructor that set root to nullptr
    IntervalTree()
    {
        Root = nullptr;
    }
    // Parameterized Constructor to set root of the tree
    IntervalTree(Interval* interval)
    {
        Root = new Node(interval);
       // Root->Max=Root->interval->High;
    }
    // InsertInterval function that take interval and insert it in Tree as interval node
    void InsertInterval(Interval* interval)
    {

        //if Tree is empty , insert the interval as root and set its max to equal interval high
        if(Root == nullptr)
        {
            Root = new Node(interval);
            Root->Max=interval->High;
        }
        // if the Tree is not empty , insert interval node using insertByRec() function
        else
        {
            InsertByRec(Root,interval);
        }
    }
    // function that takes an interval and root node then find its insertion place in the tree and insert it
    Node* InsertByRec(Node* node,Interval* interval)
    {
        //if node is empty , insert a new node with the given interval and set its Max
        if(node == nullptr)
        {
            Node* n=new Node(interval);
            n->Max=interval->High;
            return n;
        }
        // if interval low is less than or equal current node interval low ,then recurse over the left of the node
        else if(interval->Low <= node->interval->Low)
        {
            node->Left = InsertByRec(node->Left,interval);
        }
        // else recurse over the Righh of the node
        else
        {
            node->Right = InsertByRec(node->Right,interval);
        }
        // set max of each inserted node
        if(node->Max < interval->High)
        {
            node->Max = interval->High;
        }
        return node;
    }
    // Function that recurse over the tree to adjust the node Max after deletion
    void EditMax(Node* curr)
    {

        //if curr = null or curr == specified node return
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
           curr->Max=curr->interval->High;

        }
        //if curr has no left child max equal max between interval high and right node max
        else if(curr->Left==nullptr )
        {
            curr->Max=max(curr->interval->High,curr->Right->Max);
        }
        //if curr has no right child max equal max between interval high and left node max
        else if(curr->Right==nullptr)
        {
            curr->Max=max(curr->interval->High,curr->Left->Max);
        }
        //if curr has both children max equal max between interval high and left node max and right node max
        else if((curr->Left!=nullptr) &&(curr->Right!=nullptr))
        {
            curr->Max=max(curr->interval->High,max(curr->Left->Max,curr->Right->Max));

        }

    }
    // DeleteInterval function delete a node by its interval
    void DeleteInterval(Interval* interval)
    {
        cout<<"delete [ "<< interval->Low<<","<<interval->High<<" ]"<<endl;
        //the tree has only root and its to be deleted
        if(Root->interval == interval && Root->Left == nullptr && Root->Right == nullptr)
        {

            Node* curr = Root;
            Root = nullptr;
            free(curr);
            return;
        }

        Node* curr = Root;
        Node* prev = nullptr;//hold to be deleted node parent
        //Search the tree to find the node with the interval to be deleted starting from the root
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
        // if node not found return
        if (curr == nullptr)
        {
                cout << "interval [ " << interval->Low << ","<<interval->High<<" ] was not found "<<endl;
                return;
        }
        //if node has one or no children
        if(curr->Left == nullptr || curr->Right == nullptr)
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
                //if prev2=null ,node to be deleted is root new root = temp right
                if (prev2 != nullptr)
                {
                    prev2->Left = temp->Right;
                }
                // else curr right = temp right
                else
                {
                    curr->Right = temp->Right;

                }
                curr->interval->Low = temp->interval->Low;
                curr->interval->High = temp->interval->High;
                free(temp);
            }
        // edit max of tree after deletion
        EditMax(Root);

    }
    //overlap function checks if 2 intervals overlap and return bool
    bool overlap(Interval* it1,Interval* it2)
    {
        // if overlap return true
        if((it1->Low<=it2->High) && (it2->Low<=it1->High))
        {
            return true;
        }
        //else false
        return false;
    }
    //Print function prints array of all intervals overlapping with a given interval
    void Print(Interval it[],int c)
    {
        for(int i=0;i<c;i++)
        {
            cout<<"[ "<< it[i].Low<<","<<it[i].High<<" ]"<<endl;
        }

    }
    //search function that take interval and returns the first interval overlapping with it
    Interval* Search( Node* node,Interval* interval)
    {

        //if node = null return
        if(node == nullptr)
        {
            return nullptr;
        }
        // if overlap return interval
        if(overlap(interval,node->interval))
        {
            return node->interval;
        }
        // if interval low >node left max search in node right
        if(interval->Low > node->Left->Max)
        {
            return Search(node->Right, interval);
        }
        // else search in node left
        return Search(node->Left,interval);

    }
    //search function that take interval and prints all intervals overlapping with it
    void SearchInterval(Interval* it)
    {
        Interval overlaped[1000];
        int c=0;
        Interval* found= Search(Root,it);
            //while interval found not equal null put the matching interval in array of intervals
            // delete the interval and search for other intervals
            while( found != nullptr)
            {
                overlaped[c] = *found;
                //cout<<"hereeee [ "<< overlaped[c].Low<<","<<overlaped[c].High<<" ]"<<endl;
                DeleteInterval(found);
                c++;
                found=Search(Root, it);
                //cout<<"hereeee [ "<< found->Low<<","<<found->High<<" ]"<<endl;
            }
            cout<<"hhhh"<<endl;
        //print overlaped intervals
        Print(overlaped,c);
        //insert all the deleted intervals again
        for(int i=0;i<c;i++)
        {
            auto* interval = new Interval(overlaped[i].Low,overlaped[i].High);
            InsertInterval(interval);
        }



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
    Interval* i7 = new Interval(18,120);
    t1.InsertInterval(i7);
    Interval* i8 = new Interval(16,20);
    t1.InsertInterval(i8);
    Interval* i9 = new Interval(9,50);
    t1.InsertInterval(i9);

    cout<<"Test Case 1"<<endl;
    Interval* ii = new Interval(5,200);
    cout<<"Intervals Overlapping with interval"<<"[ "<< ii->Low<<","<<ii->High<<" ]"<<endl;
    t1.SearchInterval(ii);
   // t1.DeleteInterval(ii);
    cout<<"Root Max after Deleting "<<"[ "<< ii->Low<<","<<ii->High<<" ] interval :"<<endl;
    cout<<t1.Root->Max<<endl;
  //  t1.InsertInterval(ii);
    cout<<"-----------------------------------"<<endl;

    cout<<"Test Case 2"<<endl;
    Interval* ii1 = new Interval(17,19);
    cout<<"Intervals Overlapping with interval"<<"[ "<< ii1->Low<<","<<ii1->High<<" ]"<<endl;
    t1.SearchInterval(ii1);
    //t1.DeleteInterval(ii1);
    cout<<"Root Max after Deleting "<<"[ "<< ii1->Low<<","<<ii1->High<<" ] interval :"<<endl;
    cout<<t1.Root->Max<<endl;
    //t1.InsertInterval(ii1);
    cout<<"-----------------------------------"<<endl;

    cout<<"Test Case 3"<<endl;
    Interval* ii2 = new Interval(10,40);
    cout<<"Intervals Overlapping with interval"<<"[ "<< ii2->Low<<","<<ii2->High<<" ]"<<endl;
    t1.SearchInterval(ii2);
    t1.DeleteInterval(ii2);
    cout<<"Root Max after Deleting "<<"[ "<< ii2->Low<<","<<ii2->High<<" ] interval :"<<endl;
    cout<<t1.Root->Max<<endl;
    t1.InsertInterval(ii2);
    cout<<"-----------------------------------"<<endl;

    cout<<"Test Case 4"<<endl;
    Interval* ii3 = new Interval(10,30);
    cout<<"Intervals Overlapping with interval"<<"[ "<< ii3->Low<<","<<ii3->High<<" ]"<<endl;
    t1.SearchInterval(ii3);
    t1.DeleteInterval(ii3);
    cout<<"Root Max after Deleting "<<"[ "<< ii3->Low<<","<<ii3->High<<" ] interval :"<<endl;
    cout<<t1.Root->Max<<endl;
    t1.InsertInterval(ii3);
    cout<<"-----------------------------------"<<endl;

    cout<<"Test Case 5"<<endl;
    Interval* ii4 = new Interval(5,20);
    cout<<"Intervals Overlapping with interval"<<"[ "<< ii4->Low<<","<<ii4->High<<" ]"<<endl;
    t1.SearchInterval(ii4);
    t1.DeleteInterval(ii4);
    cout<<"Root Max after Deleting "<<"[ "<< ii4->Low<<","<<ii4->High<<" ] interval :"<<endl;
    cout<<t1.Root->Max<<endl;
    t1.InsertInterval(ii4);
    cout<<"-----------------------------------"<<endl;

    cout<<"Test Case 6"<<endl;
    Interval* ii5 = new Interval(12,15);
    cout<<"Intervals Overlapping with interval"<<"[ "<< ii5->Low<<","<<ii5->High<<" ]"<<endl;
    t1.SearchInterval(ii5);
    t1.DeleteInterval(ii5);
    cout<<"Root Max after Deleting "<<"[ "<< ii5->Low<<","<<ii5->High<<" ] interval :"<<endl;
    cout<<t1.Root->Max<<endl;
    t1.InsertInterval(ii5);
    cout<<"-----------------------------------"<<endl;

    cout<<"Test Case 7"<<endl;
    Interval* ii6 = new Interval(18,120);
    cout<<"Intervals Overlapping with interval"<<"[ "<< ii6->Low<<","<<ii6->High<<" ]"<<endl;
    t1.SearchInterval(ii6);
    t1.DeleteInterval(ii6);
    cout<<"Root Max after Deleting "<<"[ "<< ii6->Low<<","<<ii6->High<<" ] interval :"<<endl;
    cout<<t1.Root->Max<<endl;
    t1.InsertInterval(ii6);
    cout<<"-----------------------------------"<<endl;

    cout<<"Test Case 8"<<endl;
    Interval* ii7 = new Interval(16,20);
    cout<<"Intervals Overlapping with interval"<<"[ "<< ii7->Low<<","<<ii7->High<<" ]"<<endl;
    t1.SearchInterval(ii7);
    t1.DeleteInterval(ii7);
    cout<<"Root Max after Deleting "<<"[ "<< ii7->Low<<","<<ii7->High<<" ] interval :"<<endl;
    cout<<t1.Root->Max<<endl;
    t1.InsertInterval(ii7);
    cout<<"-----------------------------------"<<endl;

    cout<<"Test Case 9"<<endl;
    Interval* ii8 = new Interval(30,300);
    cout<<"Intervals Overlapping with interval"<<"[ "<< ii8->Low<<","<<ii8->High<<" ]"<<endl;
    t1.SearchInterval(ii8);
    t1.DeleteInterval(ii8);
    cout<<"Root Max after Deleting "<<"[ "<< ii8->Low<<","<<ii8->High<<" ] interval :"<<endl;
    cout<<t1.Root->Max<<endl;
    t1.InsertInterval(ii8);
    cout<<"-----------------------------------"<<endl;

    cout<<"Test Case 10"<<endl;
    Interval* ii9 = new Interval(9,50);
    cout<<"Intervals Overlapping with interval"<<"[ "<< ii9->Low<<","<<ii9->High<<" ]"<<endl;
    t1.SearchInterval(ii9);
    t1.DeleteInterval(ii9);
    cout<<"Root Max after Deleting "<<"[ "<< ii9->Low<<","<<ii9->High<<" ] interval :"<<endl;
    cout<<t1.Root->Max<<endl;
    t1.InsertInterval(ii9);
    cout<<"-----------------------------------"<<endl;

   return 0;
}
