import java.util.Arrays;
import java.util.Scanner;

public class FIFOandLRU {

	public static int fifo(int[] pageReference, int pageFramesSize)
	{
		int   pageFault  = 0;
		int   firstInPos = 0;
		int[] memory   	 = new int[pageFramesSize];

		Arrays.fill(memory, -1);

		for (int number: pageReference)
		{
			boolean checkResult = check(number, memory);

			if (!checkResult)
			{
				memory[firstInPos] = number;
				firstInPos = (firstInPos+1) % pageFramesSize;
				pageFault++;
			}
		}

		return pageFault;
	}
	
	
	public static int lru(int[] pageReference, int pageFramesSize) 
	{ 
		int   pageFault      = 0; 
		int[] memory         = new int[pageFramesSize]; 
		int[] countLeastUsed = new int[pageFramesSize];
	 
	 	Arrays.fill(memory, -1); 
	 	Arrays.fill(countLeastUsed, 0);
	 
	 	for (int number: pageReference) 
	 	{ 
	 		boolean checkResult = check(number, memory);
			
			for (int i=0; i<pageFramesSize; i++)
			{
				countLeastUsed[i] ++;
			}

	 		if (!checkResult) 
	 		{ 
	 			int leastUsedPos             = findMaxPos(countLeastUsed);
	 			memory[leastUsedPos]         = number;
	 			countLeastUsed[leastUsedPos] = 0;
	 			pageFault ++;
	 		}

	 		else
	 		{
	 			for (int i=0; i<pageFramesSize; i++)
	 			{
	 				if (memory[i] == number)
	 				{
	 					countLeastUsed[i] = 0;
	 				}
	 			}
	 		}
	 	}
	 
		return pageFault; 
	}
	 

	/* Check if the number in page reference is exist in memoryArray */
	public static boolean check(int num, int[] memoryArray)
	{
		for (int i: memoryArray)
		{
			if (num == i)
			{
				return true;
			}
		}

		return false;
	}
	

	/* Find the position of the maximum value in the array */
	public static int findMaxPos(int[] array)
	{
		int maxVal = array[0];
		int maxPos = 0;

		for (int i=0; i<array.length; i++)
		{
			if (array[i] > maxVal)
			{
				maxVal = array[i];
				maxPos = i;
			}
		}

		return maxPos;
	}

	
	public static void main(String[] args) 
	{
		Scanner scan   				= new Scanner(System.in);
		boolean pageFramesSizeCheck = false;
		
		int pageFramesSize;
		int pageReferenceSize;

		while (!pageFramesSizeCheck)
		{
			System.out.print("Please enter an integer for the number of page frames: ");
			
			String numPageFrames = scan.nextLine();
			pageFramesSize 		 = Integer.parseInt(numPageFrames);

			if(pageFramesSize > 7 || pageFramesSize < 1)
			{
				System.out.println("The integer is invalid.");
			}

			else
			{
				pageFramesSizeCheck = true;
			}
		}
		
		System.out.println();
		System.out.print("Please enter an Integer for the size of page-reference string: ");
		
		String numPageReference = scan.nextLine();
		pageReferenceSize 		= Integer.parseInt(numPageReference);
		int[]  pageReference 	= new int[pageReferenceSize];

		for (int i=0; i<pageReferenceSize; i++)
		{
			int randomPage 	 = (int)(Math.random()*10);
			pageReference[i] = randomPage;
		}

		int fifoFault = fifo(pageReference, pageReferenceSize);
		int lruFault  = lru(pageReference, pageReferenceSize);

		System.out.print("The page-reference string is: ");
		
		for (int i=0; i<pageReferenceSize; i++)
		{
			System.out.print(pageReference[i] + " ");
		}

		System.out.println();
		System.out.println("There is/are " + String.valueOf(fifoFault) + " page fault(s) in FIFO page-replacement algorithm.");
		System.out.println("There is/are " + String.valueOf(lruFault) + " page fault(s) in LRU page-replacement algorithm.");

	}

}
