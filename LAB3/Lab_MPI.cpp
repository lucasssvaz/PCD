#undef SEEK_SET
#undef SEEK_END
#undef SEEK_CUR

#include <cstdlib>
#include <utility>
#include <time.h>
#include <sys/time.h>
#include <iostream>
#include <vector>
#include <mpi.h>

#define SRAND_VALUE 1985
#define N_GENERATIONS 100

int Order;
int Total_Proc;
int Proc_Rank;
int Elem_Per_Proc;

//#define DEBUG

#ifdef DEBUG
	#define R_DEBUG(x) x
#else
	#define R_DEBUG(x)
#endif

using namespace std;

//==================================================================================================================================

void Grid_Init(bool *Grid)
{
	srand(SRAND_VALUE);

	for (int i = 0; i < Order; i++) 
		for (int j = 0; j < Order; j++) 
			Grid[i*Order+j] = rand() % 2;
}

//--------------------------------------------------------------------------

int Neighbours_Count(bool *Grid, int X, int Y)
{
	int Nb_Total = 0;                                                   //Qnt. of Neighbours
	vector <pair <int, int>> Nb_List;                                   //Vector of Neighbours coordinates for future validation

	for (int i = X-1; i <= X+1; i++)                                    //Analyses all squares around current X,Y
		for (int j = Y-1; j <= Y+1; j++)
			if (!(i == X && j == Y))
				Nb_List.push_back(make_pair(i+Proc_Rank*Elem_Per_Proc, j));         //Adds the neighbours coordinates to the vector


	for (int i = 0; i < 8; i++)                                         //Analyses each neighbour to check if the coordinates are valid and, if not, fixes them
	{
		if (Nb_List[i].first < 0)                                       //Validate X coordinate
			Nb_List[i].first = Order-1;
		else if (Nb_List[i].first > (signed int) Order-1)
			Nb_List[i].first = 0;

		if (Nb_List[i].second < 0)                                      //Validade Y coordinate
			Nb_List[i].second = Order-1;
		else if (Nb_List[i].second > (signed int) Order-1)
			Nb_List[i].second = 0;
	}

	for (int i = 0; i < 8; i++)                                         //Verify how many neighbours are alive
		if (Grid[Nb_List[i].first * Order + Nb_List[i].second] == 1)             //Access Cell Status for each Neighbour 
			Nb_Total++;

	return Nb_Total;
}

//--------------------------------------------------------------------------

int Cells_Total(bool *Grid)
{
	int Cells_Sum = 0;

	for (int i = 0; i < Order; i++) 
		for (int j = 0; j < Order; j++) 
			if (Grid[i*Order + j] == 1)
				Cells_Sum++;

	return Cells_Sum;
}

//--------------------------------------------------------------------------

bool Cell_Update(bool *Subset, bool* Grid, int X, int Y)
{
	int Nb_Count = Neighbours_Count(Grid, X, Y);

	if (Subset[X*Order+Y] == 0)
	{
		if (Nb_Count == 3)
			return 1;
	}
	else
	{
		if (Nb_Count < 2 || Nb_Count >= 4)
			return 0;
	}

	return Subset[X * Order + Y];
}

//--------------------------------------------------------------------------

void Grid_Update(bool *Grid, bool *New_Grid)
{
	int X, Y;

	bool *Subset = new bool[Elem_Per_Proc];
	bool *New_Subset = new bool[Elem_Per_Proc];

	MPI_Scatter(Grid, Elem_Per_Proc, MPI_CXX_BOOL, Subset, Elem_Per_Proc, MPI_CXX_BOOL, 0, MPI_COMM_WORLD);

	for (X = 0; X < Elem_Per_Proc/Order; X++)
		for (Y = 0; Y < Order; Y++)
			New_Subset[X * Order + Y] = Cell_Update(Subset, Grid, X, Y);

	MPI_Barrier(MPI_COMM_WORLD);

	MPI_Allgather(New_Subset, Elem_Per_Proc, MPI_CXX_BOOL, New_Grid, Order*Order, MPI_CXX_BOOL, MPI_COMM_WORLD);

}

//==================================================================================================================================

int main()
{

	MPI_Init(NULL, NULL);

	MPI_Comm_size(MPI_COMM_WORLD, &Total_Proc);
	
  	MPI_Comm_rank(MPI_COMM_WORLD, &Proc_Rank);

  	Order = 4;

  	Elem_Per_Proc = Order * Order / Total_Proc;

	struct timeval Start_Time, End_Time;

	gettimeofday(&Start_Time, NULL);

	bool *Grid = new bool[Order * Order];
	bool *New_Grid = new bool[Order * Order];

	Grid_Init(Grid);

	if (Proc_Rank == 0)
		cout << "Generation 0: " << Cells_Total(Grid) << endl; 

	for (int i = 1; i <= N_GENERATIONS; i++)
	{
		Grid_Update(Grid, New_Grid);
		*Grid = *New_Grid;
		if (Proc_Rank == 0)
			cout << "Generation " << i << ": " << Cells_Total(Grid) << endl;
	}

	gettimeofday(&End_Time, NULL);
	if (Proc_Rank == 0)
		cout << endl << "Time Elapsed: " << (int) (End_Time.tv_sec - Start_Time.tv_sec) << endl;

	MPI_Finalize();

	return 0;
}
