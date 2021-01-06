//
//	Lucas Saavedra Vaz - 120503
//	Maíra Baptista - 111816
//

package threads;

import java.lang.*;
import java.util.Random;
import java.util.Scanner;

import jdk.jfr.Unsigned;

public class Lab_Jv 
{
	private static int N = 2048;
	public static boolean[][] Grid = new boolean[N][N];
	//public static boolean[][] newGrid = new boolean[N][N];

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

	public static void main(String args[]) throws InterruptedException
	{
		//System.out.println("Enter the desired matrix order: ");
		//Scanner input = new Scanner(System.in);
		//int N = input.nextInt();

		int MAX_THREADS = 2;
		int N_GENERATIONS = 2000;
		boolean[][] newGrid = new boolean[N][N];
		int i, aux_i;

		Thread[] thread_array = new Thread[MAX_THREADS];
		GridUpdate[] support_array = new GridUpdate[MAX_THREADS];

		GridInit(Grid);

		System.out.println("Generation 0: " + CellsTotal(Grid));

		long start_time = System.currentTimeMillis();

		for (i = 1; i <= N_GENERATIONS; i++){ //iterate trhough all generations
			
			for(int idx = 0; idx < MAX_THREADS; idx++)
			{				
				support_array[idx] = new GridUpdate(Grid, newGrid, MAX_THREADS, N, idx);
                thread_array[idx] = new Thread(support_array[idx]);
				thread_array[idx].start();				
			}
			
			try {
                for(aux_i = 0; aux_i < MAX_THREADS; aux_i++ ) {
                    thread_array[aux_i].join();
                }
            } catch (InterruptedException e) {
                System.out.println("Error ocurred");
			}
			
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
