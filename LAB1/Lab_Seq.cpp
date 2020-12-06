#include <cstdlib>
#include <utility>
#include <time.h>
#include <sys/time.h>
#include <iostream>
#include <vector>

#define SRAND_VALUE 1985
#define N_GENERATIONS 2000

//#define DEBUG

#ifdef DEBUG
	#define R_DEBUG(x) x
#else
	#define R_DEBUG(x)
#endif

using namespace std;

//==================================================================================================================================

void Grid_Init(vector<vector<bool>> &Grid)
{
	srand(SRAND_VALUE);

	for (unsigned int i = 0; i < Grid.size(); i++) 
		for (unsigned int j = 0; j < Grid[i].size(); j++) 
	    	Grid[i][j] = rand() % 2;
}

//--------------------------------------------------------------------------

int Neighbours_Count(vector<vector<bool>> &Grid, int X, int Y)
{
	int Nb_Total = 0;													//Qnt. of Neighbours
	vector <pair <int, int>> Nb_List;									//Vector of Neighbours coordinates for future validation

	for (int i = X-1; i <= X+1; i++) 									//Analyses all squares around current X,Y
		for (int j = Y-1; j <= Y+1; j++)
			if (!(i == X && j == Y))
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

	for (unsigned int i = 0; i < Grid.size(); i++) 
		for (unsigned int j = 0; j < Grid[i].size(); j++) 
	    	if (Grid[i][j] == 1)
	    		Cells_Sum++;

	return Cells_Sum;
}

//--------------------------------------------------------------------------

bool Cell_Update(vector<vector<bool>> &Grid, int X, int Y)
{
	int Nb_Count = Neighbours_Count(Grid, X, Y);

	if (Grid[X][Y] == 0)
	{
		if (Nb_Count == 3)
			return 1;
	}
	else
	{
		if (Nb_Count < 2 || Nb_Count >= 4)
			return 0;
	}

	return Grid[X][Y];
}

//--------------------------------------------------------------------------

void Grid_Update(vector<vector<bool>> &Grid, vector<vector<bool>> &New_Grid)
{
	int X, Y;

	for (X = 0; X < (int)Grid.size(); X++)
		for (Y = 0; Y < (int)Grid.size(); Y++)
			New_Grid[X][Y] = Cell_Update(Grid, X, Y);

}

//==================================================================================================================================

int main()
{

	int N;
    struct timeval Start_Time, End_Time;

    gettimeofday(&Start_Time, NULL);

	cout << "Enter the desired matrix order:" << endl;
	cin >> N;

	vector<vector<bool>> Grid (N, vector<bool> (N));
	vector<vector<bool>> New_Grid (N, vector<bool> (N));

	Grid_Init(Grid);

	cout << "Generation 0: " << Cells_Total(Grid) << endl; 

	for (int i = 1; i <= N_GENERATIONS; i++)
	{
		Grid_Update(Grid, New_Grid);
		Grid = New_Grid;
		cout << "Generation " << i << ": " << Cells_Total(Grid) << endl;
	}

    gettimeofday(&End_Time, NULL);
    cout << endl << "Time Elapsed: " << (int) (End_Time.tv_sec - Start_Time.tv_sec) << endl;

	return 0;
}
