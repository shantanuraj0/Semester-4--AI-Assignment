#include <iostream>
#include <vector>

#define INITIAL 3         //total count of missionary and cannibal
#define LEFT 0           //boat position on left
#define RIGHT 1          //boat position on right
#define COST_MISS 10      // cost of each missionary
#define COST_CANN 20      // cost of each cannibal
#define min(a, b) (a)<(b) ? (a):(b)   // to find minimum

using namespace std;



//node structure
typedef struct node
{
    int miss_on_left;
    int cann_on_left;
    bool boat_pos;
    struct node* parent;
    int cost;
    int depth;
}node;


//goal node
node* goal;



//creation of new node
//intially boat position is left
node* newNode(int mol, int col, bool boat_pos, node* parent, int cost, int depth)
 {
    node* n = (node*)malloc(sizeof(node));
    n->miss_on_left = mol;
    n->cann_on_left = col;
    n->boat_pos = boat_pos;
    n->parent = parent;
    n->cost = cost;
    n->depth = depth;
    return n;
}



// to insert node in open list in sorted manner
void insertSorted(vector<node*>& open, node* t)
 {
    for(auto i=open.begin(); i!=open.end(); i++) {
        if(t->cost <= (*i)->cost) {
            open.insert(i, t);
            return;
        }
    }
    open.insert(open.end(), t);
}



// this function is used by goal test function
bool isEqual(node* a, node* b) {
    if(a->miss_on_left == b->miss_on_left && a->cann_on_left == b->cann_on_left && a->boat_pos == b->boat_pos)
        return true;
    else
        return false;
}


//goal test function: it returns 1 if current node is goal node else 0
bool goalTest(node* t) {
    return isEqual(t, goal);
}




// for printing node used by printing path function
void printNode(node* t) {
    printf("********************\t\t\t*********************\n");
    printf("|  %d Missionaries  |", t->miss_on_left);

    if(t->boat_pos == LEFT) printf(" ||B||\t\t");
    else printf("\t\t ||B|| ");

    printf("|  %d Missionaries  |\n", INITIAL-t->miss_on_left);
    printf("|    %d Cannibals   |", t->cann_on_left);
    if(t->boat_pos == LEFT) printf("  ````\t\t");
    else printf("\t\t  ````  ");
    printf("|    %d Cannibals   |\n", INITIAL-t->cann_on_left);
    printf("*******************\t\t\t*********************\n");
}



//for printing path
void printPath(node* t) {
    if(t->parent != NULL)
        printPath(t->parent);
    printNode(t);
    
}






//this function is used by successor function to check that given combination of missionary and cannibal is valid combination or not
bool checkNode(int mol, int col)
 {
    if(mol < 0 || mol > INITIAL || col < 0 || col > INITIAL) return false;
    if(mol == 0 || mol == INITIAL || mol == col) return true;
    return false;
}



//this function generate successors of a node
vector<node*> successor(node* t)
{
    vector<node*> succ;
    int mol, mor, col, cor;
    bool bp = t->boat_pos;
    mol = t->miss_on_left;
    col = t->cann_on_left;

    if(bp == LEFT) 
    {
        //here first checking different combinations of missionary and cannibal is valid combination or not , if valid then insert that combination as a node in succsessor list
        if(checkNode(mol-1, col)) succ.push_back(newNode(mol-1, col, RIGHT, t, t->cost+COST_MISS, t->depth+1));     //sending 1 missionary
        if(checkNode(mol-2, col)) succ.push_back(newNode(mol-2, col, RIGHT, t, t->cost+2*COST_MISS, t->depth+1));   //sending 2 missionary
        if(checkNode(mol, col-1)) succ.push_back(newNode(mol, col-1, RIGHT, t, t->cost+COST_CANN, t->depth+1));     //sending 1 cannibal
        if(checkNode(mol, col-2)) succ.push_back(newNode(mol, col-2, RIGHT, t, t->cost+2*COST_CANN, t->depth+1));   //sending 2 cannibal
        if(checkNode(mol-1, col-1)) succ.push_back(newNode(mol-1, col-1, RIGHT, t, t->cost+COST_MISS+COST_CANN, t->depth+1));  // sending 1 missionary and 1 cannibal
    }
    else {
        if(checkNode(mol+1, col)) succ.push_back(newNode(mol+1, col, LEFT, t, t->cost+COST_MISS, t->depth+1));
        if(checkNode(mol+2, col)) succ.push_back(newNode(mol+2, col, LEFT, t, t->cost+2*COST_MISS, t->depth+1));
        if(checkNode(mol, col+1)) succ.push_back(newNode(mol, col+1, LEFT, t, t->cost+COST_CANN, t->depth+1));
        if(checkNode(mol, col+2)) succ.push_back(newNode(mol, col+2, LEFT, t, t->cost+2*COST_CANN, t->depth+1));
        if(checkNode(mol+1, col+1)) succ.push_back(newNode(mol+1, col+1, LEFT, t, t->cost+COST_MISS+COST_CANN, t->depth+1));
    }
    return succ;
}






