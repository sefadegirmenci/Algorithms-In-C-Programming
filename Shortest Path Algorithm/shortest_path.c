#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>


#define INPUT_ARRAY_SIZE 50	 //length of input string
#define CITY_ARRAY_SIZE 5    //number of cities in the city array
#define ROAD_ARRAY_SIZE 5	 //number of roads in the roads array
#define MULTIPLYING_FACTOR 2 //use in realloc
#define MAX_INPUT_SIZE 256	 //maximum chars in user input

const int INF = 1e7;  

int lookup_string(char[30]);

/* Array of vertices. 
 * Each element of the str array contains a city name and 
 * its index in str becomes the city's numeric id.
 */
typedef struct vertices 
{
	// This attribute shows the size of dynamically allocated memory
	int size;
	// This attribute shows the number of cities in the array
	int city_count;
	// This attribute is the array of city names
	char (* str)[30];
} 
vertices;

/* Array of edges. 
 * Each element of the edge array contains the ids of two cities 
 * connected directy by a road and the length of the road.
 */
typedef struct edges 
{
	// This attribute shows the size of dynamically allocated memory
	int size;
	// This attribute shows the number of roads in the array
	int edge_count;
	// This attribute is the array of road information
	int (* edge)[3];
} 
edges;

vertices cities;

edges roads;

/* A two-dimensional array that shows 
 * the length of the shortest path connecting 
 * any two cities.
 */
int **city_graph;

/* A two-dimensional array that shows the direction 
 * to the shortest path between any two cities.
 */
int **shortest_paths;

/* A function that implements Floyd-Warshall algorithm. 
 * This algorithm finds the shortest path between 
 * any two vertices in a given graph.
 */
void floydWarshall()  
{  
	for (int k = 0; k < cities.city_count; k++) 
	{  
		for (int i = 0; i < cities.city_count; i++) 
		{  
			for (int j = 0; j < cities.city_count; j++) 
			{  

				// We cannot cross a road that doesn't exist  
				if ((city_graph[i][k] == INF) || (city_graph[k][j] == INF))  
				{
					continue;
				}  

				if (city_graph[i][j] > (city_graph[i][k] + city_graph[k][j])) 
				{  
					city_graph[i][j] = city_graph[i][k] + city_graph[k][j];  
					shortest_paths[i][j] = shortest_paths[i][k];  
				}  
			}  
		}  
	}  
} 

/* A function that initializes the values in 
 * city_graph and shortest_paths arrays.
 */
void initialise()  
{  
	for (int i = 0; i < cities.city_count; i++) 
	{  
		for (int j = 0; j < cities.city_count; j++) 
		{    

			// No edge between node i and j  
			if (city_graph[i][j] == INF)  
			{
				shortest_paths[i][j] = -1;
			}  
			else
			{
				shortest_paths[i][j] = j;
			}  
		}  
	}  
}

/* A function that inserts the name of a new city 
 * to the cities array.
 */
int insert_to_cities(char str[30]) 
{
	// Write your code here
	assert(str!=NULL);
	// Check if the city name already exists in the array 
	int check_duplicate;
	check_duplicate=lookup_string(str);
	// If it does, return the array index of the city 
	if(check_duplicate!=-1) return check_duplicate;
	// If the city name doesn't exist, it is inserted to the cities array, and
	// return the array index of the city
	if (cities.size>cities.city_count)
	{
		strcpy(&cities.str[cities.city_count++][0],str); // copy city name to cities array
	}

	// If the dynamically allocated size of cities array is not large enough, 
	// the size of cities array is enlarged and the city name is inserted 
	// to the newly added region
	else
	{
		int size_temp;
		size_temp=cities.size;
		cities.str  =realloc(cities.str,size_temp*MULTIPLYING_FACTOR*sizeof(char[30])); //reallocate memory

		cities.size =size_temp*MULTIPLYING_FACTOR;

		strcpy(&cities.str[cities.city_count++][0],str); // copy city name to cities array

	}
	// Return the array index of the city.
	return cities.city_count-1;
}

/* A function that looks up if a city name
 * is already in the cities array. 
 */
int lookup_string(char str[30]) 
{
	for(int i = 0; i < cities.city_count; i++) {
		if(strncmp(str, cities.str[i], 30) == 0)
			return i;
	}
	printf("city %s is not found\n", str);
	return -1;
}

void printPath(int u, int v)  
{  
	int temp_1=u;
	int temp_2=v;
    int length=0;
    //print the first city
	printf("%s ",cities.str[temp_1]); 
	while(shortest_paths[temp_1][temp_2]!=temp_2) //if there are middle cities between first and second city
	{
		printf("%s ",cities.str[shortest_paths[temp_1][temp_2]]); 
		temp_1=shortest_paths[temp_1][temp_2];
	}

    printf("%s\n",cities.str[v]); //print destination
} 


