//stochastic hill climbing


#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<stdbool.h>



const int n = 8; // for handling 8 queens on board


//for converting state array to single number 
// this is done to reduce space usage
int change(int state[n])                          
{                                                 
  int res = 0;          
  for(int i=0 ; i<n ; i++)
  {
    res = res*10 + state[i];
  }
  return res;
}



// intialising board with queens randomly by rand function
void Randomly(int board[][n], int state[n] )  
{
  srand(time(0));

  for (int i = 0; i < n; ++i)
  {
    state [i] = rand()%n;
    board[state[i]][i] = 1;
  }
}


// printing the board
void printBoard(int board[][n])   
{
 
    for (int i = 0; i < n; i++) {
        printf(" ");
        for (int j = 0; j < n; j++) {
            printf("%d ", board[i][j]);
        }
        printf("\n");
    }
}


// objective value calculation of the state
//calculating objective function value in terms of number of attacking pairs each other
//state [1 ,2,3 ,4,5]  : queen 1st present in 1st row of 1st column
int calculateObjective(int board[][n], int state[n])    
{
    int attacking = 0;  //for mainting count of attacking queens
 
    int row, col;
 
    
    // traversing across columns
    for (int i = 0; i < n; i++) 
    {
        // current position of queen in row
        row = state[i];
         col = i - 1;
        while (col >= 0 && board[row][col] != 1) {      // right to left check (traversing aross columns and checking whether any attacking queen in left)
            col--;
        }
        if (col >= 0 && board[row][col] == 1) {
            attacking++;
        }


        row = state[i], col = i + 1;
        while (col < n && board[row][col] != 1) {      // left to right check
            col++;
        }
        if (col < n && board[row][col] == 1) {
            attacking++;
        }
 
        row = state[i] - 1, col = i - 1;
        while (col >= 0 && row >= 0 && board[row][col] != 1) {    // bottom right to top left
            col--;
            row--;
        }
        if (col >= 0 && row >= 0 && board[row][col] == 1) {
            attacking++;
        }
 
        row = state[i] + 1, col = i + 1;
        while (col < n && row < n && board[row][col] != 1) {      // top left to bottom right
            col++;
            row++;
        }
        if (col < n && row < n && board[row][col] == 1) {
            attacking++;
        }
 
        row = state[i] + 1, col = i - 1;
        while (col >= 0 && row < n && board[row][col] != 1) {     // top right to bottom left
            col--;
            row++;
        }
        if (col >= 0 && row < n && board[row][col] == 1) {
            attacking++;
        }
 
        row = state[i] - 1, col = i + 1;
        while (col < n && row >= 0 && board[row][col] != 1) {     // bottom left to top right
            col++;
            row--;
        }
        if (col < n && row >= 0 && board[row][col] == 1) {
            attacking++;
        }
    }
 
    return (int)(attacking / 2);  //we have to count pairs only
}



int backup = 64205713;



// generates the board with the given state
void generateBoard(int board[][n], int state[n])      
{
  for (int i = 0; i < n; ++i)
  {
    for(int j = 0 ; j < n ; j++)
      board[i][j] = 0;
  }

  for (int i = 0; i < n; ++i)
  {
    board[state[i]][i] = 1;
  }
}


//generating next neighbour of state
//generating all possible states of given state and selecting a state randomly from set of possible states
//probability of selection of state depends on objective function value i.e ( a state with low objective function value have high probability of selection)
void getNeighbour(int board[][n], int state[n])       
{
  int neighbours[2000], k=0;

  int NeighbourBoard[n][n];  //board representation of child
  int NeighbourState[n];  // state representation of child
 
 //copying parent state to child state
  for (int i = 0; i < n; ++i)
  {
    NeighbourState[i] = state[i];
  }

  //generating board
  generateBoard(NeighbourBoard, NeighbourState);

  //generating all possible states for given state
  for (int i = 0; i < n; ++i)
  {
    for(int j=0 ; j<n ; j++)
    {
      if(j == state[i]) continue;  //if state element is same as parent , then skip

      NeighbourState[i] = j;                    // creating new state by changing initial state
      NeighbourBoard[NeighbourState[i]][i] = 1;
      NeighbourBoard[state[i]][i] = 0;          // changing board according to the new genereated state

      
      int temp = calculateObjective(NeighbourBoard, NeighbourState);
                                                // on the basis of this opState will be selected
      int count = 21-temp;

      temp = change(NeighbourState);        // storing newly generated state
      
      for (int i = 0; i < count; ++i)
      {
        neighbours[k] = temp;
        k++;
      }

      NeighbourBoard[NeighbourState[i]][i] = 0;  // getting back to the parent state
      NeighbourState[i] = state[i];
      NeighbourBoard[state[i]][i] = 1;

    }
  }

  int index = rand()%k;                         // generating random index 
  //printf("%d\t%d\n", index, neighbours[index]);

  int objectivevalue = neighbours[index];

  for(int i=1 ; i<=n ; i++)                     // converting single number back to original state array
  {
    state[n-i] = objectivevalue%10;
    objectivevalue = objectivevalue/10;
  }

  generateBoard(board, state);                  // generating board from the state array

}


//for checking two states are equal or not
bool is_equal(int state1[n], int state2[n])                        
{
 
    for (int i = 0; i < n; i++) {
        if (state1[i] != state2[i]) {
            return false;
        }
    }
    return true;
}


//generating a child state and making changes in that state in order to reduce objective function value
void stochastic_hill_Climbing(int board[][n], int state[n])
{

  int neighbourBoard[n][n];  //board  representation of child
  int neighbourState[n];  //state representation of child

  
   // copying parent state to child state
  for (int i = 0; i < n; ++i)            
    neighbourState[i] = state[i];

  //generating board for child
  generateBoard(neighbourBoard, neighbourState);
  int tim=0;


  do
  {
    for (int i = 0; i < n; ++i)
    {
      state[i] = neighbourState[i];
    }

    generateBoard(board, state);

    if(calculateObjective(board, state) == 0)
    {
      printf("We got the answer: \n");
      printBoard(board);
      return;
    }

    //generating  next state 
    getNeighbour(neighbourBoard, neighbourState);
    printf("\nThe new selected Board: \n");
    printBoard(neighbourBoard);
    printf("Number of pairs attacking each other = %d\n", calculateObjective(neighbourBoard, neighbourState));
    printf("****************\n");
    
  } while ((tim++) < 500);

  for(int i=1 ; i<=n ; i++)                     // converting single number back to original state array
  {
    state[n-i] = backup%10;
    backup = backup/10;
  }
  printf("We got the answer: \n");
  generateBoard(board, state);
  //printf("#######\n");
  printBoard(board);
  printf("Number of pairs attacking each other = %d\n",calculateObjective(board, state));

}



int main()
{

  int state[n];    //for state representation , each queen per column
  int board[n][n];  // for board representation { 2-D Matrix}

  

  for (int i = 0; i < n; ++i)         // initially intialising the board with all elements = 0
  {
    for(int j=0 ; j<n ; j++)
      board[i][j] = 0;
  }

  
  Randomly(board, state);  // randomly placing the queens on board
  
  stochastic_hill_Climbing(board, state);  

  return 0;
}