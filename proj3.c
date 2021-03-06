#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "proj3.h"


int main(int argc, char ** argv)
{
  //initializations
  int numvertix = 0; 
  int numedge = 0;
  int vertix = 0; 
  int xvertix = 0; 
  int yvertix = 0;
  int i = 0;//counter for 1st for loop
  int j = 0;//counter for 2nd for loop
  int edge = 0;
  int vertixcurr = -1;
  int sourceVertix = 0;
  int numquery = 0;
  int a = 1;  
  int vdest = 0;
  VertixNodeList * vsource = NULL;
  VertixNodeList * verdest = NULL;
  VertixNodeList * Big = NULL;
  VertixNodeList * vsearched = NULL;
  EdgeNodeList * addedge = NULL;
  VertixNodeList * vpsearched = NULL;
  EdgeNodeList * tempedge = NULL;
  VertixNodeList * flipprev = NULL;
  VertixNodeList * flipcurr = NULL;
  VertixNodeList * flipnext = NULL;
  
  
  //argv[1] is goint to be the map and argv[2] will be the querry file accring to the document
  const char * mapfile = NULL;
  const char * queryfile = NULL;
  mapfile = argv[1];
  queryfile = argv[2];

  //////////////////////////////////111111111
  //we will begin with map operations 
  //opeing and reading the the map file
  FILE * mapptr = NULL;//map ptr
  mapptr = fopen(mapfile, "r");
  if (mapptr == NULL)
    {
      printf("\nERROR in opeing the MAP\n");
    } 

  fscanf(mapptr, "%d %d\n", &numvertix, &numedge);//get the values for numvertix and edge
  fscanf(mapptr, "%d %d %d\n", &vertix, &xvertix, &yvertix);//get the values 
  //jsut checking here//printf("%d %d", numvertix, numedge);
  //jsut checking here//printf("%d %d %d", vertix, xvertix, yvertix);
  
  VertixNodeList * Headvertix = VertixList_createNode(vertix, xvertix, yvertix);   
  VertixNodeList * vertix1 = Headvertix;
  VertixNodeList * vertix2 = NULL;
  
  for(i = 0; i < (numvertix-1); i++)
    {
      fscanf(mapptr, "%d %d %d\n", &vertix, &xvertix, &yvertix);
      vertix2 = VertixList_createNode(vertix, xvertix, yvertix);      
      vertix1->next = vertix2;
      vertix1 = vertix2;
      //printf("HERE\n");
    }

   fscanf(mapptr, "%d %d\n", &vertix, &edge);

   vertixcurr = vertix;

   //this operation is done to locate the first couple of vertices 
   vsearched = vertixListSearch(Headvertix, vertix);
   vpsearched = vertixListSearch(Headvertix, edge);

   ascendingedge(vsearched, tempedge, addedge, edge);
   ascendingedge(vpsearched, tempedge, addedge, vertix);

   for(j = 0; j < (numedge-1); j++)
    {
      fscanf(mapptr, "%d %d\n", &vertix, &edge);
            
      if(vertixcurr != vertix)//stuck here
	{	  
	  vsearched = vertixListSearch(Headvertix, vertix);
	  vertixcurr = vertix; 
	}    

      vpsearched = vertixListSearch(Headvertix, edge);

      //need to make calls for ascending edge addition function herereere
      ascendingedge(vsearched, tempedge, addedge, edge);
      ascendingedge(vpsearched, tempedge, addedge, vertix);
    }

 
  fclose(mapptr);


  ////////////////////////////////////////////////////////////////////////////
  //now we will perform querry operations
  //just checking // printf("%d\n", numVertices);
  //open the query file
  FILE * qptr = NULL;
  qptr = fopen(queryfile, "r");
  if(qptr == NULL)//error check
    {
      printf("CANNOT OPEN QUERY FILE \n");
    }

  Big = VertixList_createNode((numvertix+1), 1, 1);
  fscanf(qptr, "%d\n", &numquery);//start by getting querries from querry file
   // printf("%d\n", numQueries);
  ////////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////////  


  for(a = 1; a <= numquery; a++)
    {
      fscanf(qptr, "%d %d\n", &sourceVertix, &vdest);
      // printf("%d %d\n", vsource, vdest);
      vsource = vertixListSearch(Headvertix,sourceVertix);
      //check to see if found???
      verdest = vertixListSearch(Headvertix, vdest);     
      vsource->distance = 0;//start from origin     
      shortestpath(Headvertix, Big, vsource, vsource, verdest); 
      printf("\n\n%d\n\n", (int)verdest->distance);//this prints the total distance with origin and destination
      flipcurr = verdest;
      flippath(flipprev, flipcurr, flipnext);
      
      answerpath(vsource);
     
    }

  //close the qptr and deallocate
  fclose(qptr); 

  //check for memory leaksssssss
  destroyAll(Headvertix);
  free(Big);

  return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////
//this function adds all the given edges or roads based on which one is closest or smallest..
//
void ascendingedge(VertixNodeList * vsearched, EdgeNodeList * tempedge, EdgeNodeList * addedge, int valueedge)
{
 
  addedge = EdgeList_createNode(valueedge);//the new edge that needs to be added 
  tempedge = vsearched->firstEdge;

  EdgeNodeList * tempedgeprev = NULL;

  if(tempedge != NULL)
    {
      if(valueedge < tempedge->edge)
	{
	  vsearched->firstEdge = addedge;
	  addedge->next = tempedge;
	}
      else if(tempedge->next == NULL)
	{
	  tempedge->next = addedge;
	}
      else
	{
	  tempedgeprev = tempedge;
	  tempedge = tempedge->next;
	 
	  whichway(vsearched, addedge, valueedge, tempedge, tempedgeprev);      
	}
    }
  else
    {
      vsearched->firstEdge = addedge;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////
//this function looks for the verix with the lease value .
//it is important to note that the vertix has to be one which has not been used before
//to avoid loops
VertixNodeList * FindLeast(VertixNodeList * head, VertixNodeList * vsmall, VertixNodeList * dest)
{
  if((head->visited != 1) && (head->distance < vsmall->distance))
    {
      vsmall = head;
    }

  if(head->next != NULL)
    {
      head = head->next;
      vsmall = FindLeast(head, vsmall, dest);
    }
  return vsmall;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//this is the most important function of the program
//This is basically dijkstra algorithm.
//This function first makes the current vertix the one witht he least edge value
//then calls the search vertix and uclids for=mula function to caculate teh distance between sorigin node and the
//dest node .
//it does so by first searching for the vertic(city) with the specific edge value and then keeps moving towards
//it usend linked list operations
void shortestpath(VertixNodeList * head, VertixNodeList * Big, VertixNodeList * sorigin, VertixNodeList * scurrent, VertixNodeList * dest)
{
  EdgeNodeList * edgeNode = NULL;
  VertixNodeList * nextVertix = NULL;
  int nextDistance = 10001;

  scurrent = FindLeast(head, Big, dest);
  edgeNode = scurrent->firstEdge;

  while(edgeNode != NULL)
    {
      nextVertix = vertixListSearch(head,edgeNode->edge);     
      nextDistance = scurrent->distance + Euclidformula(scurrent,nextVertix);
  
      if(nextDistance < nextVertix->distance)
	{
	  nextVertix->prev = scurrent;
	  nextVertix->distance = nextDistance;
	}
      edgeNode = edgeNode->next;
    }
  scurrent->visited = 1;

  if(dest->visited != 1)
    {
      shortestpath(head, Big, sorigin, scurrent, dest);
    }
}

