# Kalafut and Visscher SIC step fitting algorithm implementation  

I have the code organized into three files...these are in the 'Kalafut' folder  
     -- kal_main.c       the main file   
     -- SICalgorithm.c   file that does SIC algorithm for no step hypoth. and for adding steps  
     -- InputTrace.c     file that reads in a dataset from a file  
(there are corresponding header files where needed)  
To run the code:  
     1) type make to compile, kal becomes the executable  
     2) to run: type "./kal dataset > output"  
    
Notes: the dataset should have three columns:  
            #time  force  position  
       the output file has two columns:  
       	    #time  position  
