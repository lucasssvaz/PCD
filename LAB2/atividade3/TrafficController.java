//
//  Lucas Saavedra Vaz - 120503
//  Maíra Baptista - 111816
//

import java.util.concurrent.*;

public class TrafficController {

	static Semaphore sem = new Semaphore(1, false);

	public void enterLeft() 
	{
		try
		{
			sem.acquire();
		}
		catch(Exception e)
		{
			System.out.println("Error");
		}
	}

	public void enterRight() 
	{
		try
		{
			sem.acquire();
		}
		catch(Exception e)
		{
			System.out.println("Error");
		}
	}

	public void leaveLeft() 
	{
		sem.release();
	}

	public void leaveRight() 
	{
		sem.release();
	}

}