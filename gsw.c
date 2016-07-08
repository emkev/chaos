
/* 2016.05.08 am 8:31 , 
simulating a ecosystem . Maybe it can be called chaos theory . Who knows ! 
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int height = 5 , width = 10 , steps = 20 ;
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
  for(i = 0 ; i < height ; i++)
  {
    world[i] = malloc(sizeof(cell) * width) ;
    for(j = 0 ; j < width ; j++)
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
    //printf("i = %d\n" , i);
    count = 0 ;
    do
    {
      j = random_range(0 , height);
      k = random_range(0 , width);
      //printf("j = %d\n" , j);
      //printf("k = %d\n" , k);
      //printf("count = %d\n" , count);
      count++ ;
    } while((world[j][k].type != EMPTY) && (count < threshold)) ;

    if(world[j][k].type == EMPTY)
    {
      world[j][k].type = PLANT ;
      world[j][k].energy = pt ;
    }
  } /* for(i = 0 ; i < plants ; i++) */

  printf("s1\n");

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

void update_herbs(void)
{
  int i , j , ni , nj , k , r , found ;

  for(i = 0 ; i < height ; i++)
  {
    for(j = 0 ; j < width ; j++)
    {
      /* always operate ~[][] , not easy . whether use pointer ! */
      /* Don't know why use 'mark' in world[][] . */
      if(world[i][j].type != HERB) continue ;

      /* cost energy */
      world[i][j].energy -= ch ;
      if(world[i][j].energy <= 0)
      {
        world[i][j].type = EMPTY ;
        world[i][j].energy = 0 ;
        break;
      }

      found = 0 ;
      /* by using r , to begin to rotate from random direction */
      /* r = random_range(0 , 8); */
      for(k = 0 ; k < 8 ; k++)
      {
        ni = (i + neighs[k][0] + height) % height ;
        nj = (j + neighs[k][1] + width) % width ;

        if(world[ni][nj].type == PLANT)
	{
          found = 1 ;
          break;
	} 
      } /* for(k = 0 ; k < 8 ;  */

      /* if found , then eat */
      if(found)
      {
        world[ni][nj].type = HERB ;
        world[ni][nj].energy = world[i][j].energy + ep ;

        /* make a baby if possible */
        if(world[ni][nj].energy > eh)
	{
          world[ni][nj].energy /= 2 ;
          world[i][j] = world[ni][nj] ;
	}
        /* move from old location [i,j] to the new [ni,nj] */
        else
	{
          world[i][j].type = EMPTY ;
          world[i][j].energy = 0 ;
	}
      } /* if(found) */
      /* randomly move to a empty loaction */
      else
      {
        r = random_range(0 , 8) ;
        for(k = 0 ; k < 8 ; k++ , r = (r + 1) % 8)
	{
          ni = (i + neighs[r][0] + height) % height ;
          nj = (j + neighs[r][1] + width) % width ;
	}
        if(world[ni][nj].type == EMPTY)
	{
          world[ni][nj] = world[i][j] ;
          world[i][j].type = EMPTY ;
          world[i][j].energy = 0 ;
          break ;
	}
      }

    } /* for(j = 0 ; j < width ;  */
  } /* for(i = 0 ; i < height ;  */ 
}

