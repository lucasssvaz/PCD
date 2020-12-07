package sequential;

import java.lang.*;
import java.util.Random;
import java.util.Scanner;
import java.util.*; 

import jdk.jfr.Unsigned;

/*
Compile: javac sequential/Lab_Jv.java
Run: java sequential/Lab_Jv
*/

public class Lab_Jv 
{
	public static boolean isEven (long i)
	{
	    return (i % 2) == 0;
	}

	public static void GridInit(boolean[][] Grid)
	{
		Random RandGen = new Random(1985);
		 
		for(int X = 0; X < Grid.length; X++)
			for(int Y = 0; Y < Grid.length; Y++)
		  		Grid[X][Y] = isEven(RandGen.nextInt(2147483647));

		return;
	}

	public static int CellsTotal(boolean[][] Grid)
	{
		int cellsSum = 0;
		for (int i = 0; i < Grid.length; i++)
		{
			for (int j = 0; j < Grid[i].length; j++)
			{
				if(Grid[i][j] == true)
				{
					cellsSum++;
				}
			}
		}
		return cellsSum;
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

	public static void GridUpdate(boolean[][] Grid, boolean[][] newGrid)
	{
		int i;
		int j;
		for(i= 0; i < (int)Grid.length; i++)
		{
			for(j = 0; j < (int)Grid.length; j++)
			{
				newGrid[i][j] = CellUpdate(Grid, i, j);
			}	
		}
	}

	public static void main(String args[]) throws InterruptedException
	{
		//System.out.println("Enter the desired matrix order: ");
		//Scanner input = new Scanner(System.in);
		//int N = input.nextInt();

		
		int N_GENERATIONS = 2000;
		int N = 2048;
		boolean[][] Grid = new boolean[N][N];
		boolean[][] newGrid = new boolean[N][N];
		int i;

		GridInit(Grid);

		System.out.println("Generation 0: " + CellsTotal(Grid));

		long start_time = System.currentTimeMillis();

		for (i = 1; i <= N_GENERATIONS; i++){ //iterate trhough all generations
			GridUpdate(Grid, newGrid);

			Grid = newGrid;
			newGrid = null;
			newGrid = new boolean[N][N];

			System.out.println("Generation " + (i) + ": " + CellsTotal(Grid));
			System.out.println("End of a generation");

		}

		long final_time = System.currentTimeMillis() - start_time;
		System.out.println("Processing time ->" + final_time + "ms");
	}
}