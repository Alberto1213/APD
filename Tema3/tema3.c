#include "mpi.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
 
int main (int argc, char **argv) 
{
    
    int numtasks;
    int RANK; 
    int len;
    int ok;

    char hostname[MPI_MAX_PROCESSOR_NAME];

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
    MPI_Comm_rank(MPI_COMM_WORLD,&RANK);
    MPI_Get_processor_name(hostname, &len); 
    
    
    MPI_Status status;
    MPI_Request request;

    FILE *file;
   
    if(RANK == 0)
        {  
            int numw_0;
            int numw_1;
	        int numw_2;

            int *w_0=malloc(15*sizeof(int));
            int *w_1=malloc(15*sizeof(int));
    	    int *w_2=malloc(15*sizeof(int));

            file = fopen("cluster0.txt", "r");

            if(file != NULL)
                {
                    fscanf(file,"%d",&numw_0);
                        
                    w_0[0]=numw_0;

                    for(int i=1;i<=numw_0;i++)
                            fscanf(file,"%d" , w_0+i);
                }

            else 
                {
                    printf("Error");
                    exit(1);      
                }      

	        

            MPI_Send(&numw_0, 1, MPI_INT, 2, 0, MPI_COMM_WORLD);
                printf("M(0,2)\n");

		    for(int i=1;i<=numw_0;i++)
                {
		            MPI_Send(w_0+i, 1, MPI_INT, 2, 0, MPI_COMM_WORLD);
		                printf("M(0,2)\n");
		        }

            MPI_Recv(&numw_2, 1, MPI_INT, 2, 0, MPI_COMM_WORLD,MPI_STATUS_IGNORE);
            w_2[0]=numw_2;
            
            for(int i=1;i<=numw_2;i++)
                MPI_Recv(w_2+i, 1, MPI_INT, 2, 0, MPI_COMM_WORLD,MPI_STATUS_IGNORE);

		    MPI_Recv(&numw_1, 1, MPI_INT, 2, 0, MPI_COMM_WORLD,MPI_STATUS_IGNORE);
            w_1[0]=numw_1;
             
            for(int i=1;i<=numw_1;i++)
		        MPI_Recv(w_1+i, 1, MPI_INT, 2, 0, MPI_COMM_WORLD,MPI_STATUS_IGNORE);
		
	        for(int i=1;i<=numw_0;i++)
                {	
                    MPI_Isend(&RANK, 1, MPI_INT, *(w_0+i), 0, MPI_COMM_WORLD, &request);
                        printf("M(0,%d)\n" , *(w_0+i));
        
                    MPI_Test(&request, &ok, &status); 
                    
                    if (!ok) 
                        MPI_Wait(&request, &status);
                                     
	                MPI_Send(&numw_0, 1, MPI_INT, *(w_0+i), 0, MPI_COMM_WORLD);
	                    printf("M(0,%d)\n" , *(w_0+i));

	                for(int j=1;j<=numw_0;j++)
                        {
	                        MPI_Send(w_0+j, 1, MPI_INT, *(w_0+i), 0, MPI_COMM_WORLD);
	                            printf("M(0,%d)\n" , *(w_0+i));
	                    }
	
	                MPI_Send(&numw_1, 1, MPI_INT, *(w_0+i), 0, MPI_COMM_WORLD);
	                    printf("M(0,%d)\n" , *(w_0+i));

	                for(int j=1;j<=numw_1;j++)
                        {
	                        MPI_Send(w_1+j, 1, MPI_INT, *(w_0+i), 0, MPI_COMM_WORLD);
	                            printf("M(0,%d)\n" , *(w_0+i));
	                    }
	
	                MPI_Send(&numw_2, 1, MPI_INT, *(w_0+i), 0, MPI_COMM_WORLD);
	                    printf("M(0,%d)\n" , *(w_0+i));

	                for(int j=1;j<=numw_2;j++)
                        {
	                        MPI_Send(w_2+j, 1, MPI_INT, *(w_0+i), 0, MPI_COMM_WORLD);
	                            printf("M(0,%d)\n" , *(w_0+i));
	                    }
	            }
	
            printf("0 -> 0:");
	        printf("%d" , *(w_0+1));

	        for(int i=2;i<=numw_0;i++)
	            printf(",%d" , *(w_0+i));

	        printf(" 1:");
	        printf("%d" , *(w_1+1));

	        for(int i=2;i<=numw_1;i++)
	            printf(",%d" , *(w_1+i));
	
            printf(" 2:");
	        printf("%d" , *(w_2+1));

	        for(int i=2;i<=numw_2;i++)
	            printf(",%d" , *(w_2+i));

	        printf("\n");

	    int v_len=atoi(argv[1]);
        
        int *sol=malloc(sizeof(int)*v_len);//todo alocat dinamic+schimbare de denumiri

        int totw=numw_0+numw_1+numw_2;
        
        double frag_size=(double)v_len/(double)totw;

        int *frag=malloc((totw+1)*sizeof(int));//todo alocati dinamic+schimbare de denumire

        for (int i=0;i<=totw;i++)
            frag[i]=ceil(i*frag_size);

        int len0=frag[numw_0];       
        int len1=frag[numw_0+numw_1]-frag[numw_0];
        int len2=frag[numw_0+numw_1+numw_2]-frag[numw_0+numw_1];
      		
        int *v_0=malloc(len0*sizeof(int));//todo alocati dinamic
        int *v_1=malloc(len1*sizeof(int));
        int *v_2=malloc(len2*sizeof(int));

        for(int i=0;i<len0;i++)
            v_0[i]=i;
        
        for(int i=len0;i<len0+len1;i++)
            v_1[i-len0]=i;
        
        for(int i=len0+len1;i<len0+len1+len2;i++)
            v_2[i-len0-len1]=i;
        
        MPI_Send(&len1, 1, MPI_INT, 2, 0, MPI_COMM_WORLD);
	        printf("M(0,2)\n");

	    MPI_Send(v_1, len1, MPI_INT, 2, 0, MPI_COMM_WORLD);
	        printf("M(0,2)\n");

	    MPI_Send(&len2, 1, MPI_INT, 2, 0, MPI_COMM_WORLD);
	        printf("M(0,2)\n");
 	    
        MPI_Send(v_2, len2, MPI_INT, 2, 0, MPI_COMM_WORLD);
	        printf("M(0,2)\n");
	
        int maxl=0;
	    
        int *arr=malloc(sizeof(int)*numw_0);//todo alocat dinamic

	    for(int i=1;i<=numw_0;i++) 
            {
	            arr[i-1]=ceil((double)len0*i/(double)numw_0-ceil((double)len0*(i-1)/(double)numw_0));
	            
                int temp=arr[i-1];
                int *arr_w=malloc(temp*sizeof(int));

                for(int j=0;j<arr[i-1];j++)
                    arr_w[j]=v_0[maxl++];

	            MPI_Send(arr+i-1, 1, MPI_INT, *(w_0+i), 0, MPI_COMM_WORLD);
	                printf("M(0,%d)\n" , *(w_0+i));
	
                MPI_Send(arr_w , *(arr+i-1), MPI_INT, *(w_0+i), 0, MPI_COMM_WORLD);
	                printf("M(0,%d)\n" , *(w_0+i));
	        }

	    maxl=0;

	    for(int i=1;i<=numw_0;i++) 
            {

                int temp=arr[i-1];
                int *final_arr_w=malloc(temp*sizeof(int));

	            MPI_Recv(final_arr_w, *(arr+i-1), MPI_INT, *(w_0+i), 0, MPI_COMM_WORLD,MPI_STATUS_IGNORE);
	            
                for(int j=0;j<arr[i-1];j++)
                    sol[maxl++]=final_arr_w[j];
	        }

	    for(int i=1;i<=numw_1;i++) 
            {
	            int size1;
	
                MPI_Recv(&size1, 1, MPI_INT, 2, 0, MPI_COMM_WORLD,MPI_STATUS_IGNORE);
	            
                int *final_arr_w=malloc(size1*sizeof(int));//todo alocat dinamic

	            MPI_Recv(final_arr_w, size1, MPI_INT, 2, 0, MPI_COMM_WORLD,MPI_STATUS_IGNORE);
	            
                for(int j=0;j<size1;j++)
	                sol[maxl++]=final_arr_w[j];
	        }

	    for(int i=1;i<=numw_2;i++) 
            {
	            int size2;

	            MPI_Recv(&size2, 1, MPI_INT, 2, 0, MPI_COMM_WORLD,MPI_STATUS_IGNORE);
	            
                int *final_arr_w=malloc(size2*sizeof(int));//todo alocat dinamic
	            
                MPI_Recv(final_arr_w, size2, MPI_INT, 2, 0, MPI_COMM_WORLD,MPI_STATUS_IGNORE);
	
                for(int j=0;j<size2;j++)
	                sol[maxl++]=final_arr_w[j];
	        }

	  	sleep(0.5);
        printf("Rezultat:");

        for(int i=0;i<v_len;i++)
            printf(" %d" , *(sol+i)); 
        
    }

    if(RANK == 1)
        {
            int *w_0=malloc(15*sizeof(int));
            int *w_1=malloc(15*sizeof(int));
	        int *w_2=malloc(15*sizeof(int));

            int numw_0;
            int numw_1;
	        int numw_2;
                
            file = fopen("cluster1.txt", "r");
                if(file != NULL)
                    {   
                        fscanf(file,"%d",&numw_1);
                        w_1[0]=numw_1;

                        for(int i=1;i<=numw_1;i++)
                                fscanf(file,"%d" , w_1+i);
                    }

                else 
                    {
                        printf("Error");
                        exit(1);         
                    }

            MPI_Send(&numw_1, 1, MPI_INT, 2, 0, MPI_COMM_WORLD);
                printf("M(1,2)\n");
	
    	    for(int i=1;i<=numw_1;i++)
                {
		            MPI_Send(w_1+i, 1, MPI_INT, 2, 0, MPI_COMM_WORLD);
		                printf("M(1,2)\n");
		        }

            MPI_Recv(&numw_2, 1, MPI_INT, 2, 0, MPI_COMM_WORLD,MPI_STATUS_IGNORE);
                w_2[0]=numw_2;
             
            for(int i=1;i<=numw_2;i++)
		        MPI_Recv(w_2+i , 1, MPI_INT, 2, 0, MPI_COMM_WORLD,MPI_STATUS_IGNORE);
		
		    MPI_Recv(&numw_0, 1, MPI_INT, 2, 0, MPI_COMM_WORLD,MPI_STATUS_IGNORE);
                w_0[0]=numw_0;
             
            for(int i=1;i<=numw_0;i++)
		        MPI_Recv(w_0+i, 1, MPI_INT, 2, 0, MPI_COMM_WORLD,MPI_STATUS_IGNORE);
		
		    for(int i=1;i<=numw_1;i++)
                {	
                    MPI_Isend(&RANK, 1, MPI_INT, w_1[i], 0, MPI_COMM_WORLD, &request);
                        printf("M(1,%d)\n" , *(w_1+i));
        
                    MPI_Test(&request, &ok, &status); 
                    
                    if (!ok) 
                        MPI_Wait(&request, &status);
              
	                MPI_Send(&numw_0, 1, MPI_INT, w_1[i], 0, MPI_COMM_WORLD);
	                    printf("M(1,%d)\n" , *(w_1+i));
	
                    for(int j=1;j<=numw_0;j++)
                        {
	                        MPI_Send(w_0+j, 1, MPI_INT, *(w_1+i), 0, MPI_COMM_WORLD);
	                            printf("M(1,%d)\n" , *(w_1+i));
	                    }
	
	                MPI_Send(&numw_1, 1, MPI_INT, *(w_1+i), 0, MPI_COMM_WORLD);
	                    printf("M(1,%d)\n" , *(w_1+i));

	                for(int j=1;j<=numw_1;j++)
                        {
	                        MPI_Send(w_1+j , 1, MPI_INT, *(w_1+i), 0, MPI_COMM_WORLD);
	                            printf("M(1,%d)\n" , *(w_1+i));
	                    }
	
	                MPI_Send(&numw_2, 1, MPI_INT, *(w_1+i), 0, MPI_COMM_WORLD);
	                    printf("M(1,%d)\n" , *(w_1+i));
	
                    for(int j=1;j<=numw_2;j++)
                        {
	                        MPI_Send(w_2+j, 1, MPI_INT, w_1[i], 0, MPI_COMM_WORLD);
	                            printf("M(1,%d)\n" , *(w_1+i));
	                    }
	            }

	        printf("1 -> 0:");
	        printf("%d" , *(w_0+1));
	
            for(int i=2;i<=numw_0;i++)
	            printf(",%d" , *(w_0+i));
	
            printf(" 1:");
	        printf("%d" , *(w_1+1));
	
            for(int i=2;i<=numw_1;i++)
	            printf(",%d" , *(w_1+i));
	
            printf(" 2:");
	        printf("%d" , *(w_2+1));
	
            for(int i=2;i<=numw_2;i++)
	            printf(",%d" , *(w_2+i));
	
            printf("\n");
	
            int len1;
	        
            MPI_Recv(&len1, 1, MPI_INT, 2, 0, MPI_COMM_WORLD,MPI_STATUS_IGNORE);
	
            int *v_1=malloc(len1*sizeof(int));//todo alocat dinamic
	        
            MPI_Recv(v_1, len1, MPI_INT, 2, 0, MPI_COMM_WORLD,MPI_STATUS_IGNORE);
	
            int maxl=0;
	        
            int *arr=malloc(numw_1*sizeof(int));//todo alocat dinamic
	
            for(int i=1;i<=numw_1;i++) 
                {	
	                arr[i-1]=ceil((double)len1*i/(double)numw_1-ceil((double)len1*(i-1)/(double)numw_1));
    	        
                    int temp=arr[i-1];
                    int *arr_w=malloc(temp*sizeof(int));

                    for(int j=0;j<arr[i-1];j++)
	                    arr_w[j]=v_1[maxl++];
	
                    MPI_Send(arr+i-1 , 1, MPI_INT, *(w_1+i), 0, MPI_COMM_WORLD);
	                    printf("M(1,%d)\n" , *(w_1+i));
	
                    MPI_Send(arr_w , *(arr+i-1) , MPI_INT , *(w_1+i), 0, MPI_COMM_WORLD);
	                    printf("M(1,%d)\n" , *(w_1+i)); 	
	            }

	        for(int i=1;i<=numw_1;i++) 
                {
                    int temp=arr[i-1];
                    int *final_arr_w=malloc(temp*sizeof(int));

	                MPI_Recv(final_arr_w, *(arr+i-1) , MPI_INT, *(w_1+i), 0, MPI_COMM_WORLD,MPI_STATUS_IGNORE);
	                
                    MPI_Send(arr+i-1 ,1, MPI_INT, 2, 0, MPI_COMM_WORLD);
	                        printf("M(1,2)\n"); 
	
                    MPI_Send(final_arr_w , *(arr+i-1), MPI_INT, 2, 0, MPI_COMM_WORLD);
	                    printf("M(1,2)\n"); 
	            }
	    }

    if(RANK == 2)
        {   
            int *w_0=malloc(15*sizeof(int));
	        int *w_1=malloc(15*sizeof(int));
            int *w_2=malloc(15*sizeof(int));
    		
            int numw_0;
	        int numw_1;
            int numw_2;
   
            file = fopen("cluster2.txt", "r");
                if(file != NULL)
                    {
                        fscanf(file,"%d",&numw_2);
                        w_2[0]=numw_2;

                        for(int i=1;i<=numw_2;i++)
                                fscanf(file,"%d" , w_2+i);
                    }

                else 
                    {
                        printf("Error");
                        exit(1);        
                    } 

            MPI_Recv(&numw_0, 1, MPI_INT, 0, 0, MPI_COMM_WORLD,MPI_STATUS_IGNORE);
                w_0[0]=numw_0;
            
            for(int i=1;i<=numw_0;i++)
		        MPI_Recv(w_0+i, 1, MPI_INT, 0, 0, MPI_COMM_WORLD,MPI_STATUS_IGNORE);
		
		    MPI_Recv(&numw_1,1, MPI_INT, 1, 0, MPI_COMM_WORLD,MPI_STATUS_IGNORE);
                w_1[0]=numw_1;
             
            for(int i=1;i<=numw_1;i++)
		        MPI_Recv(w_1+i, 1, MPI_INT, 1, 0, MPI_COMM_WORLD,MPI_STATUS_IGNORE);

		    MPI_Send(&numw_2, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
                printf("M(2,0)\n");
		
            for(int i=1;i<=numw_2;i++)
                {
		            MPI_Send(w_2+i, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
		                printf("M(2,0)\n");
		        }
		          
            MPI_Send(&numw_1, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
                printf("M(2,0)\n");
		
            for(int i=1;i<=numw_1;i++)
                {
		            MPI_Send(w_1+i, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
		                printf("M(2,0)\n");
		        }
		          
            MPI_Send(&numw_2, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
                printf("M(2,1)\n");
		
            for(int i=1;i<=numw_2;i++)
                {
		            MPI_Send(w_2+i, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
		                printf("M(2,1)\n");
		        }
		         
            MPI_Send(&numw_0, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
                printf("M(2,1)\n");
		
            for(int i=1;i<=numw_0;i++)
                {
		            MPI_Send(w_0+i, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
		                printf("M(2,1)\n");
		        }

		    for(int i=1;i<=numw_2;i++)
                {	
                    MPI_Isend(&RANK, 1, MPI_INT, *(w_2+i), 0, MPI_COMM_WORLD, &request);
                        printf("M(2,%d)\n" , *(w_2+i));
        
                    MPI_Test(&request, &ok, &status); 
         
                    if (!ok) 
                        MPI_Wait(&request, &status);
        
	                MPI_Send(&numw_0, 1, MPI_INT, *(w_2+i), 0, MPI_COMM_WORLD);
	                    printf("M(2,%d)\n" , *(w_2+i));
	
                    for(int j=1;j<=numw_0;j++)
                        {
	                        MPI_Send(w_0+j, 1, MPI_INT, *(w_2+i), 0, MPI_COMM_WORLD);
	                            printf("M(2,%d)\n" , *(w_2+i));
	                    }
	
	                MPI_Send(&numw_1, 1, MPI_INT, *(w_2+i), 0, MPI_COMM_WORLD);
	                    printf("M(2,%d)\n" , *(w_2+i));
	
                    for(int j=1;j<=numw_1;j++)
                        {
	                        MPI_Send(w_1+j , 1, MPI_INT, *(w_2+i), 0, MPI_COMM_WORLD);
	                            printf("M(2,%d)\n" , *(w_2+i));
	                    }
	
	                MPI_Send(&numw_2, 1, MPI_INT, *(w_2+i), 0, MPI_COMM_WORLD);
	                    printf("M(2,%d)\n" , *(w_2+i));
	
                    for(int j=1;j<=numw_2;j++)
                        {
	                        MPI_Send(w_2+j, 1, MPI_INT, *(w_2+i), 0, MPI_COMM_WORLD);
	                            printf("M(2,%d)\n" , *(w_2+i));
	                    }
	            }

	        printf("2 -> 0:");
	        printf("%d" , *(w_0+1));

	        for(int i=2;i<=numw_0;i++)
	            printf(",%d" , *(w_0+i));
	
            printf(" 1:");
	        printf("%d" , *(w_1+1));
	
            for(int i=2;i<=numw_1;i++)
	            printf(",%d" , *(w_1+i));
	
            printf(" 2:");
	        printf("%d" , *(w_2+1));
	        
            for(int i=2;i<=numw_2;i++)
	            printf(",%d" , *(w_2+i));
	
            printf("\n");
	
	        int len1;
	        
            MPI_Recv(&len1, 1, MPI_INT, 0, 0, MPI_COMM_WORLD,MPI_STATUS_IGNORE);
	
            int *v_1=malloc(len1*sizeof(int));//alocat dinamic
	        
            MPI_Recv(v_1, len1, MPI_INT, 0, 0, MPI_COMM_WORLD,MPI_STATUS_IGNORE);
	
	        MPI_Send(&len1, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
	            printf("M(2,1)\n");
	
            MPI_Send(v_1, len1, MPI_INT, 1, 0, MPI_COMM_WORLD);
	            printf("M(2,1)\n");
	
	        int len2;
	        
            MPI_Recv(&len2, 1, MPI_INT, 0, 0, MPI_COMM_WORLD,MPI_STATUS_IGNORE);
	
            int *v_2=malloc(len2*sizeof(int));//alocat dinamic
	        
            MPI_Recv(v_2, len2, MPI_INT, 0, 0, MPI_COMM_WORLD,MPI_STATUS_IGNORE);
	
            int maxl=0;
	        
            int *arr=malloc(numw_2*sizeof(int));//alocat dinamic
	
            for(int i=1;i<=numw_2;i++) 
                {
	                arr[i-1]=ceil((double)len2*i/(double)numw_2-ceil((double)len2*(i-1)/(double)numw_2));
	                
                    int temp=arr[i-1];
                    int *arr_w=malloc(temp*sizeof(int));
	
                    for(int j=0;j<arr[i-1];j++)
	                    arr_w[j]=v_2[maxl++];

	                MPI_Send(arr+i-1, 1, MPI_INT, *(w_2+i), 0, MPI_COMM_WORLD);
	                    printf("M(2,%d)\n" , *(w_2+i));
	
                    MPI_Send(arr_w , *(arr+i-1), MPI_INT, *(w_2+i), 0, MPI_COMM_WORLD);
	                    printf("M(2,%d)\n" , *(w_2+i)); 
	            }

	        for(int i=1;i<=numw_1;i++) 
                {
	                int size1;
	                
                    MPI_Recv(&size1, 1, MPI_INT, 1, 0, MPI_COMM_WORLD,MPI_STATUS_IGNORE);
	
                    int *final_arr_w=malloc(size1*sizeof(int));//todo alocat dinamic
	                
                    MPI_Recv(final_arr_w, size1, MPI_INT, 1, 0, MPI_COMM_WORLD,MPI_STATUS_IGNORE);
	                
                    MPI_Send(&size1, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
	                    printf("M(2,0)\n"); 
	                
                    MPI_Send(final_arr_w, size1, MPI_INT, 0, 0, MPI_COMM_WORLD);
	                    printf("M(2,0)\n"); 
	            }

	        for(int i=1;i<=numw_2;i++) 
                {
                    int temp=arr[i-1];
                    int *final_arr_w=malloc(temp*sizeof(int));
	                
                    MPI_Recv(final_arr_w, *(arr+i-1), MPI_INT, *(w_2+i), 0, MPI_COMM_WORLD,MPI_STATUS_IGNORE);
	
                    MPI_Send(arr+i-1, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
	                    printf("M(2,0)\n"); 
	
                    MPI_Send(final_arr_w, *(arr+i-1), MPI_INT, 0, 0, MPI_COMM_WORLD);
    	                printf("M(2,0)\n"); 
	            }
	    }

  	if(RANK > 2)
        {
  	        
            int numw_0;
	        int numw_1;
	        int numw_2;

            int *w_0=malloc(15*sizeof(int));//alocati dinamic
	        int *w_1=malloc(15*sizeof(int));
	        int *w_2=malloc(15*sizeof(int));
	
            int crd;
  	        int ok=0;
  	
            while(ok == 0)
                {
  	                sleep(1);
  	    
                    MPI_Irecv(&crd, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &request);
  	                
                      MPI_Test(&request, &ok, &status);
  	
                    if(ok == 0)
                        {
 	                        MPI_Irecv(&crd, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, &request);
 	                        MPI_Test(&request, &ok, &status);
 	                    }
 	
                    if(ok == 0)
                        {
                            MPI_Irecv(&crd, 1, MPI_INT, 2, 0, MPI_COMM_WORLD, &request);
                            MPI_Test(&request, &ok, &status);
                        }
        
                    if (ok == 0) 
                        MPI_Wait(&request, &status);
        
                }

  	        MPI_Recv(&numw_0,1, MPI_INT, crd, 0, MPI_COMM_WORLD,MPI_STATUS_IGNORE);
  	            w_0[0]=numw_0;
  	  
            for(int i=1;i<=numw_0;i++)
	            MPI_Recv(w_0+i,1, MPI_INT, crd, 0, MPI_COMM_WORLD,MPI_STATUS_IGNORE);
	
	        MPI_Recv(&numw_1,1, MPI_INT, crd, 0, MPI_COMM_WORLD,MPI_STATUS_IGNORE);
  	            w_1[0]=numw_1;
  	  
            for(int i=1;i<=numw_1;i++)
	            MPI_Recv(w_1+i,1, MPI_INT, crd, 0, MPI_COMM_WORLD,MPI_STATUS_IGNORE);
	
	        MPI_Recv(&numw_2,1, MPI_INT, crd, 0, MPI_COMM_WORLD,MPI_STATUS_IGNORE);
  	            w_2[0]=numw_2;
  	  
            for(int i=1;i<=numw_2;i++)
	            MPI_Recv(w_2+i,1, MPI_INT, crd, 0, MPI_COMM_WORLD,MPI_STATUS_IGNORE);
	
            printf("%d -> 0:",RANK);
	        printf("%d", *(w_0+1));
	
            for(int i=2;i<=numw_0;i++)
	            printf(",%d" , *(w_0+i));
	
            printf(" 1:");
	        printf("%d" , *(w_1+1));
	
            for(int i=2;i<=numw_1;i++)
	            printf(",%d" , *(w_1+i));
	
            printf(" 2:");
	        printf("%d" , *(w_2+1));
	
            for(int i=2;i<=numw_2;i++)
	            printf(",%d" , *(w_2+i));
	
            printf("\n");
	
    	    int size;

	        MPI_Recv(&size, 1, MPI_INT, crd, 0, MPI_COMM_WORLD,MPI_STATUS_IGNORE);
	
            int *v=malloc(size*sizeof(int));//alocat dinamic
	        
            MPI_Recv(v, size, MPI_INT, crd, 0, MPI_COMM_WORLD,MPI_STATUS_IGNORE);
	
            for(int i=0;i<size;i++)
	            v[i]=2*v[i];
	
            MPI_Send(v, size, MPI_INT, crd, 0, MPI_COMM_WORLD);
	            printf("M(%d,%d)\n",RANK,crd);
  	    }
   
    MPI_Finalize();

    return 0;
}

