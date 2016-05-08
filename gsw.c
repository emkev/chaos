
/* 2016.05.08 am 8:31 , 
simulating a ecosystem . Maybe it can be called chaos theory . Who knows ! 
*/

#include <stdio.h>
#include <stdlib.h>

int height = 5 , width = 10 , steps = 3 ;
int plants = 10 , herbs = 4 , carns = 2 ;

/* energy values */
int ep = 15 , eh = 50 , ec = 200 ;
/* energy cost range of herbs and carns , and energy boundary of plants . */
int pt = 0 , ch = 5 , cc = 5 ;
/* plants growing range values */
int pmin = 2 , pmax = 4 ;

/* whether allow extinctions . 
   I think it should allow extinctions in normal case .
 */
int noext = 0 ;

#define EMPTY 0
#define PLANT 1
#define HERB  2
#define CARN  3
#define NEWPLANT 4

/* each unit in world */
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
  int count , threshold ;

  world = malloc(sizeof(cell *) * height) ;
  for(i = 0 ; i < 4 ; i++)
  {
    world[i] = malloc(sizeof(cell) * width) ;
    for(j = 0 ; j < 4 ; j++)
    {
      world[i][j].type = EMPTY;
      world[i][j].mark = 0;
      world[i][j].energy = 0;
    }
  }

  /* ready a threshold(20%) to limit times active 
     to find out " empty " .
     It means that finding 20% chances in entity area .
     It also can be set to three threshold for plants , herbs , and carns . 
   */
  threshold = (int)(height * width / 5) ;

  for(i = 0 ; i < plants ; i++)
  {
    printf("i = %d\n" , i);
    count = 0 ;
    do
    {
      j = random_range(0 , height);
      k = random_range(0 , width);
      printf("j = %d\n" , j);
      printf("k = %d\n" , k);
      printf("count = %d\n" , count);
      count++ ;
    } while((world[j][k].type != EMPTY) && (count < threshold)) ;

    if(world[j][k].type == EMPTY)
    {
      world[j][k].type = PLANT ;
      world[j][k].energy = 1 ;
    }
  } /* for(i = 0 ; i < plants ; i++) */

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
