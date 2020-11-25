#include <iostream>
#include <cstdlib>
#include <pthread.h>
#include <vector>
#include <utility>

#define SRAND_VALUE 1985
#define N_THREADS 4

//#define DEBUG

#ifdef DEBUG
	#define P_DEBUG(x) x
#else
	#define P_DEBUG(x)
#endif

using namespace std;

//==================================================================================================================================

void Grid_Init (vector< vector<bool> > &Grid)
{
	srand(SRAND_VALUE);

	for(unsigned int i = 0; i < Grid.size(); i++) 
		for(unsigned int j = 0; j < Grid[i].size(); j++) 
	    	Grid[i][j] = rand() % 2;
}

//--------------------------------------------------------------------------

int Neighbours_Count (vector< vector<bool> > &Grid, int X, int Y)
{
	int Nb_Total = 0;													//Qnt. of Neighbours
	vector < pair <int, int> > Nb_List;									//Vector of Neighbours coordinates for future validation

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

int Cells_Total (vector< vector<bool> > &Grid)
{
	int Cells_Sum = 0;

	for(unsigned int i = 0; i < Grid.size(); i++) 
		for(unsigned int j = 0; j < Grid[i].size(); j++) 
	    	if (Grid[i][j] == 1)
	    		Cells_Sum++;

	return Cells_Sum;
}

//==================================================================================================================================

int main()
{
	int N;
	pthread_t Threads[N_THREADS];

	cout << "Enter the desired matrix order:" << endl;
	cin >> N;

	vector< vector<bool> > Grid (N, vector<bool> (N));
	vector< vector<bool> > New_Grid (N, vector<bool> (N));

	Grid_Init(Grid);

	return 0;
}