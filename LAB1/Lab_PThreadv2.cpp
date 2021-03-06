#include <iostream>
#include <cstdlib>
#include <pthread.h>
#include <vector>
#include <utility>
#include <stack>
#include <time.h>
#include <sys/time.h>

#define SRAND_VALUE 1985
#define N_THREADS 2
#define N_GENERATIONS 2000

//#define DEBUG

#ifdef DEBUG
    #define R_DEBUG(x) x;
#else
    #define R_DEBUG(x)
#endif

using namespace std;

struct PT_Args
{
    vector <vector<bool>>* Grid;
    vector <vector<bool>>* New_Grid;
    stack <pair<int, int>>* Coord_Stack;
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

void Vector_Print(vector <pair <int, int>> Nb_List)
{
    for (auto i = Nb_List.begin(); i != Nb_List.end(); i++)
        cout << (*i).first << ' ' << (*i).second << endl;
    return;
}

//--------------------------------------------------------------------------

int Neighbours_Count(vector<vector<bool>> &Grid, int X, int Y)
{
    int Nb_Total = 0;                                                   //Qnt. of Neighbours
    vector <pair <int, int>> Nb_List;                                   //Vector of Neighbours coordinates for future validation

    for(int i = X-1; i <= X+1; i++)                                     //Analyses all squares around current X,Y
        for(int j = Y-1; j <= Y+1; j++)
            if  (!(i == X && j == Y))
                Nb_List.push_back(make_pair(i, j));                     //Adds the neighbours coordinates to the vector

    R_DEBUG(Vector_Print(Nb_List); cout << endl);

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

    R_DEBUG(Vector_Print(Nb_List); cout << endl);

    for (int i = 0; i < 8; i++)                                         //Verify how many neighbours are alive
        if (Grid[Nb_List[i].first][Nb_List[i].second] == 1)             //Access Cell Status for each Neighbour 
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
    vector <vector<bool>> *Grid, *New_Grid;
    stack <pair<int, int>> *Coord_Stack;
    int X, Y;

    Grid = ((struct PT_Args*) Args)->Grid;
    New_Grid = ((struct PT_Args*) Args)->New_Grid;                      //Extracts data from Args Struct
    Coord_Stack = ((struct PT_Args*) Args)->Coord_Stack;

    while(!(*Coord_Stack).empty())
    {
        X = (*Coord_Stack).top().first;
        Y = (*Coord_Stack).top().second;
        (*Coord_Stack).pop();

        int Nb_Count = Neighbours_Count(*Grid, X, Y);
    
            if ((*Grid)[X][Y] == 0)
            {
                if (Nb_Count == 3)
                {
                    (*New_Grid)[X][Y] = 1;
                    continue;
                }
            }
            else
            {
                if (Nb_Count < 2 || Nb_Count >= 4)
                {
                    (*New_Grid)[X][Y] = 0;
                    continue;
                }       
            }
    
        (*New_Grid)[X][Y] = (*Grid)[X][Y];
    }

    pthread_exit(NULL);
}

//--------------------------------------------------------------------------

void Grid_Update(vector<vector<bool>> &Grid, vector<vector<bool>> &New_Grid)
{
    pthread_t Threads[N_THREADS];
    int X, Y;
    int PT_Count = 0;
    struct PT_Args Args[N_THREADS];
    stack <pair <int, int>> Coord_Stack[N_THREADS];

    for (X = 0; X < (int)Grid.size(); X++)
    {
        for (Y = 0; Y < (int)Grid[0].size(); Y++)
        {
            R_DEBUG(cout << "Coord: " << X << " " << Y << endl);
            Coord_Stack[PT_Count % N_THREADS].push(make_pair(X, Y));
            PT_Count++;
        }
    }

    for (int i = 0; i < N_THREADS; i++)
    {
        Args[i].Grid = &Grid;
        Args[i].New_Grid = &New_Grid;
        Args[i].Coord_Stack = &(Coord_Stack[i]);
        pthread_create(&Threads[i], NULL, Cell_Update, (void*) &(Args[i]));
    }

    for (int i = 0; i < N_THREADS; i++)
        pthread_join(Threads[i], NULL);
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