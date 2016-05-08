
/* 2016.05.08 am 8:31 , 
simulating a ecosystem . Maybe it can be called chaos theory . Who knows ! 
*/

#include <stdio.h>
#include <stdlib.h>

int height = 4 , width = 4 ;
int plants = 4 ;

typedef struct cell {
  unsigned type : 4 ;
  unsigned mark : 4 ;
  int energy ;
} cell ;

cell **world ;


int random_range(int min , int max)
{
  int a ;
  
  a = random() % (max - min) ;

  return a ;
}

void init_world(void)
{
  int  i , j , k ;
  int count ;

  world = malloc(sizeof(cell *) * height) ;
  for(i = 0 ; i < 4 ; i++)
  {
    world[i] = malloc(sizeof(cell) * width) ;
    for(j = 0 ; j < 4 ; j++)
    {
      world[i][j].type = 0;
      world[i][j].mark = 0;
      world[i][j].energy = 0;
    }
  }

  
  for(i = 0 ; i < plants ; i++)
  {
    count = 0 ;
    do
    {
      j = random_range(0 , height);
      k = random_range(0 , width);
      printf("j = %d\n" , j);
      printf("k = %d\n" , k);
      printf("count = %d\n" , count);
      count++ ;
    } while((world[j][k].type != 0) && (count < 4)) ;
    world[j][k].type = 1 ;
    world[j][k].energy = 1 ;    
  }

}

void whilecount(void)
{
  int count = 0 ;

  do
  {
    printf("a count = %d\n" , count);
    count++ ;
    printf("b count = %d\n" , count);
  } while(count < 4) ;

}

int main(int argc , char **argv)
{
  init_world();
  //whilecount();
  printf("gsw!\n");
  exit(1);
}
