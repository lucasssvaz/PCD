package threads;

import java.lang.*;
import java.util.*; 

class GridUpdate implements Runnable {
	private boolean[][] Grid;
	private boolean[][] newGrid;

	private int start_idx;
    private int end_idx;
	private int num_lines;
	private int grid_dimension;
	private int thread_id;

	public GridUpdate(boolean[][] _old, boolean[][] _new, int num_threads, int _grid_dimension, int _thread_id) {
		Grid = _old;
		newGrid = _new;
		grid_dimension = _grid_dimension;
		thread_id = _thread_id;

        num_lines = grid_dimension/num_threads;
        if(thread_id == 0) {
            start_idx = 0;
            end_idx = num_lines - 1;
        } else if(thread_id == num_threads - 1) {
            start_idx = grid_dimension - num_lines;
            end_idx = grid_dimension - 1;
        } else {
            start_idx = num_lines * thread_id;
            end_idx = start_idx + num_lines - 1;
        }
    }
    
    public static int NeighboursCount(boolean[][] Grid, int X, int Y)
    {
        int totalNeighbours = 0;													
        List<Pair<Integer, Integer>> neighboursList = new ArrayList<Pair<Integer, Integer>>(); 

        for (int i = X-1; i <= X+1; i++) 									
            for (int j = Y-1; j <= Y+1; j++)
                if (!(i == X && j == Y)){ 
                    Pair<Integer, Integer> newPair = new Pair<Integer, Integer>(i, j);
                    neighboursList.add(newPair);	
                }

        for (int i = 0; i < 8; i++)											
        {
            if (neighboursList.get(i).getLeft() < 0)										
                neighboursList.get(i).setLeft(Grid.length-1);
            else if (neighboursList.get(i).getLeft() > (int) Grid.length-1)
                neighboursList.get(i).setLeft(0); 

            if (neighboursList.get(i).getRight() < 0)										
                neighboursList.get(i).setRight(Grid.length-1);
            else if (neighboursList.get(i).getRight() > (int) Grid.length-1)
                neighboursList.get(i).setRight(0);
        }

        for (int i = 0; i < 8; i++)											
            if (Grid[neighboursList.get(i).getLeft()][neighboursList.get(i).getRight()] == true)				
                totalNeighbours++;

        return totalNeighbours;
    }
    
    
    public static boolean CellUpdate(boolean[][] Grid, int X, int Y)
    {
        int Nb_Count = NeighboursCount(Grid, X, Y);

        if (Grid[X][Y] == false)
        {
            if (Nb_Count == 3)
                return true;
        }
        else
        {
            if (Nb_Count < 2 || Nb_Count >= 4)
                return false;
        }

        return Grid[X][Y];
    }

	public void run() {
        int idx;
        for(idx = start_idx; idx <= end_idx; idx++) {         
            for(int inner_idx = 0; inner_idx < grid_dimension; inner_idx++) {
                newGrid[idx][inner_idx] =  CellUpdate(Grid, idx, inner_idx);
            }
        }
	}
}
