
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

/* array of eight directions */
int neighs[8][2] = {
  {-1 , -1} , {-1 , 0} , {-1 , 1} ,
  {0 , -1 } ,            { 0 , 1} ,
  {1 , -1 } , { 1 , 0} , { 1 , 1} 
};

int random_range(int min , int max)
{
  int a ;
  
  a = random() % (max - min) ;

  return a ;
}

/* init plants , herbs , and carns */
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

  /* add herbs */
  for(i = 0 ; i < herbs ; i++)
  {
    do
    {
      j = random_range(0 , height);
      k = random_range(0 , width);
    } while(world[j][k].type != EMPTY) ;
    world[j][k].type = HERB ;
    world[j][k].energy = eh ;  
  }

  /* add carns */
  for(i = 0 ; i < carns ; i++)
  {
    do
    {
      j = random_range(0 , height);
      k = random_range(0 , width);
    } while(world[j][k].type != EMPTY) ;
    world[j][k].type = CARN ;
    world[j][k].energy = ec ;
  }

}

void update_plants(void)
{
  int i , j , ni , nj , nni , nnj , count ;

  for(i = 0 ; i < height ; i++)
  {
    for(j = 0 ; j < width ; j++)
    {
      /* whether is for performance by this checking way . */
      if(world[i][j].type != EMPTY) continue ;

      count = 0 ;

      /* record plants sum of eight directions of current location */
      for(ni = -1 ; ni <= 1 ; ni++)
      {
        for(nj = -1 ; nj <= 1 ; nj++)
	{
          nni = (i + ni + height) % height ;
          nnj = (j + nj + width) % width ;

          if(world[nni][nnj].type == PLANT)
            count++ ;

	} /* for(nj = -1 ; ... */
      } /* for(ni = -1 ; ... */

      /* plants sum in allowed domain(pmin , pmax) , 
         AND the energy of the location allow to grow a new plant . */
      if(count >= pmin && count <= pmax && world[nni][nnj].energy >= pt)
        world[i][j].type = NEWPLANT ;

    } /* for(j = 0 ; j < width ; j++) */

  } /* for(i = 0 ; i < height ; i++) */

  for(i = 0 ; i < height ; i++)
  {
    for(j = 0 ; j < width ; j++)
    {
      if(world[i][j].type == NEWPLANT)
        world[i][j].type = PLANT ;
      /* if empty , energy++ to feed land . :-) */
      else if (world[i][j].type == EMPTY)
        world[i][j].energy++ ;

    } /* for(j = 0 ; j < width ; ... */

  } /* for(i = 0 ; i < height ; ... */
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