// bfs implementation
int BFS(node* start)
 {
    int i, j;
    bool done, flag = true;
    node* t=start;
    vector<node*> open, close, succ;   //maintaining open list ,close list and successor list
    open.push_back(t);  //inserting new node at end

    if(goalTest(t)) flag = false;   //applying goal test and if it returns true , terminate search process


    printf("\nBFS:-\n\n");
    printf("Open List: {{%d, %d, %d, %d}}\n", start->miss_on_left, start->cann_on_left, start->boat_pos, start->cost);   //displaying contents of open list
    printf("Close List: {}\n\n");   //displaying contents of close list



    //if intial goal test returns false then we move here
    while(flag)
    {
        if(open.empty()) return -1; // checking open list is empty , if true then return failure
        t = open.front();         //picking  a node from front of open list
        succ = successor(t);      //generating succesors of that node
        close.push_back(t);        //inserting node in close after expansion of that node
        open.erase(open.begin());  // removing a node from front of open list



        for(i=0; i<succ.size(); i++)
         {
             //applying goal test on each node generated by successor function and terminate search if it returns true
            if(goalTest(succ[i])) {
                flag = false;
                t = succ[i];
            }
            done = false;


            //checking whether generated node is present in close list or not
            for(j=0; j<close.size(); j++)
            {
                if(isEqual(succ[i], close[j])) {
                    done = true;
                    break;
                }
            }

            //checking generated node is present in open list or not
            for(j=0; j<open.size(); j++)
             {
                if(isEqual(succ[i], open[j])) {
                    done = true;
                    break;
                }
            }

            //if generated node is not present in open list or close list , then insert that node into open list
            if(!done) open.push_back(succ[i]);
            else free(succ[i]);   //removing expanded node from successor node
        }

        succ.clear();  //clearing the successor list for next iteration

        //displaying contents of open list
        printf("Open List: {");
        for(i=0; i<open.size(); i++) printf("{%d, %d, %d, %d} ", open[i]->miss_on_left, open[i]->cann_on_left, open[i]->boat_pos, open[i]->cost);
        printf("\b}\n");

        //displaying contents of close list
        printf("Close List: {");
        for(i=0; i<close.size(); i++) printf("{%d, %d, %d, %d} ", close[i]->miss_on_left, close[i]->cann_on_left, close[i]->boat_pos, close[i]->cost);
        printf("\b}\n\n");

    }


    printf("No. of Nodes Generated: %d\n", open.size()+close.size());
    printf("No. of Nodes Expanded: %d\n\n", close.size());
    printf("Path Cost: %d\n\n", t->cost);
    printf("Solution Path:\n");
    printPath(t);

    if(close.empty()) return 1;  //checking if close list is empty , it means no node is expanded : so need to deallocate the memory

    //free the allocated memory for open list and close list
    for(i=0; i<open.size(); i++) free(open[i]);
    for(i=1; i<close.size(); i++) free(close[i]);
    return 1;
}





//dfs implementation
int DFS(node* start)
 {
    int i, j;
    bool done, flag = true;
    node* t=start;
    vector<node*> open, close, succ; //open list and close list and successor list
    open.push_back(t);  //inserting new node at end

    if(goalTest(t)) flag = false;  //applying goal test and if it returns true , terminate search process


    printf("\nDFS:-\n\n");
    printf("Open List: {{%d, %d, %d, %d}}\n", start->miss_on_left, start->cann_on_left, start->boat_pos, start->cost);   //displaying contents of open list
    printf("Close List: {}\n\n");   //displaying contents of close list


    while(flag)
     {

        if(open.empty()) return -1;   // checking open list is empty , if true then return failure
        t = open.front();   //picking  a node from front of open list
        succ = successor(t);  //generating succesors of that node
        close.push_back(t);   //inserting node in close after expansion of that node
        open.erase(open.begin());  // removing a node from front of open list


        for(i=0; i<succ.size(); i++)
         {
             //applying goal test on each node generated by successor function and terminate search if it returns true
            if(goalTest(succ[i])) {
                flag = false;
                t = succ[i];
            }
            done = false;

            //checking generated node is present in close list or not
            for(j=0; j<close.size(); j++)
            {
                if(isEqual(succ[i], close[j])) {
                    done = true;
                    break;
                }
            }


            //checking generated node is present in open list or not
            for(j=0; j<open.size(); j++)
             {
                if(isEqual(succ[i], open[j])) {
                    done = true;
                    break;
                }
            }

            //if generated node is not present in open list or close list , then insert that node into open list
            if(!done) open.insert(open.begin(), succ[i]);
            else free(succ[i]);  //removing expanded node from successor node

        }

        succ.clear();  //clearing the successor list for next iteration


        //displaying contents of open list
        printf("Open List: {");
        for(i=0; i<open.size(); i++) printf("{%d, %d, %d, %d} ", open[i]->miss_on_left, open[i]->cann_on_left, open[i]->boat_pos, open[i]->cost);
        printf("\b}\n");

        //displaying contents of close list
        printf("Close List: {");
        for(i=0; i<close.size(); i++) printf("{%d, %d, %d, %d} ", close[i]->miss_on_left, close[i]->cann_on_left, close[i]->boat_pos, close[i]->cost);
        printf("\b}\n\n");


    }

    printf("No. of Nodes Generated: %d\n", open.size()+close.size());
    printf("No. of Nodes Expanded: %d\n\n", close.size());
    printf("Path Cost: %d\n\n", t->cost);
    printf("Solution Path:\n");
    printPath(t);

    if(close.empty()) return 1;  //checking if close list is empty , it means no node is expanded : so need to deallocate the memory

    //free the allocated memory for open list and close list
    for(i=0; i<open.size(); i++) free(open[i]);
    for(i=1; i<close.size(); i++) free(close[i]);
    return 1;
}





