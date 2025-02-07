/*
*Name : Abdallah AboShoaib
*ID : 1210211
* Ins : Anas Arram
* Sec : 5
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>
#define MAX 100
#define INF 9999999
struct node
{
    char city[MAX];
    int distance;
    struct node* next;
};
typedef struct node *List;

struct g
{
    int numOfVertices;
    List *adjList;

};
typedef struct g *Graph;
// Function to create a new node
List creatNode(char city[MAX],int distance)
{
    List newNode = (List)malloc(sizeof(struct node));
    strcpy(newNode->city,city);
    newNode->distance = distance;
    newNode->next = NULL;
    return newNode;

}
// Function to create a new graph
Graph creatGraph(int vertices)
{
    Graph newGraph = (Graph)malloc(sizeof(struct g));
    newGraph->numOfVertices = vertices;
    newGraph->adjList = (List)malloc(sizeof(struct node));

    for(int i =0 ; i < vertices; i++)
        newGraph->adjList[i] = NULL;

    return newGraph;

}
// Function to add an edge to the graph
void addEdge(Graph graph, int src, int dest, int distance)
{
    List newNode = creatNode(dest,distance);
    newNode->next = graph->adjList[src];
    graph->adjList[src] = newNode;
}
// Function to find the index of a city in the graph
int findCity(char city[MAX], Graph graph)
{
    for(int i = 0 ; i < graph->numOfVertices ; i++)
    {
        if(strcmp(city, graph->adjList[i]->city)== 0)
            return i;
    }
    return -1;

}
// Function to insert a new node at the end of the linked list
void insertLast(List *head, char city[MAX], int distace)
{
    List newNode = creatNode(city, distace);

    List temp = *head;

    while(temp->next != NULL)
    {
        temp = temp->next;
    }

    temp->next = newNode;


}
// Function to load cities from a file and construct the graph
void LoadCities(Graph graph)
{
    FILE *in = fopen("Cities.txt","r");
    if(in == NULL)
    {
        printf("Error\n");
        return;

    }
    graph->numOfVertices = 0;
    char city[MAX], adj[MAX];
    int distance;
    int i = 0;
        // Read data from the file and construct the graph
    while(fscanf(in, "%s %s %dkm\n", city, adj,&distance)!=EOF)
    {
        if(graph->numOfVertices == 0)
        {    // First city encountered, create a node and add adjacent city
            graph->adjList[i] = creatNode(city, 0);
            graph->adjList[i]->next = creatNode(adj, distance);
            graph->numOfVertices++;
            i++;

        }
        else
        {
            // Check if the city already exists in the graph
            int R = findCity(city,graph);
            if(R != -1 )
            {    // (City found) add adjacent city to the existing linked list
                insertLast(&(graph->adjList[R]), adj,distance);

            }
            else
            {
                // (City not found) create a new node and add adjacent city
                graph->adjList[i] = creatNode(city, 0);
                graph->adjList[i]->next = creatNode(adj, distance);
                graph->numOfVertices++;
                i++;

            }


        }
    }


}
// Function to print the graph
void printGraph(Graph graph)
{
    if (graph == NULL)
    {
        printf("Graph is NULL.\n");
        return;
    }

    printf("Graph:\n");
    for (int i = 0; i < graph->numOfVertices; i++)
    {
        printf("Vertex %d: %s\n", i, graph->adjList[i]->city);
    }
}
// Function to find the shortest path using Breadth First Search (BFS)
int BFS(Graph graph, int startVertex, int destIndex , char result[MAX])
{
    bool* visited = (bool*)malloc(graph->numOfVertices * sizeof(bool));
    int* parent = (int*)malloc(graph->numOfVertices * sizeof(int));
    int* distances = (int*)malloc(graph->numOfVertices * sizeof(int));
    for (int i = 0; i < graph->numOfVertices; i++)
    {
        visited[i] = false;
        parent[i] = -1;
        distances[i] = -1;
    }

    int* queue = (int*)malloc(graph->numOfVertices * sizeof(int));
    int front = 0, rear = 0;

    visited[startVertex] = true;
    queue[rear++] = startVertex;
    distances[startVertex] = 0;

    bool found = false;

    while (front != rear)
    {
        int currentVertex = queue[front++];

        if (currentVertex == destIndex)
        {
            found = true;
            break;
        }

        List adjNode = graph->adjList[currentVertex]->next;

        while (adjNode != NULL)
        {
            int adjVertex = findCity(adjNode->city, graph);
            if (!visited[adjVertex])
            {
                visited[adjVertex] = true;
                parent[adjVertex] = currentVertex;
                distances[adjVertex] = distances[currentVertex] + 1;
                queue[rear++] = adjVertex;
            }
            adjNode = adjNode->next;
        }
    }

    if (found)
    {
        printf("BFS Shortest Path: ");
        int currentVertex = destIndex;
        printf("(%s)", graph->adjList[currentVertex]->city);
        strcpy(result,graph->adjList[currentVertex]->city);
        while (parent[currentVertex] != -1)
        {
            printf(" <- (%s)", graph->adjList[parent[currentVertex]]->city);
            strcat(result,"<-");
            strcat(result,graph->adjList[parent[currentVertex]]->city);

            currentVertex = parent[currentVertex];
        }
        printf("\n");
        printf("Distance: %d\n", distances[destIndex]);
      strcat(result,"   Total Distance:  ");


    }
    else
    {
        printf("No path found between the cities using BFS.\n");
    }

    free(visited);
    free(parent);
    free(distances);
    free(queue);
    return  distances[destIndex];
}
// Function to find the shortest path using Dijkstra's algorithm
int Dijkstra(Graph graph, int startVertex, int destIndex, char result[MAX]) {
    int* distances = (int*)malloc(graph->numOfVertices * sizeof(int));
    int* previous = (int*)malloc(graph->numOfVertices * sizeof(int));
    bool* visited = (bool*)malloc(graph->numOfVertices * sizeof(bool));

    for (int i = 0; i < graph->numOfVertices; i++) {
        distances[i] = INF;
        previous[i] = -1;
        visited[i] = false;
    }

    distances[startVertex] = 0;

    for (int count = 0; count < graph->numOfVertices - 1; count++) {
        int minDistance = INF, minIndex = -1;

        for (int v = 0; v < graph->numOfVertices; v++) {
            if (!visited[v] && distances[v] <= minDistance) {
                minDistance = distances[v];
                minIndex = v;
            }
        }

        visited[minIndex] = true;

        List adjNode = graph->adjList[minIndex]->next;
        while (adjNode != NULL) {
            int adjVertex = findCity(adjNode->city, graph);

            if (!visited[adjVertex] && distances[minIndex] != INF &&
                distances[minIndex] + adjNode->distance < distances[adjVertex]) {
                distances[adjVertex] = distances[minIndex] + adjNode->distance;
                previous[adjVertex] = minIndex;
            }

            adjNode = adjNode->next;
        }
    }

    if (distances[destIndex] != INF) {
        printf("Dijkstra's Shortest Path: ");
        int currentVertex = destIndex;
        printf("%s : %dKM", graph->adjList[currentVertex]->city, distances[currentVertex]);
        strcpy(result, graph->adjList[currentVertex]->city);
        strcat(result, ":");
        char distanceStr[10];
        sprintf(distanceStr, "%d", distances[currentVertex]);
        strcat(result, distanceStr);
        while (previous[currentVertex] != -1) {
                  int prevVertex = previous[currentVertex];
            printf(" <- %s : %dKM", graph->adjList[prevVertex]->city, distances[prevVertex]);
            strcat(result, " <- ");
            strcat(result, graph->adjList[prevVertex]->city);
            sprintf(distanceStr, "%d", distances[prevVertex]);
            strcat(result, ":");
            strcat(result, distanceStr);
            currentVertex = prevVertex;
        }
        printf("\n");
        printf("Total Distance: %d KM\n", distances[destIndex]);
         strcat(result,"   Total Distance:  ");

    } else {
        printf("No path found between the cities using Dijkstra's algorithm.\n");
    }

    free(distances);
    free(previous);
    free(visited);
    return distances[destIndex];
}

// Function to print the shortest path between two cities
int printShortestPath(Graph graph,char source[MAX],char destination[MAX] , char result1[MAX], char result2[MAX],int *dis1  , int *dis2)
{
    int srcIndex, destIndex;

    srcIndex = findCity(source, graph);
    destIndex = findCity(destination, graph);

    if (srcIndex == -1 || destIndex == -1)
    {
        printf("One or both cities not found.\n");
        return;
    }

   (*dis2) = BFS(graph, srcIndex, destIndex ,result2 );

    printf("--------------------------\n");
    (*dis1) =  Dijkstra(graph, srcIndex, destIndex, result1);

}

int main()

{
    Graph graph = creatGraph(0);
    char source[MAX], destination[MAX] , result1[MAX],result2[MAX];
    int choice;
    int dis1 , dis2;
    FILE *out;



     do {
          printf("-------------- MENU ------------\n");
          printf("1. Load cities\n");
          printf("2. Enter source\n");
          printf("3. Enter destination\n");
          printf("4. Exit\n");
          printf("--------------------------\n");
          printf("Enter your choice: ");
          scanf("%d", &choice);

          switch (choice) {
              case 1:
                   LoadCities(graph);
                  if (graph != NULL)
                      printf("----------------------------\n");
                      printf("Cities loaded successfully.\n");
                      printf("----------------------------\n");
                      printGraph(graph);
                  printf("----------------------------\n");

                  break;
              case 2:
                  if (graph == NULL) {
                      printf("Please load cities first.\n");
                      break;
                  }
                   printf("Enter source city: ");
                     scanf("%s", source);
                    printf("----------------------------\n");

                  break;
              case 3:
                  if (graph == NULL) {
                      printf("Please load cities first.\n");
                      break;
                  }
                   printf("Enter destination city: ");
                   scanf("%s", destination);
                  printf("----------------------------\n");
                  printShortestPath(graph,source,destination, result1,result2,&dis1,&dis2);
                  break;
              case 4:
                  out = fopen("shortest_distance.txt","w");
                  fprintf(out ,"Dijkstra's Shortest Path: \n");
                  fprintf(out , "%s %dKM\n" , result1, dis1);
                  fprintf(out , "BFS Shortest Path: \n");
                  fprintf(out , "%s %d\n" , result2, dis2);
                  printf("Shortest path written to the file 'shortest_distance.txt'.\n");

                  break;
              default:
                  printf("Invalid choice. Please try again.\n");
                  break;
          }
      } while (choice != 4);

    return 0;
}

