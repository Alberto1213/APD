import java.io.FileNotFoundException;

public class Reduce extends Thread 
{
    private final int[][] task;
    private final double[][] sol=new double[25][25];

    private final int cont;
    private final int worker;
    private final int doc;
    
    public int F1 = 1;
    public int F2 = 2;
    public int swap;
    public int rank=0;
    public int word=0;
    
    double[][] wordmax;
    
    public static int fib(int n)
    	{
		if (n <= 2)
        		return n;
    		return fib(n-1) + fib(n-2);
	}
    public static double calculate(int par1 , int par2)
    	{
		if(( 1000 * par1 / par2 )-
		10 * (Math.round( 100 * par1 / par2 )) >= 5)
                                    
                	return ( 100 * par1 / par2 + 1 ) / 100.0;
            
		else  
			return ( 100 * par1 / par2) / 100.0;

	}
    public Reduce(int cont, int doc, int worker, int[][] task) throws FileNotFoundException 
	{
		this.worker=worker;
        	this.doc=doc;
        	this.cont = cont;
        	this.task=task; 	
    	}

    public double[][] getSolution() 
	{
        	return sol;
    	}

    public void run() 
	{
	        
	        wordmax = new double[25][3];

	        for (int i = cont; i < doc; i += worker) 
			{
	            	for (int j = 1; j < 20; j++)
                		if (task[i][j] != 0) 
					{
						wordmax[i][1]=j;
                    				wordmax[i][2]=task[i][j];
                    				word+=task[i][j];
						rank+=fib(j)*task[i][j];
                			}

				wordmax[i][0]=calculate(rank , word);
		                sol[i]=wordmax[i];

            			word=0;
				rank=0;
        		}
    	}

    
}
