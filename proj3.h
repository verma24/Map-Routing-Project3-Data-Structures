#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

//Data structures
typedef struct ListNodeVertix
{
  int vertix;
  int visited;
  double distance;
  int x;
  int y;
  struct ListNodeVertix * prev;
  struct ListNodeVertix * next;
  struct ListNodeEdge * firstEdge;
} VertixNodeList;

//Data Structure for List of Edges, pointed at by each vertix
typedef struct ListNodeEdge
{
  int edge;
  struct ListNodeEdge * next;
} EdgeNodeList;


/////////////////////////////////////////////////////////////////////////////////////////
//function definitions
void ascendingedge(VertixNodeList * svertix, EdgeNodeList * tempedge, EdgeNodeList * addedge, int valueedge);
void whichway(VertixNodeList * vertixSearched, EdgeNodeList * edgeAdd, int edgeValue, EdgeNodeList * edgeTemp, EdgeNodeList * edgeTempPrev);
VertixNodeList * vertixListSearch(VertixNodeList * head, int vertix);
void destroyEdges(EdgeNodeList * edgeNode);
void destroyAll(VertixNodeList * vertixHead);
VertixNodeList * FindLeast(VertixNodeList * head, VertixNodeList * vsmall, VertixNodeList * destin);
double Euclidformula(VertixNodeList * source, VertixNodeList * destination);
void shortestpath(VertixNodeList * head, VertixNodeList * Big, VertixNodeList * sorigin, VertixNodeList * scurrent, VertixNodeList * dest);
void flippath(VertixNodeList * prev, VertixNodeList * curr, VertixNodeList * next);
void answerpath(VertixNodeList * sourceNode);

///////////////////////////////////////////////////////////////////////////////////////////




//////////////////////////////////////////////////////
//this function is just used for deallocating
//frees all the edges we malloced
void destroyEdges(EdgeNodeList * edgeNode)
{
  if(edgeNode->next != NULL)
    {
      destroyEdges(edgeNode->next);
    }
  
  free(edgeNode);
}

//////////////////////////////////////////////////////////////////
//this function frees all the data values we used for head vertix
//the values we definied in out vertix list structure

void destroyAll(VertixNodeList * vertixHead)
{
  if(vertixHead->next != NULL)
    {
      destroyAll(vertixHead->next);
    }
  if(vertixHead->firstEdge != NULL)
    {
      destroyEdges(vertixHead->firstEdge);
    }
  
  free(vertixHead);
}


/////////////////////////////////////////////////////////////////////////////////////
//this function creates a new vertix list node by mallocing the required space for it
//and assigns the values fo the x and y coordinates to its vertixnodelist parameters
VertixNodeList * VertixList_createNode(int vertix, int x, int y)
{
  
  VertixNodeList * vertixptr = NULL;//List of vertix 
  vertixptr = malloc(sizeof(VertixNodeList));//allocate the space for it

  //VALUES
  vertixptr->vertix = vertix;
  vertixptr->x = x;
  vertixptr->y = y;  
  vertixptr->distance = 10001;
  vertixptr->visited = 0;
  vertixptr->firstEdge = NULL;
  
  //LINKED LIST OPERATIONS
  vertixptr->next = NULL;
  vertixptr->prev = NULL;
   
  return vertixptr;
}

/////////////////////////////////////////////////////////////////////////////
//This function wroks in the same way as Vertix list does.
//it takes in the given edge and allocates space for it and assigns its values
//to the given parameters
EdgeNodeList * EdgeList_createNode(int edge)
{
  EdgeNodeList * edgeptr = NULL;
  edgeptr = malloc(sizeof(EdgeNodeList));
  //VALUE OPERATION
  edgeptr->edge = edge; 
  //LINKED LIST OPERATION
  edgeptr->next = NULL;
 
  return edgeptr;
}

/////////////////////////////////////////////////////////////////////
//this function searches for the particular vertices using recursion.
//if not found it moves on to the next one
VertixNodeList * vertixListSearch(VertixNodeList * head, int vertix)
{
  if(head->vertix == 0)
    {
      // printf("HEAD-> VERTIX IS NULL\n");
    } 
  if(head->vertix == vertix)
    {
      //printf("VERTIX found");
      return head;
    }

  else if(head->next != NULL)
    {
      //recursive call here;
      head = vertixListSearch(head->next, vertix);
    }

  //printf("vertix =  %d", head->vertix);
  return head;
}


////////////////////////////////////////////////////////////////////////////////////
//This function compares the values of the edges and distances before adding them in 
//the smallest to biggest manner.
//these comparisons are very important as this function is helps the ascendingedge function 
//to find the next route towards the city ie. veritx
void whichway(VertixNodeList * vsearched, EdgeNodeList * addedge, int valueedge, EdgeNodeList * tempedge, EdgeNodeList * edgeTempPrev)
{
  //donot check for null here
  if(valueedge == tempedge->edge)
    {
      addedge->next = tempedge->next;
      tempedge->next = addedge;


        /* if(edgevalue == NULL)
	{
	  printf("vertix2 not working in main \n\n");
	}
	*/
    }

  else if(valueedge < tempedge->edge)//this case when edge is behing the right p[ath
    {
      edgeTempPrev->next = addedge;
      addedge->next = tempedge;
    }
  else if(tempedge->next == NULL)
    {
      tempedge->next = addedge;
    }
  else if(tempedge->next != NULL)
    {
      whichway(vsearched, addedge, valueedge, tempedge->next, tempedge);	  
    }
  
}

//////////////////////////////////////////////////////////////////////////
//this function flips the path of from the destination to source
//this is required to do to get the path from source to destination
void flippath(VertixNodeList * previous, VertixNodeList * current, VertixNodeList * next)
{

  if(current == NULL)
    {
      printf("some ERROR\n\n in flipping");
    }

  while(current != NULL)
    {
      next  = current->prev;  
      current->prev = previous;   
     


      previous = current;
      current = next;
    }
}

////////////////////////////////////////////////////////////////////////////////
//this function simply prints the path after we have reversed it 
//the path is from the origin(source) to the destination
void answerpath(VertixNodeList * vsource)
{
  if(vsource != NULL)
    {
      printf("%d ", vsource->vertix);   
      vsource = vsource->prev;
      answerpath(vsource);
    }
}

//////////////////////////////////////////////////////////////////////////////////////
//this is the function which returns the value from the euclid formula as we all know
//
double Euclidformula(VertixNodeList * source, VertixNodeList * destination)
{
  double value = 0;

  value = pow((pow((destination->x - source->x),2))+(pow((destination->y - source->y),2)),0.5);

  // printf("%d", (int)(value));

  return value;
}


