//genetic algorithm

#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<stdbool.h>

const int n = 3;  //each state(chromosome) have 3 elements(genes)
int K = 4;
int maxfitness = 0;   // maximum fitness


 // function for mutation
 //for providing genetic diversity from one generation to other
 //bit flip mutation
void mutate(int state[n])
{
  int index = rand()%n;

  if(state[index] == 1)
    state[index] = 0;
  else
    state[index] = 1;
}


//for counting the sum of digits
int sum(int arr[n])
{
  int res = 0;
  for (int i = 0; i < n; ++i)
  {
    res += arr[i];
  }
  return res;
}


//fitness value calculation of state
//in terms of 2*x*x +1
int fitness(int state[n])
{
  int res = 0;
  for (int i = 0; i < n; ++i)
    res = res*2 + state[i];

  return 2*res*res + 1;
}


//for printing fitness value of given states
void print(int state1[n], int state2[n], int state3[n], int state4[n])
{
  printf("\n");
  for (int i = 0; i < n; ++i)
    printf("%d  ", state1[i]);
  printf("fitness = %d\n", fitness(state1));
  for (int i = 0; i < n; ++i)
    printf("%d  ", state2[i]);
  printf("fitness = %d\n", fitness(state2));
  for (int i = 0; i < n; ++i)
    printf("%d  ", state3[i]);
  printf("fitness = %d\n", fitness(state3));
  for (int i = 0; i < n; ++i)
    printf("%d  ", state4[i]);
  printf("fitness = %d\n", fitness(state4));
  printf("\n");
}


//for random generation of   state
//each state have 3 elements
void generate(int state[n])
{
  state[0] = rand()%2;
  state[1] = rand()%2;
  state[2] = rand()%2;

  if(sum(state) == 3)
    mutate(state);
}


//to check whether two states are equal or not
bool isequal(int state1[n], int state2[n])
{
  for (int i = 0; i < n; ++i)
  {
    if(state1[i] != state2[i])
      return false;
  }
  return true;
}


//for copying one state into another state
void copy(int x[n], int y[n])
{
  for(int i=0 ; i<n ; i++)
    x[i] = y[i];
}


//assigning elements of state to parent based on given index
void assign(int parent[n], int index, int state1[n], int state2[n], int state3[n], int state4[n])
{
  switch(index)
  {
    case 0:
      copy(parent, state1);
      break;
    case 1:
      copy(parent, state2);
      break;
    case 2:
      copy(parent, state3);
      break;
    case 3:
      copy(parent, state4);
      break;
  }

}

//function to perform single point crossover
//for generating same number of offspring as intial population
void crossover(int parent1[n], int parent2[n], int child1[n], int child2[n])
{
  //generation of offspring
  child1[0] = parent1[0];
  child1[1] = parent2[1];
  child1[2] = parent2[2];
  child2[0] = parent2[0];
  child2[1] = parent1[1];
  child2[2] = parent1[2];

//for invalid states
  if(sum(child1) == 3)
    mutate(child1);
  if(sum(child2) == 3)
    mutate(child2);

  //checking for optimum fitness value
  if(fitness(child1) > maxfitness)
    maxfitness = fitness(child1);
  if(fitness(child2) > maxfitness)
    maxfitness = fitness(child2);
}


bool same(int arr[n])
{
  int temp = arr[0];
  for (int i = 1; i < n; ++i)
  {
    if(arr[i] != temp)
      return false;
  }
  return true;
}


//performing GA on given set of states
void GA(int state1[n], int state2[n], int state3[n], int state4[n])
{
    int objectiveValues[K], count = 0;
    int parent1[n], parent2[n];
    int child1[n], child2[n], child3[n], child4[n];
    int idx1, idx2;

    //
    while(count != K)
    {
      //randomly generating index for parent selection
      idx1 = rand()%K;
      idx2 = rand()%K;

      while(idx1 == idx2)
        idx2 = rand()%K;

      //selecting pairs of parents from given current population
      assign(parent1, idx1, state1, state2, state3, state4);
      assign(parent2, idx2, state1, state2, state3, state4);

      //generating offspring using crossover
      crossover(parent1, parent2, child1, child2);

      //objective value calculation of child
      objectiveValues[count++] = fitness(child1);
      objectiveValues[count++] = fitness(child2);

      while(idx1 == idx2)
       idx2 = rand()%K;

      assign(parent1, idx1, state1, state2, state3, state4);
      assign(parent2, idx2, state1, state2, state3, state4);

      crossover(parent1, parent2, child3, child4);

      objectiveValues[count++] = fitness(child3);
      objectiveValues[count++] = fitness(child4);
    }

    //offspring population
    printf("New generation: \n");
    print(child1, child2, child3, child4);


    if(same(objectiveValues) && objectiveValues[0] == maxfitness)
    {
      printf("The maximum value that can be attained is: %d\n", objectiveValues[0]);
      return;
    }

    else if(same(objectiveValues) && objectiveValues[0] != maxfitness)
    {
      printf("Stuck in local maxima\n");
      printf("The maximum value that can be attained is: %d\n", objectiveValues[0]);
      printf("Applying mutation and again applying algorithm\n");
      mutate(child1);
      mutate(child2);
      GA(child1, child2, child3, child4);
    }

    else
      GA(child1, child2, child3, child4);
}


int main()
{
  srand(time(0));

  //binary encoding
  int state1[n], state2[n], state3[n], state4[n]; //state representation

  //random generation of intial population (set of intial states)
  generate(state1);
  generate(state2);
  generate(state3);
  generate(state4);

  //checking whether set of intial state have variation or not , if not then regenerate that state
  while(isequal(state2, state1) || isequal(state2, state3) || isequal(state2, state4))
    generate(state2);

  while(isequal(state3, state1) || isequal(state3, state4))
    generate(state3);

  while(isequal(state4, state1))
    generate(state4);

  //intial population
  printf("Initial Population: \n");
  print(state1, state2, state3, state4);

  //performing GA on intial population
  GA(state1, state2, state3, state4);

  return 0;
}
