#include <iostream>
#include <cstdlib>
#include <pthread.h>
#include <vector>
#include <utility>

#define SRAND_VALUE 1985
#define N_THREADS 4
#define N_GENERATIONS 2000

//#define DEBUG

#ifdef DEBUG
	#define P_DEBUG(x) x
#else
	#define P_DEBUG(x)
#endif

using namespace std;

struct PT_Args
{
	vector<vector<bool>>* Grid;
	vector<vector<bool>>* New_Grid;
	int X, Y;
};

//==================================================================================================================================

void Grid_Init(vector<vector<bool>> &Grid)
{
	srand(SRAND_VALUE);

	for(unsigned int i = 0; i < Grid.size(); i++) 
		for(unsigned int j = 0; j < Grid[i].size(); j++) 
	    	Grid[i][j] = rand() % 2;
}

//--------------------------------------------------------------------------

int Neighbours_Count(vector<vector<bool>> &Grid, int X, int Y)
{
	int Nb_Total = 0;													//Qnt. of Neighbours
	vector <pair <int, int>> Nb_List;									//Vector of Neighbours coordinates for future validation

	for(int i = X-1; i <= X+1; i++) 									//Analyses all squares around current X,Y
		for(int j = Y-1; j <= Y+1; j++)
			if	(!(i == X && j == Y))
				Nb_List.push_back(make_pair(i, j));						//Adds the neighbours coordinates to the vector


	for (int i = 0; i < 8; i++)											//Analyses each neighbour to check if the coordinates are valid and, if not, fixes them
	{
		if (Nb_List[i].first < 0)										//Validate X coordinate
			Nb_List[i].first = Grid.size()-1;
		else if (Nb_List[i].first > (signed int) Grid.size()-1)
			Nb_List[i].first = 0;

		if (Nb_List[i].second < 0)										//Validade Y coordinate
			Nb_List[i].second = Grid.size()-1;
		else if (Nb_List[i].second > (signed int) Grid.size()-1)
			Nb_List[i].second = 0;
	}

	for (int i = 0; i < 8; i++)											//Verify how many neighbours are alive
		if (Grid[Nb_List[i].first][Nb_List[i].second] == 1)				//Access Cell Status for each Neighbour 
			Nb_Total++;

	return Nb_Total;
}

//--------------------------------------------------------------------------

int Cells_Total(vector<vector<bool>> &Grid)
{
	int Cells_Sum = 0;

	for(unsigned int i = 0; i < Grid.size(); i++) 
		for(unsigned int j = 0; j < Grid[i].size(); j++) 
	    	if (Grid[i][j] == 1)
	    		Cells_Sum++;

	return Cells_Sum;
}

//--------------------------------------------------------------------------

void *Cell_Update(void *Args)
{
	vector<vector<bool>> *Grid, *New_Grid;
	int X, Y;

	Grid = ((struct PT_Args*) Args)->Grid;
	New_Grid = ((struct PT_Args*) Args)->New_Grid;						//Extracts data from Args Struct
	X = ((struct PT_Args*) Args)->X;
	Y = ((struct PT_Args*) Args)->Y;

	int Nb_Count = Neighbours_Count(*Grid, X, Y);

	if ((*Grid)[X][Y] == 0)
	{
		if (Nb_Count == 3)
		{
			(*New_Grid)[X][Y] = 1;
			free(Args);
			pthread_exit(NULL);
		}
	}
	else
	{
		if (Nb_Count < 2 || Nb_Count >= 4)
		{
			(*New_Grid)[X][Y] = 0;
			free(Args);
			pthread_exit(NULL);
		}		
	}

	(*New_Grid)[X][Y] = (*Grid)[X][Y];

	free(Args);
	pthread_exit(NULL);
}

//--------------------------------------------------------------------------

void Grid_Update(vector<vector<bool>> &Grid, vector<vector<bool>> &New_Grid)
{
	pthread_t Threads[N_THREADS];
	int X, Y;
	int PT_Count = 0;
	struct PT_Args *Args;


	for (X = 0; X < (int)Grid.size(); X++)
	{
		for (Y = 0; Y < (int)Grid.size(); Y++)
		{
			if (PT_Count % N_THREADS == 0)
				for (int i = 0; i < N_THREADS; i++)
					pthread_join(Threads[i], NULL);

			Args = (struct PT_Args*) malloc(sizeof(struct PT_Args));
			Args->X = X;
			Args->Y = Y;
			Args->Grid = &Grid;
			Args->New_Grid = &New_Grid;

			pthread_create(&Threads[PT_Count % N_THREADS], NULL, Cell_Update, (void*) Args);

			PT_Count++;
		}
	}

	for (int i = 0; i < N_THREADS; i++)
		pthread_join(Threads[i], NULL);
}

//==================================================================================================================================

int main()
{
	int N;

	cout << "Enter the desired matrix order:" << endl;
	cin >> N;

	vector<vector<bool>> Grid (N, vector<bool> (N));
	vector<vector<bool>> New_Grid (N, vector<bool> (N));

	Grid_Init(Grid);

	cout << "Generation 0: " << Cells_Total(Grid) << endl; 

	for (int i = 0; i < N_GENERATIONS; i++)
	{
		Grid_Update(Grid, New_Grid);
		Grid = New_Grid;
		cout << "Generation " << i << ": " << Cells_Total(Grid) << endl;
	}

	return 0;
}