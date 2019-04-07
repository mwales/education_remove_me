
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.math.BigInteger;
import java.util.Vector;
import java.util.TreeSet;
import java.util.Iterator;
import java.util.TreeMap;

/*
 * My code successfully would solve the sample problem, but Google said it
 * failed to find correct solution when I tried to submit it during the
 * Code Jam.
 *
 * Fun little problem.  The encryptor would have been a fun excercise to
 * create as well.
 *
 * javac Solution.java
 * cat sample_input.java | java Solution
 */

/*
Problem
On the Code Jam team, we enjoy sending each other pangrams, which are phrases that use each letter of the English alphabet at least once. One common example of a pangram is "the quick brown fox jumps over the lazy dog". Sometimes our pangrams contain confidential information — for example, CJ QUIZ: KNOW BEVY OF DP FLUX ALGORITHMS — so we need to keep them secure.

We looked through a cryptography textbook for a few minutes, and we learned that it is very hard to factor products of two large prime numbers, so we devised an encryption scheme based on that fact. First, we made some preparations:

We chose 26 different prime numbers, none of which is larger than some integer N.
We sorted those primes in increasing order. Then, we assigned the smallest prime to the letter A, the second smallest prime to the letter B, and so on.
Everyone on the team memorized this list.
Now, whenever we want to send a pangram as a message, we first remove all spacing to form a plaintext message. Then we write down the product of the prime for the first letter of the plaintext and the prime for the second letter of the plaintext. Then we write down the product of the primes for the second and third plaintext letters, and so on, ending with the product of the primes for the next-to-last and last plaintext letters. This new list of values is our ciphertext. The number of values is one smaller than the number of characters in the plaintext message.

For example, suppose that N = 103 and we chose to use the first 26 odd prime numbers, because we worry that it is too easy to factor even numbers. Then A = 3, B = 5, C = 7, D = 11, and so on, up to Z = 103. Also suppose that we want to encrypt the CJ QUIZ... pangram above, so our plaintext is CJQUIZKNOWBEVYOFDPFLUXALGORITHMS. Then the first value in our ciphertext is 7 (the prime for C) times 31 (the prime for J) = 217; the next value is 1891, and so on, ending with 3053.

We will give you a ciphertext message and the value of N that we used. We will not tell you which primes we used, or how to decrypt the ciphertext. Do you think you can recover the plaintext anyway?

Input
The first line of the input gives the number of test cases, T. T test cases follow; each test case consists of two lines. The first line contains two integers: N, as described above, and L, the length of the list of values in the ciphertext. The second line contains L integers: the list of values in the ciphertext.

Output
For each test case, output one line containing Case #x: y, where x is the test case number (starting from 1) and y is a string of L + 1 uppercase English alphabet letters: the plaintext.

Limits
1 ≤ T ≤ 100.
Time limit: 20 seconds per test set.
Memory limit: 1 GB.
25 ≤ L ≤ 100.
The plaintext contains each English alphabet letter at least once.

Test set 1 (Visible)
101 ≤ N ≤ 10000.

Test set 2 (Hidden)
101 ≤ N ≤ 10100.

Sample

Input 
 	
2
103 31
217 1891 4819 2291 2987 3811 1739 2491 4717 445 65 1079 8383 5353 901 187 649 1003 697 3239 7663 291 123 779 1007 3551 1943 2117 1679 989 3053
10000 25
3292937 175597 18779 50429 375469 1651121 2102 3722 2376497 611683 489059 2328901 3150061 829981 421301 76409 38477 291931 730241 959821 1664197 3057407 4267589 4729181 5335543

  


Output 
 
Case #1: CJQUIZKNOWBEVYOFDPFLUXALGORITHMS
Case #2: SUBDERMATOGLYPHICFJKNQVWXZ
*/



class Solution
{
	private static BufferedReader r;

	private static TreeMap<BigInteger,String> decryptLookup;

	public static Vector<BigInteger> readLineOfCrypto() throws IOException
	{
		Vector<BigInteger> retVal = new Vector<BigInteger>();

		String thisLine = r.readLine();
		String individualNumStrings[];
		individualNumStrings = thisLine.split(" ");

		for(int i = 0; i < individualNumStrings.length; i++)
		{
			retVal.add(new BigInteger(individualNumStrings[i]));
		}

		return retVal;
	}

	public static String bigNumVecToString(Vector<BigInteger> v)
	{
		String retVal = "";

		for(int i = 0; i < v.size(); i++)
		{
			retVal += v.elementAt(i).toString();
			retVal += " ";
		}

		return retVal;
	}

