import java.lang.*;
import java.util.Random;

public class Lab_Jv implements Runnable 
{

	public static boolean isEven (long i)
	{
	    return (i % 2) == 0;
	}

	public void Grid_Init(boolean[][] Grid)
	{
		Random RandGen = new Random(1985);
		 
		for(int X = 0; X < Grid.length; X++)
			for(int Y = 0; Y < Grid.length; Y++)
		  		Grid[X][Y] = isEven(RandGen.nextInt(2147483647));

		return;
	}

	public void run()
	{
		
	}

	public static void main(String args[])
	{
		int MAX_THREADS = 8;
	}
}