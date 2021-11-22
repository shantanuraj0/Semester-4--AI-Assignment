//particle swarm optimisation

#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<stdbool.h>


int gbest = 100000;  //gbest value
int *gbest_pos;     //gbest position
int num_particles = 3;  //number of particles
int *marks;         //for storing marks of students
int n, k, c1 = 2, c2 = 1;  //c1 and c2 are constants used in updation of velocity of particles


//structure of particle
struct particle
{
  int *pos;         //position of particle
  int *vel;         //velocity of particle
  int pbest;        //pbest value
  int *pbest_pos;   //pbest position
};

//particles array
struct particle particles[3];


//for printing details of particles
void print_details(struct particle part)
{
      for(int j=0 ; j<k ; j++)
        printf("%d  ", part.pos[j]);
      printf("\n");

      for(int j=0 ; j<k ; j++)
        printf("%d  ", part.vel[j]);
      printf("\n");

      printf("pbest = %d\n", part.pbest);

      for(int j=0 ; j<k ; j++)
        printf("%d  ", part.pbest_pos[j]);
      printf("\n");
        printf("\n");
}


//for copying one state into another
void copy(int x[], int y[])
{
  for(int i=0 ; i<k ; i++)
    x[i] = y[i];
}


//checking whether x is present in array or not
bool present_in(int x, int arr[])
{
  for(int i=0 ; i<k ; i++)
  {
    if(arr[i] == x)
      return true;
  }
  return false;
}


//sorting the given array
void sort(int arr[], int n)
{
    int i, key, j;
    for (i = 1; i < n; i++) {
        key = arr[i];
        j = i - 1;
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = key;
    }
}


//for finding  sum of square of absolute postion value of all groups
int evaluate(int pos[])
{
  int res=0;
  for(int i=0 ; i<n ; i++)                // for every marks
  {
    if(present_in(marks[i], pos))
      continue;

    int temp;
    for(int j=0 ; j<k ; j++)            // check marks with every position
    {
      temp = 10000;
      if(abs(pos[j] - marks[i]) < temp)
        temp = abs(pos[j] - marks[i]);
    }
    res += temp*temp;
  }
  return res;
}


//for updating the details of the particles
void update(int *velocity, int pbest, int *position, int *pbest_pos)
{
  float constant = (0.1)*(rand()%10+1);
  printf("random number = %f\n", constant);
  for(int i = 0 ; i < k ; i++)
  {
    velocity[i] = velocity[i] + c1*constant*(pbest_pos[i] - position[i]) + c2*constant*(gbest_pos[i] - position[i]);
    position[i] = position[i] + velocity[i];

    if(position[i] < marks[0]){
      position[i] = marks[0];
    }
    else if(position[i] > marks[n-1]){
      position[i] = marks[n-1];
    }
  }
}


int main()
{
  srand(time(0));
  printf("Enter the number of  students: \n");   //number of students
  scanf("%d", &n);
  printf("Enter the value of K: \n");   //number of groups
  scanf("%d", &k);

  marks = (int*)malloc(sizeof(int)*n);   //allocating space for storing marks of students
  printf("Enter students marks: \n");
  for (int i = 0; i < n; ++i)
    scanf("%d", &marks[i]);             //storing marks

  sort(marks, n);      //sorting marks of students

  gbest_pos = (int*)malloc(sizeof(int)*k);  // allocating space for storing gbest value for each group


  // creating initial number of particles
  for(int i=0 ; i<num_particles ; i++)
  {
     //allocating space for storing details of particles
    int *pos = (int*)malloc(sizeof(int)*k);
    int *vel = (int*)malloc(sizeof(int)*k);
    particles[i].pos = (int*)malloc(sizeof(int)*k);
    particles[i].vel = (int*)malloc(sizeof(int)*k);
    particles[i].pbest_pos = (int*)malloc(sizeof(int)*k);

    //providing marks randomly
    // treating marks as postion value for each element
    //setting intial velocity to zero
    for(int j=0 ; j<k ; j++)
    {
      pos[j] = marks[rand()%n];
      vel[j] = 0;
    }

    copy(particles[i].pos, pos);
    copy(particles[i].vel, vel);

    // initiallise personal best
    //here we have to optimise pbest value
    particles[i].pbest = evaluate(pos);
    copy(particles[i].pbest_pos, particles[i].pos);


    // initialise global best
    if(particles[i].pbest < gbest)
    {
      gbest = particles[i].pbest;
      copy(gbest_pos, particles[i].pbest_pos);
    }
  }

    //intial population
    printf("\nDetails of the initial population : \n");
    for(int i=0 ; i<num_particles ; i++)
    {
      printf("Particle %d: \n", i+1);
      print_details(particles[i]);
    }

    printf("Global best so far = %d\nGlobal best position: ", gbest);
    for(int i=0 ; i<k ; i++)
    printf("%d  ", gbest_pos[i]);
    printf("\n\n");


  //outer loop for number of iterations
  for (int itr = 0; itr < 1000; ++itr)
  {
    printf("Iteration %d: \n", itr+1);

    // inner loop is for number of particles
    for(int i = 0 ; i < num_particles ; i++)
    {
        //updating details of particle
         update(particles[i].vel, particles[i].pbest, particles[i].pos, particles[i].pbest_pos);
         int temp = evaluate(particles[i].pos);


         // updating pbest
        if(temp < particles[i].pbest)
        {
          particles[i].pbest = temp;
          copy(particles[i].pbest_pos, particles[i].pos);
        }

        // update gbest
        if(particles[i].pbest < gbest)
        {
          gbest = particles[i].pbest;
          copy(gbest_pos, particles[i].pbest_pos);
        }
    }


    // printing details of particles after each iterations
    for(int i=0 ; i<num_particles ; i++)
    {
      printf("Particle %d: \n", i+1);
      print_details(particles[i]);
    }

    printf("Global best so far = %d\nGlobal best position: ", gbest);
    for(int i=0 ; i<k ; i++)
      printf("%d  ", gbest_pos[i]);
    printf("\n\n");

  }

  //final result of group
  printf("\nFinal group\n");
  for (int i = 0; i < n; ++i)
    printf("%d  ", marks[i]);
  printf("\n");


  for (int i = 0; i < n; ++i)
  {
    int temp = 10000;
    int index = -1;
    for(int j = 0 ; j < k ; j++)
    {
      if(abs(gbest_pos[j] - marks[i]) < temp)
      {
        temp = abs(gbest_pos[j] - marks[i]);
        index = j+1;
      }
    }
    printf("%d  ", index);
  }
  printf("\n");
  return 0;
}