int main(int argc, char *argv[])
{
	// Write your code here
	// Allocate memory regions dynamically to cities array 
	cities=(vertices){
				.size       =CITY_ARRAY_SIZE,
				.city_count =0,
				.str        =malloc(CITY_ARRAY_SIZE*sizeof(char[30])), //cities array
	};
	// and roads array.
	roads=(edges){
				.size 		=CITY_ARRAY_SIZE,
				.edge_count =0,
				.edge 		=malloc(ROAD_ARRAY_SIZE*sizeof(int[3])),//roads array
	};

	// Read and parse the input file. Insert the city names to cities array.
	FILE* input_file;
	char* file_name;

	if(argc == 2) 
	{
    file_name=argv[1];
  	}else printf("Could not get argument");

	input_file=fopen(file_name,"r");
    if(input_file==NULL) printf("Could not open a file");

    char *line = (char*)malloc(sizeof(char)* INPUT_ARRAY_SIZE);

 	while(fgets(line,INPUT_ARRAY_SIZE,input_file))
  	{
		char * token= strtok(line," ");
		if(strlen(token)<=1) continue; //we reached empty line, continue loop
		// The index of the city name in the cities array becomes the city id. 
		// Insert city ids and road lengths to roads array.
		int index_1=insert_to_cities(token);
		roads.edge[roads.edge_count][0]=index_1;
		token=strtok(NULL," ");
	
		int index_2=insert_to_cities(token);
		roads.edge[roads.edge_count][1]=index_2;
		token=strtok(NULL," ");	
		
		int road_length=atoi(token);
		roads.edge[roads.edge_count++][2]=road_length;

		//reallocate memory for roads array
		if(roads.edge_count>=roads.size)
		{
			int size_temp=roads.size;
			roads.edge  =realloc(roads.edge,size_temp*MULTIPLYING_FACTOR*sizeof(int[3])); 

		roads.size =size_temp*MULTIPLYING_FACTOR;
		}
	}
	fclose(input_file);
	free(line);
	// Allocate memory regions dynamically to city_graph, 
	// distance, and shortest_paths arrays.
	city_graph=malloc(cities.size*sizeof(int*));
	shortest_paths=malloc(cities.size*sizeof(int*));
	
	for(int i=0;i<cities.size;i++)
	{
		city_graph[i]=malloc(cities.size*sizeof(int));
	}
	for(int i=0;i<cities.size;i++)
	{
		shortest_paths[i]=malloc(cities.size*sizeof(int));
	}

	// Initialize the values in city_graph array with road lengths, 
	// such that the value in city_graph[i][j] is the road length 
	// between cities i and j if these cities are directly connected 
	// by a road 
	// For cities m and n that are not connected directly by a road, 
	// the value in city_graph[m][n] will be INF, 
  // which is a large value like 10M, 
	// that is assumed to be infinite. 

	/*Make every road INF*/
	for (int i = 0; i < cities.city_count; i++) 
	{  
		for (int j = 0; j < cities.city_count; j++) 
		{    
			// No edge between node i and j  
			city_graph[i][j] = INF;
		}  
	} 
	/*Get city and road info from roads array and initialize city_graph*/

	for (int i = 0; i < roads.edge_count; i++) 
	{  
		
		int city_1=roads.edge[i][0];
		int city_2=roads.edge[i][1];
		int length=roads.edge[i][2];
		city_graph[city_1][city_2] = length;
		city_graph[city_2][city_1] = length;
	} 

	initialise();
	floydWarshall();



	// prompt user to enter two city names
	char* input=malloc(MAX_INPUT_SIZE*sizeof(char));
	if(input==NULL) printf("No memory for input");

	printf("%s","Enter two city names: ");
  	fgets(input, MAX_INPUT_SIZE, stdin);
    
	char* first_city;
	first_city=strtok(input," ");
	char* second_city;
	second_city=strtok(NULL," \n");

  	//get index of each city 
	int first_city_index=lookup_string(first_city);
	int second_city_index=lookup_string(second_city); 

  	//print path between cities if these cities exist and there is a path between them
 	if(first_city_index!=-1&&second_city_index!=-1&&city_graph[first_city_index][second_city_index]!=INF)
 	{
 		printPath(first_city_index,second_city_index);

		// print the length of the path
 		printf("Length: %d\n",city_graph[first_city_index][second_city_index]);

  	}else printf("*** NO PATH ***");	
	
	for(int i=0;i<cities.size;i++)
    {
		free(shortest_paths[i]); //free rows of shortest_paths
 	}
 	free(shortest_paths); //free shortest_paths
 	 
  	free(cities.str); //free cities array
  	free(input); //free input
  	free(roads.edge); //free roads array
  	for(int i=0;i<cities.size;i++)
  	{
  		free(city_graph[i]);
  	}
  	free(city_graph);
  
  	
  	return 0;	
}
