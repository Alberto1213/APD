import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.RandomAccessFile;
import java.util.*;

public class Maps extends Thread 
{
    private final int[][] res =new int[25][25];

    private final int cont;
    private final int len;
    private final int num;
    private final int work;
    private final String s;
    
    private final int doc;
    
    public int m;
    public int wlen=0;
    public byte[] arr;
     
    public static boolean condition(byte array[] , int i)
	{
		boolean ok1=(array[i] > 64 && array[i] < 91);
		boolean ok2=(array[i] > 96 && array[i] < 123);
		boolean ok3=(array[i] > 47 && array[i] < 58);

		return (ok1 || ok2 || ok3);
	}
    public static int minn(int a , int b)
	{
		if(a > b)
			return b;
		return a;
	}

    public static int lenmodif(int len , int coef , int cst)
	{
		return coef*len+cst;
	}
    public Maps(int cont,int len,int num,int work, String s,int doc) throws FileNotFoundException 
	{
	        this.cont = cont;
	        this.len=len;
                this.s=s;
	        this.doc=doc;
	        this.num=num;
	        this.work=work;
	        
    	}
    public int[][] getResult() 
	{
        	return res;
    	}

    public void run() 
	{
 	       try(RandomAccessFile str = new RandomAccessFile(s, "r")) 
		{

	            arr = new byte[len + 20];
		    
	            for (int i = cont; i < Math.ceil((float) num / len); i += work) 
			{
		                int j = 1;
		                for(int p = 0 ; p< lenmodif(len,1,20) ; p++)
		                    arr[p]=0;

		                try 
					{
                    				if (i != 0)
                        				str.seek((long) lenmodif(len,i,-1));
                			} catch (IOException e) 
						{
				                    e.printStackTrace();
				                }
                
				try 
					{
                    				if (num - len <= 20) 
							{
								m = minn((num-lenmodif(len,i,1)),len);
                        					str.read(arr, 0, (num-lenmodif(len,i,1)));                        					
                    					} 
						if (num - lenmodif(len,i,0) >= 20) 
							{
								m = len;
                        					str.read(arr, 0, lenmodif(len,1,20));
                    					} 
							else 
								{
									m = minn((num-lenmodif(len,i,1)),len);
                        						str.read(arr, 0, (num - (lenmodif(len,i,-1))));	
                    						}
                    				if(i==0)
							{
								j=0;
								m--;
							}
                    				else if (condition(arr,0)) 
							{
    					                    while (condition(arr,j)) 
									j++;

                    					}
                    				if(j > m)
							continue;

                    				if (condition(arr,m)) 
							{
                        					while (condition(arr,m+1))
                            						m++;
                    					}

                    				for(int p = j ; p <= m ; p++)
							{
								while(condition(arr,p)) 
									{
										p+=1;
                            							wlen+=1;	
                        						}
                        					if(wlen>0) 
									{
                            							res[doc][wlen]++;
										wlen=0;
                        						}
                    					}
                			} catch (IOException e) 
						{
                    					e.printStackTrace();
                				}
            		}
        	} catch (IOException e) 
			{
            			e.printStackTrace();
        		}
    }
}
