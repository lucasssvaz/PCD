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
#include <math.h>

#define SRAND_VALUE 1985
#define N_GENERATIONS 100
#define N 64

int Order;
int Total_Proc;
int Proc_Rank;
int Elem_Per_Proc;
int send_one[N], send_two[N];
int receive_one[N], receive_two[N];



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
    int Nb_Total = 0;                                                   //Qnt. of Neighbours
    vector <pair <int, int>> Nb_List;                                   //Vector of Neighbours coordinates for future validation

    for (int i = X-1; i <= X+1; i++)                                    //Analyses all squares around current X,Y
        for (int j = Y-1; j <= Y+1; j++)
            if (!(i == X && j == Y))
                Nb_List.push_back(make_pair(i, j));                     //Adds the neighbours coordinates to the vector


    for (int i = 0; i < 8; i++)                                         //Analyses each neighbour to check if the coordinates are valid and, if not, fixes them
    {
        if (Nb_List[i].first < 0)                                       //Validate X coordinate
            Nb_List[i].first = Grid.size()-1;
        else if (Nb_List[i].first > (signed int) Grid.size()-1)
            Nb_List[i].first = 0;

        if (Nb_List[i].second < 0)                                      //Validade Y coordinate
            Nb_List[i].second = Grid.size()-1;
        else if (Nb_List[i].second > (signed int) Grid.size()-1)
            Nb_List[i].second = 0;
    }

    for (int i = 0; i < 8; i++)                                         //Verify how many neighbours are alive
        if (Grid[Nb_List[i].first][Nb_List[i].second] == 1)             //Access Cell Status for each Neighbour 
            Nb_Total++;

    return Nb_Total;
}

//--------------------------------------------------------------------------

int Cells_Total(vector<vector<bool>> &Grid)
{
    int Cells_Sum = 0;

    for (unsigned int i = 1; i < Elem_Per_Proc+1; i++) 
        for (unsigned int j = 1; j < N+1; j++) 
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

void Grid_Update(vector<vector<bool>> &Grid, vector<vector<bool>> &New_Grid, MPI_Status status, int anterior, int posterior)
{
    int X, Y;

    for (int l = 0; l < N; l++){
        send_one[l] = Grid[1][l+1];
        send_two[l] = Grid[Elem_Per_Proc][l+1];
    }

    MPI_Sendrecv (send_one, N, MPI_INTEGER, anterior, 1, receive_two,  N, MPI_INTEGER, posterior, 1, MPI_COMM_WORLD, &status);

    MPI_Sendrecv (send_two, N, MPI_INTEGER, posterior, 1, receive_one, N, MPI_INTEGER, anterior, 1, MPI_COMM_WORLD, &status);   

    for(int j = 0; j < N;j++){
        Grid[0][j+1] = receive_one[j];
        Grid[Elem_Per_Proc+1][j+1] = receive_two[j];
    }

    for(int i = 0; i < Elem_Per_Proc+2; i++){
        Grid[i][N+1] = Grid[i][1];
        Grid[i][0] = Grid[i][N];
    }
    MPI_Barrier(MPI_COMM_WORLD);
    for (X = 0; X < (int)Elem_Per_Proc+1; X++)
        for (Y = 0; Y < (int)Grid.size()+1; Y++)
            New_Grid[X][Y] = Cell_Update(Grid, X, Y);
    MPI_Barrier(MPI_COMM_WORLD);
}

//==================================================================================================================================

int main()
{
    MPI_Init(NULL, NULL);

	int Total_Proc;
	MPI_Comm_size(MPI_COMM_WORLD, &Total_Proc);

	int Proc_Rank;
  	MPI_Comm_rank(MPI_COMM_WORLD, &Proc_Rank);

    MPI_Status status;

    //int N;
    int anterior, posterior;
    int start = 0, end = 0, total_alive = 0;
    struct timeval Start_Time, End_Time;

	gettimeofday(&Start_Time, NULL);
    Order = 64;
    //Elem_Per_Proc = Order * Order / Total_Proc;
    
    Elem_Per_Proc = N / Total_Proc;

    posterior = ((Proc_Rank + Total_Proc - 1) % Total_Proc);
    anterior = ((Proc_Rank + 1) % Total_Proc);

    vector<vector<bool>> Grid (Elem_Per_Proc+2, vector<bool> (N+2));
    vector<vector<bool>> New_Grid (Elem_Per_Proc+2, vector<bool> (N+2));

    MPI_Barrier(MPI_COMM_WORLD);

    Grid_Init(Grid);

    MPI_Barrier(MPI_COMM_WORLD);

    if (Proc_Rank == 0)
	    cout << "Generation 0: " << Cells_Total(Grid) << endl; 

    for (int i = 1; i <= N_GENERATIONS; i++)
    {
        total_alive = 0;
        Grid_Update(Grid, New_Grid, status, anterior, posterior);
        Grid = New_Grid;
        total_alive = Cells_Total(Grid);
        if (Proc_Rank == 0)
		    cout << "Generation " << i << ": " << Cells_Total(Grid) << endl;
    }

    MPI_Barrier(MPI_COMM_WORLD);

    int *quantity_final = new int [Total_Proc];

    MPI_Allgather(&total_alive, 1, MPI_INTEGER, quantity_final, 1, MPI_INTEGER, MPI_COMM_WORLD);

    int sum = 0;

    for(int i = 0; i < Total_Proc; i++){
        sum += quantity_final[i];
    }
    if (Proc_Rank == 0){
        cout << "Final count: " << sum << endl;
    }
    
    gettimeofday(&End_Time, NULL);
    if (Proc_Rank == 0)
	    cout << endl << "Time Elapsed: " << (int) (End_Time.tv_sec - Start_Time.tv_sec) << endl;

	MPI_Finalize();

    return 0;
}
