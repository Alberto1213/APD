import java.io.*;
import java.util.*;
import java.io.RandomAccessFile;

public class Tema2 
{

    public static int minn(int a ,int b)
    	{
		if(a > b)
			return b;
		else return a;
	}
    public static void swapmatrix(double[][] matrix,int i, int j)
    	{
        	double[] aux2 = matrix[i];
		matrix[i] = matrix[j];
	        matrix[j] = aux2;										
        }
    public static void main(String[] args) 
	{
        	Scanner s;
                FileWriter fout;

        	double[][] res = new double[25][25];
        	
		
                int len;
       		int tasks;
        	int workers;
		Thread[] threads;
                
		if (args.length < 3) 
			{
            			System.err.println("Usage: Tema2 <workers> <in_file> <out_file>");
            			return;
        		}

        	File fin = new File("../" + args[1]);
        	

		try 
			{
            			s = new Scanner(fin);
                                char workerschar = (args[0]).charAt(0);

            			len = s.nextInt();
            			tasks = s.nextInt();
				
				ArrayList<String> strdata = new ArrayList<String> (tasks);
				byte[] info = new byte[len];

                    		int number = 0;
                    		int condition;

            			workers = (int) workerschar - 48;

				int taskreduce = minn(workers, tasks);

            			s.nextLine();
                                int[][] arr = new int[25][25];

            			for (int i = 0; i < tasks; i++) 
					{
                				String data = s.nextLine();
						strdata.add(i,data);
                
						try (RandomAccessFile raf = new RandomAccessFile("../" + data, "r")) 
							{	
								threads = new Thread[workers];

								do 
									{
                        							condition = raf.read(info, 0, len);
                        							number += condition;
                    							} while (condition >= len);

                    
								for (int k = 0; k < workers; k++) 
									{
                        							try 
											{
												threads[k] = new Maps(k, len, number, workers, "../" + data, i);
                        									threads[k].start();
                            									threads[k].join();

                            									for (int m = 0; m < tasks; m++)
													{
                                										for (int n = 1; n < 20; n++)
                                    											{
																arr[m][n] = arr[m][n] + ((Maps) threads[k]).getResult()[m][n];
															}
													}
                        								} catch (InterruptedException e) 
												{
                           										 e.printStackTrace();
                        									}
                    							}
                					} catch (IOException e) 
								{
                    							e.printStackTrace();
                						}
            				}

            					threads = new Thread[taskreduce];
            
						

            					for (int j = 0; j < taskreduce; j++) 
							{
								
                						try {
									threads[j] = new Reduce(j, tasks, taskreduce, arr);
                							threads[j].start();
                    							threads[j].join();

                    						for (int k = 0; k < tasks; k += taskreduce)
                        						
									res[j + k] = ((Reduce) threads[j]).getSolution()[j + k];

                					} catch (InterruptedException e) 
								{
                    							e.printStackTrace();
               	 						}	
            						}
				
			        
			 	for (int i = 0; i < tasks - 1; i++)
					{
                				for (int p = 0; p < tasks - 1 - i; p++) 
							{
                   						if (res[p][0] < res[p+1][0]) 	
									{
										Collections.swap(strdata,p,p+1);
										swapmatrix(res,p,p+1);
                    							}
                					}
            				}
                                
            
				try 
					{
                				fout = new FileWriter(args[2]);

               	 				for (int i = 0; i < tasks; i++) 
							{
                    						int poz;

                    						for (poz=0 ; (int) strdata.get(i).charAt(poz) != 47; poz++) 
									{
										if((int) strdata.get(i).charAt(poz) == 47) 
											break;
									}

                    						poz++;
								int poz_aux=poz;

								for(poz=poz_aux ; (int) strdata.get(i).charAt(poz) != 47; poz++)
									{
										if((int) strdata.get(i).charAt(poz) == 47) 
											break;
									}
								poz++;

                    						while (poz < strdata.get(i).length()) 										
									{
                        							fout.write(strdata.get(i).charAt(poz));
                        							poz++;
                    							}
								poz++;
                    						fout.write("," + Double.toString(res[i][0]));

                    						fout.write("," + Integer.toString((int) (res[i][1])));

                    						fout.write("," + Integer.toString((int) (res[i][2])));

                    						fout.write("\n");
                					}
                				fout.close();
            				} catch (IOException e) 
						{
                					e.printStackTrace();
            					}
			s.close();

        		} catch (IOException e) 
				{
            				e.printStackTrace();
        			}
			
    	}
}