	// Shamelss code theft from StackOverflow
	// https://stackoverflow.com/questions/16802233/faster-prime-factorization-for-huge-bigintegers-in-java
	public static Vector<BigInteger> tdFactors(BigInteger n)
	{
		BigInteger two = BigInteger.valueOf(2);
		Vector<BigInteger> fs = new Vector<BigInteger>();

		if (n.compareTo(two) < 0)
		{
		    throw new IllegalArgumentException("must be greater than one");
		}

		while (n.mod(two).equals(BigInteger.ZERO))
		{
		    fs.add(two);
		    n = n.divide(two);
		}

		if (n.compareTo(BigInteger.ONE) > 0)
		{
			BigInteger f = BigInteger.valueOf(3);
			while (f.multiply(f).compareTo(n) <= 0)
			{
				if (n.mod(f).equals(BigInteger.ZERO))
				{
					fs.add(f);
					n = n.divide(f);
				}
				else
				{
					f = f.add(two);
				}
			}
			fs.add(n);
		}

		return fs;
	}

	public static TreeMap<BigInteger, String> createLookupMap(TreeSet<BigInteger> keySet)
	{
		TreeMap<BigInteger, String> retVal = new TreeMap<BigInteger, String>();
		
		char curChar = 'A';
		Iterator<BigInteger> it = keySet.iterator();
		while(it.hasNext())
		{
			BigInteger curVal = it.next();
			String curStr = Character.toString(curChar);
			retVal.put(curVal, curStr);

			// System.out.println("      " + curStr + " = " + curVal.toString());

			curChar++;
		}

		return retVal;
	}

	public static String lookupCT(BigInteger x)
	{
		return decryptLookup.get(x).toString();
	}

	public static String printCTD(BigInteger x)
	{
		String retVal = "(" + x.toString() + ":" + lookupCT(x) + ")";
		return retVal;
	}

	public static String solveProblem() throws IOException
	{
		String problemSetup = r.readLine();
		String problemSetupParts[];
		problemSetupParts = problemSetup.split(" ");
		int N = Integer.parseInt(problemSetupParts[0]);
		int L = Integer.parseInt(problemSetupParts[1]);

		//System.out.println("N = " + N + ", and L = " + L);

		TreeSet<BigInteger> keySet = new TreeSet<BigInteger>();
		Vector<BigInteger> cipherText = readLineOfCrypto();
		Vector<Vector<BigInteger>> cipherFactors = new Vector<Vector<BigInteger>>();

		for(int i = 0; i < cipherText.size(); i++)
		{
			//System.out.println("i = " + i + " = " + cipherText.elementAt(i).toString());
			//System.out.println("  Factors: " + bigNumVecToString(tdFactors(cipherText.elementAt(i))));
			Vector<BigInteger> factors = tdFactors(cipherText.elementAt(i));
			cipherFactors.add(factors);

			keySet.addAll(factors);
		}

		if (keySet.size() != 26)
		{
			System.out.println("WTF, not the right number of factors!");
			return "Fubar";
		}

		decryptLookup = createLookupMap(keySet);

		String retVal = "";
		BigInteger prevCtFactor;

		// Starting the decryption is a special case, you have to peek at 2nd CT factors
		Vector<BigInteger> firstCT = cipherFactors.elementAt(0);
		Vector<BigInteger> secondCT = cipherFactors.elementAt(1);
		BigInteger firstLeft = firstCT.elementAt(0);
		BigInteger firstRight = firstCT.elementAt(1);
		BigInteger secondLeft = secondCT.elementAt(0);
		BigInteger secondRight = secondCT.elementAt(1);

		if ( (firstLeft.compareTo(secondLeft) == 0) || (firstLeft.compareTo(secondRight) == 0) )
		{
			// If a is in 2nd, that means a is the 2nd, b is the first
			//retVal += lookupValue(keySet, firstCT.elementAt(1));
			retVal += lookupCT(firstRight);
			prevCtFactor = firstRight;
		}
		else
		{
			//retVal += lookupValue(keySet, a);
			retVal += lookupCT(firstLeft);
			prevCtFactor = firstLeft;
		}

		//System.out.println("FirstCT = " + printCTD(prevCtFactor));

		for(int i = 0; i < cipherFactors.size(); i++)
		{
			Vector<BigInteger> curPair = cipherFactors.elementAt(i);
			BigInteger left = curPair.elementAt(0);
			BigInteger right = curPair.elementAt(1);

			if (left.compareTo(prevCtFactor) == 0)
			{
				retVal += decryptLookup.get(right);
				//System.out.println("PrevCT = " + printCTD(prevCtFactor));
				//System.out.println("Added to string " + printCTD(right));

				//System.out.println("  i = " + i + "    " + printCTD(left) + " [" + printCTD(right) + "]");
				prevCtFactor = right;
			}
			else
			{
				retVal += decryptLookup.get(left);
				//System.out.println("PrevCT = " + printCTD(prevCtFactor));
				//System.out.println("Added to string " + printCTD(left));

				//System.out.println("  i = " + i + "    [" + printCTD(left) + "] " + printCTD(right) + "");
				prevCtFactor = left;
			}
		}

		return retVal;


	}

	public static void main(String args[]) throws IOException
	{
		r = new BufferedReader(new InputStreamReader(System.in));

	
		String numProbsStr = r.readLine();
		int numProbs = Integer.parseInt(numProbsStr);

		for(int i = 0; i < numProbs; i++)
		{
			String soln = solveProblem();
			System.out.println("Case #" + (i+1) + ": " + soln);
		}
		
	}






}