void update_carns(void)
{
  int i , j , ni , nj , k , r , found ;

  for(i = 0 ; i < height ; i++)
  {
    for(j = 0 ; j < width ; j++)
    {
      if(world[i][j].type != CARN) continue;

      world[i][j].energy -= cc ;
      if(world[i][j].energy <= 0)
      {
        world[i][j].type = EMPTY ;
        world[i][j].energy = 0 ;
        break ;
      }

      found = 0 ;
      r = random_range(0 , 8) ;
      for(k = 0 ; k < 8 ; k++ , r = (r + 1) % 8)
      {
        ni = (i + neighs[r][0] + height) % height ;
        nj = (j + neighs[r][1] + width) % width ;
        if(world[ni][nj].type == HERB)
	{
          found = 1 ;
          break ;
	} 
      }

      if(found)
      {
        world[ni][nj].type = CARN ;
        world[ni][nj].energy = world[i][j].energy + eh ;
        /* make a baby if possible  */
        if(world[ni][nj].energy > ec)
        {
          world[ni][nj].energy /= 2 ;
          world[i][j] = world[ni][nj] ;
	}
        else
	{
          world[i][j].type = EMPTY ;
          world[i][j].energy = 0 ;
	}
      } /* if(found)  */
      else /* no found , just move a random no-herb loaction .  */
      {
        r = random_range(0 , 8) ;
        for(k = 0 ; k < 8 ; k++ , r = (r + 1) % 8)
	{
          ni = (i + neighs[r][0] + height) % height ;
          nj = (j + neighs[r][1] + width) % width ;
          /* */
          if(world[ni][nj].type == EMPTY || world[ni][nj].type == PLANT)
	  {
            found = world[ni][nj].type ;
            world[ni][nj] = world[i][j] ;
            world[i][j].type = found ;

            if(found == EMPTY)
	    {
              world[i][j].energy = 0 ;
	    }
            break ;
	  }

	} /* for(k = 0 ; k < 8 ;   */
      }


    } /* for(j = 0 ; j < width ; j++)  */
  } /* for(i = 0 ; i < width ; i++)  */
}

void plot(void)
{
  int i , j ;

  for(i = 0 ; i < height ; i++)
  {
    for(j = 0 ; j < width ; j++)
    {
      if(world[i][j].type == EMPTY)
        printf(" ");
      else if(world[i][j].type == PLANT)
        printf("#");
      else if(world[i][j].type == HERB)
        printf("&");
      else if(world[i][j].type == CARN)
        printf("@");
    } /* for(j = 0 ; j < width ; j++)  */

    printf("\n");
  } /* for(i = 0 ; i < height ; i++)   */

}