//iterative deepening implementation
int IterativeDeepening(node* start)
{
    int i, j, k;
    int x=0;
    bool done, flag = true;
    node* t;
    vector<node*> open, close, succ;   //maintaining open list ,close list and successor list

    printf("\nIterative Deepening:-\n\n");

    for(k=0;k<100 ; k++)
     {

        t = start;
        open.push_back(t);   //inserting new node at end


        if(goalTest(t)) flag = false;  //applying goal test and if it returns true , terminate search process
        printf("k: %d\n", k);


        printf("Open List: {{%d, %d, %d, %d}}\n", start->miss_on_left, start->cann_on_left, start->boat_pos, start->cost);   //displaying contents of open list
        printf("Close List: {}\n\n");   //displaying contents of close list


        //if intial goal test returns false then we move here
        while(flag)
        {
            // checking open list is empty , if true then return failure
            if(open.empty()) {

                bool finished=true;
                for(i=0; i<close.size(); i++) if(close[i]->depth == k) finished=false;
                if(finished) return -1;

                break;
            }


            t = open.front();   //picking  a node from front of open list
            if(t->depth < k) succ = successor(t);  //generating succesors of that node when current depth < k
            close.push_back(t);   //inserting node in close after expansion of that node
            open.erase(open.begin());   // removing a node from front of open list



            for(i=0; i<succ.size(); i++)
             {
                //applying goal test on each node generated by successor function and terminate search if it returns true
                if(goalTest(succ[i])) {
                    flag = false;
                    t = succ[i];
                }
                done = false;


                //checking whether generated node is present in close list or not
                for(j=0; j<close.size(); j++)
                 {
                    if(isEqual(succ[i], close[j])) {
                        done = true;
                        break;
                    }
                }


                //checking generated node is present in open list or not
                for(j=0; j<open.size(); j++)
                {
                    if(isEqual(succ[i], open[j])) {
                        done = true;
                        break;
                    }
                }

                //if generated node is not present in open list or close list , then insert that node into open list
                if(!done) open.insert(open.begin(), succ[i]);
                else free(succ[i]);
            }

            succ.clear();  //clearing the successor list for next iteration


            //displaying contents of open list
            printf("Open List: {");
            for(i=0; i<open.size(); i++) printf("{%d, %d, %d, %d} ", open[i]->miss_on_left, open[i]->cann_on_left, open[i]->boat_pos, open[i]->cost);
            if(open.empty()) printf("}\n");
            else printf("\b}\n");

            //displaying contents of close list
            printf("Close List: {");
            for(i=0; i<close.size(); i++) printf("{%d, %d, %d, %d} ", close[i]->miss_on_left, close[i]->cann_on_left, close[i]->boat_pos, close[i]->cost);
            printf("\b}\n\n");


        }

        if(!flag) break;
        x += close.size();

        //after every iteration , clear the close list
        for(i=1; i<close.size(); i++) free(close[i]);
        close.clear();

    }

    if(flag) return -1;
    printf("No. of Nodes Generated: %d\n", x+open.size()+close.size());
    printf("No. of Nodes Expanded: %d\n\n", x+close.size());
    printf("Path Cost: %d\n\n", t->cost);
    printf("Solution Path:\n");
    printPath(t);

    if(close.empty()) return 1; //checking if close list is empty , it means no node is expanded : so need to deallocate the memory

    //free the allocated memory
    for(i=0; i<open.size(); i++) free(open[i]);
    for(i=1; i<close.size(); i++) free(close[i]);
    return 1;
}





