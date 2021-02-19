import java.io.*;
	
public class Q2Fibonacci{

	public static void main(String[] args) throws Exception
	{
		String inputString;
		int inputValue = 0;
		int[] outputArray;

		if(args.length == 0 || args.length > 1)
		{
			BufferedReader br = new BufferedReader(new InputStreamReader(System.in));
			System.out.println("Invalid input. Please enter a number: ");
			inputString = br.readLine(); //read keyboard input
			inputValue = Integer.parseInt(inputString);
		}

		else
		{
			inputString = args[0];
			inputValue = Integer.parseInt(inputString);
		}

		outputArray = new int[inputValue];
		FibonacciThread fibChildThread = new FibonacciThread(inputValue, outputArray);
		
		fibChildThread.start(); //start child thread

		fibChildThread.join(); //wait child thread to complete

		System.out.print("The output of " + inputString + " Fibonacci numbers are: ");
		for(int i=0; i<outputArray.length; i++)
		{
			outputArray[i] = FibonacciThread.fibArray[i];
			System.out.print(outputArray[i] + " ");
		}
	}	
}


/* Child thread for generating Fibonacci series */
class FibonacciThread extends Thread{
		
	int input;
	static int[] fibArray;

	public FibonacciThread(int input, int[] fibArray)
	{
		this.input = input;
		input = input+2;
		this.fibArray = new int[input];
	}
		
	/* Fibonacci implementation using Recursion-Memorization */
	public int fibonacci(int n)
	{
		if(n <= 1)
		{
			fibArray[n] = n;
			return n;
		}
		else
		{
			if(fibArray[n-2] == -1)
				fibArray[n-2] = fibonacci(n-2);
			
			else if(fibArray[n-1] == -1)
				fibArray[n-1] = fibonacci(n-1);

			else
				fibArray[n] = fibArray[n-2] + fibArray[n-1];
		}
		return fibonacci(n-1) + fibonacci(n-2);
	}

	public void run()
	{
		fibonacci(input+1);
	}
} 