/* 2016.07.07 */
void plot_curve(void)
{
  int a , points_num = 3 ;
  int i , j , tmpnum = 0 ;
  char tmpname ;
  char min , mid , max ;
  int **points ;

  points = malloc(sizeof(int *) * 2) ;
  points[0] = malloc(sizeof(char) * 3) ;
  points[1] = malloc(sizeof(int) * 3) ;

  points[0][0] = 'p' ;
  points[1][0] = plants ;
  points[0][1] = 'h' ;
  points[1][1] = herbs ;
  points[0][2] = 'c' ;
  points[1][2] = carns ;

  //printf("points[0][2] = %c\n" , points[0][2]);
  //printf("points[1][2] = %d\n" , points[1][2]);

  /* order "plants and herbs and carns" . 
     Don't forget to test the ordering algorithm .
   */
  a = points_num - 1 ;
  while(a > 0)
  {
    for(i = 0 ; i < a ; i++)
    {
      if(points[1][i] > points[1][i + 1])
      {
        tmpname = points[0][i] ;
        tmpnum  = points[1][i] ;
        points[0][i] = points[0][i + 1] ;
        points[1][i] = points[1][i + 1] ;
        points[0][i + 1] = tmpname ;
        points[1][i + 1] = tmpnum ; 
      }

    } /* for(i = 0 ; i < a ;   */

    a-- ;

  } /* while(a > 0)  */

  //printf("points[1][0] = %d\n" , points[1][0]);
  //printf("points[1][1] = %d\n" , points[1][1]);
  //printf("points[1][2] = %d\n" , points[1][2]);

  /* thinking :

  for(i = 0 ; i < max ; i++)
  {
    while(i < min)
    {

    }
  }

  while(i < min)
  {
    printf(" ");
    i++ ;
  }
  printf("%c" , min);

  while(i < mid)
  {
    printf(" ");
    i++ ;
  }
  printf("%c" , mid);

  while(i < max)
  {
    printf(" ");
    i++ ;
  }
  printf("%c" , max);
  */

  // way 2
  j = 0 ;
  /* make sure clearing all 0 value points  */
  while(j <= points_num - 1 && points[1][j] == 0)
  {
    j++ ;
  }

  i = 1 ;
  while((j <= points_num - 1)
        && (i <= points[1][points_num - 1]) 
        && (points[1][j] > 0))
  {
    if(i != points[1][j])
    {
      printf(" ");
    }
    else
    {
      if(points[0][j] == 'p')
        printf("#");
      else if(points[0][j] == 'h')
        printf("&");
      else if(points[0][j] == 'c')
        printf("@");

      j++ ;

    } /* else  */
    
    i++ ;

  } /* while(i <= points[1][2])  */

  printf("\n");

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

void random_wheel(int spos)
{
  int r , j , k ;
  int a , b , c ;

  //r = random_range(0 , 8) ;

  /* NOTE ! Loop beginning by r , not (r+1) . NOTE !  */

  /*
     r = (r + 1) % 8 , TO turn the wheel !
     r = (r + 8) % 8 , FOR the edge question ! 
   */

  for(k = 0 , j = spos , r = j % 8 ; 
      k < 8 ; 
      k++ , r = (j + 1) % 8 , j++)
  {
    printf("j = %d " , j);
    printf("r = %d\n" , r);
  }

  printf("\n");

  c = spos % 8 ;
  printf("turning c = %d\n" , c);
  for(a = 0 ; a < 8 ; a++ , c = (c + 1) % 8 )
  {
    printf("c= %d\n" , c);
  }

}

int main(int argc , char *argv[])
{
  int arglen , argnum , count ;
  int t , i , j ;

  //whilecount();

  arglen = argnum = argc - 1 ;
  count = 1 ;
  printf("arglen = %d\n" , arglen);
  
  while(arglen > 0)
  {
    /* argv[count] is the first-address of multiple string-array-factors . */
    printf("argue-%d = %s\n" , count , argv[count]);
    arglen-- ;
    count++ ;
  }

  printf("\n");

  /* strncmp : string comparing function . 
     if same , return 0 , 
     if different , return >0 or <0 .
   */

  if(argnum == 2 && strncmp(argv[1], "wheel" , 5) == 0)
  {
    printf("wheel\n");
    /* atoi , convert string to int (%d) . 
       atol , convert string to long int (%ld) .
     */
    printf("%d\n" , atoi(argv[2]));
    random_wheel(atoi(argv[2]));
  }
  
  /* try to convert int to string by itoa() , but 
     the function is not support in C standard library .
    */
  //printf("%s\n" , itoa(count));

  if(argnum == 7 && strncmp(argv[1] , "set" , 3) == 0)
  {
    steps = atoi(argv[2]) ;
    height = atoi(argv[3]) ;
    width = atoi(argv[4]) ;
    plants = atoi(argv[5]) ;
    herbs = atoi(argv[6]) ;
    carns = atoi(argv[7]) ;
  }

  if(argnum == 2 && strncmp(argv[1] , "steps" , 5) == 0)
  {
    steps = atoi(argv[2]) ;
  }

  // 2016.07.06
  if(argnum == 1 && strncmp(argv[1] , "help" , 4) == 0)
  {
    printf("set steps height width plants herbs carns\n");
    exit(1);
  }

  init_world();

  for(t = 0 ; t < steps ; t++)
  {
    plants = herbs = carns = 0 ;

    for(i = 0 ; i < height ; i++)
    {
      for(j = 0 ; j < width ; j++)
      {
        if(world[i][j].type == PLANT) plants++ ;
        else if(world[i][j].type == HERB) herbs++ ;
        else if(world[i][j].type == CARN) carns++ ;

      } /* for(j = 0 ; j < width ; j++)  */
    } /* for(i = 0 ; i < height ; i++)  */

    // plot mode : plot();
    plot_curve();
    //printf("%d , %d , %d\n" , plants , herbs , carns);

    update_plants();
    update_herbs();
    update_carns();

  } /* for(t = 0 ; t < steps ; t++)  */

  printf("gsw!\n");
  exit(1);
}