//ucs implementation
int UCS(node* start)
{
    int i;
    vector<node*>::iterator j;
    int g, e;
    bool done, flag = true;
    node* t=start;

    vector<node*> open, close, succ;  //maintaining open , close and succesor list
    open.push_back(t);  //inserting new node at end

    if(goalTest(t)) flag = false;  //applying goal test and if it returns true , terminate search process


    g = e = 0;


    printf("\nUCS:-\n\n");
    printf("Open List: {{%d, %d, %d, %d}}\n", start->miss_on_left, start->cann_on_left, start->boat_pos, start->cost);   //displaying contents of open list
    printf("Close List: {}\n\n");


    //if intial goal test returns false then we move here
    while(flag)
    {

        if(open.empty()) return -1;  // checking open list is empty , if true then return failure due to no nodes are present for expansion
        t = open.front();   //picking  a node from front of open list

        if(goalTest(t)) break;

        succ = successor(t);   //generating succesors of that node
        close.push_back(t);    //inserting node in close after expansion of that node
        open.erase(open.begin());  // removing a node from front of open list


        for(i=0; i<succ.size(); i++)
        {
            done = false;


             //checking whether generated node is present in close list or not
            for(j=close.begin(); j!=close.end(); j++)
            {
                if(isEqual(succ[i], *j)) {
                    //if node is present in close list , then checking whether generated node  have lower path cost ( i.e node reopening is required) , then move that node from close list to open list
                    if((*j)->cost > succ[i]->cost) {

                        //node reopening
                        free(*j);  //free the allocated memory
                        close.erase(j);  // removing that node from close list
                        g++; e++;
                        insertSorted(open, succ[i]);  // inserting succesor generated node in open list in sorted manner
                    }
                    done = true;
                    break;
                }
            }

             //checking whether generated node is present in open list or not
            for(j=open.begin(); j!=open.end(); j++)
             {

                if(isEqual(succ[i], *j)) {

                    //if node present in open list , then checking whether generated node have lower path cost , then we have to update the path cost of that node
                    if((*j)->cost > succ[i]->cost) {

                        //path cost updation in open list
                        free(*j);  // free the allocated memory
                        open.erase(j);  // removing that node from open list
                        g++;
                        insertSorted(open, succ[i]);  // inserting that node with updated path cost in open list
                    }
                    done = true;
                    break;
                }
            }

             //if generated node is not present in open list or close list , then insert that node into open list in sorted manner
            if(!done) insertSorted(open, succ[i]);
            else free(succ[i]);  //removing expanded node from successor node
        }

        succ.clear();  //clearing the successor list for next iteration

        //displaying contents of open list
        printf("Open List: {");
        for(i=0; i<open.size(); i++) printf("{%d, %d, %d, %d} ", open[i]->miss_on_left, open[i]->cann_on_left, open[i]->boat_pos, open[i]->cost);
        if(open.empty()) printf("}\n");
        else printf("\b}\n");

        //displaying contents of close list
        printf("Close List: {");
        for(i=0; i<close.size(); i++) printf("{%d, %d, %d, %d} ", close[i]->miss_on_left, close[i]->cann_on_left, close[i]->boat_pos, close[i]->cost);
        if(close.empty()) printf("}\n\n");
        else printf("\b}\n\n");

    }


    printf("No. of Nodes Generated: %d\n", g+open.size()+close.size());
    printf("No. of Nodes Expanded: %d\n\n", e+close.size());
    printf("Path Cost: %d\n\n", t->cost);
    printf("Solution Path:\n");
    printPath(t);

    if(close.empty()) return 1; //checking if close list is empty , it means no node is expanded : so need to deallocate the memory

    //free the allocated memory for open list and close list
    for(i=0; i<open.size(); i++) free(open[i]);
    for(i=1; i<close.size(); i++) free(close[i]);
    return 1;
}



int main() {
    node* start = newNode(INITIAL, INITIAL, LEFT, NULL, 0, 0);
    goal = newNode(0, 0, RIGHT, NULL, 0, 0);      // setting goal node  :  goal state (0 , 0 ,1)
    if(BFS(start) == -1) printf("\nBFS: Solution not found\n\n");    //bfs
    if(DFS(start) == -1) printf("\nDFS: Solution not found\n\n");   //dfs
    if(IterativeDeepening(start) == -1) printf("\nIterative Deepening: Solution not found\n\n");  //ids
    if(UCS(start) == -1) printf("\nUCS: Solution not found\n\n");  //ucs
    free(start); free(goal);  //free the allocated memory of start node , goal node
    return 0;
}
