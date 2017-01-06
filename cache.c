#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<unistd.h>
#include<limits.h>
#include<assert.h>
#include<string.h>

#define ERR_MSG( msg, errno ) {					\
		char * mess = msg;				\
		int err = errno;				\
		(void)fflush(stderr); 				\
                (void)fprintf(stderr, __FILE__ ":%d:%s:%s\n\n",	\
		             __LINE__, mess, strerror(err));	\
}

int n,k,*MemRequests;
int *CacheArray,*EvictionNumber;
char * progname;
FILE * fp, * fw;

char * PROG_USAGE =
"\n\nUsage: %s path to text file containing input \n\
Cache management algorithm minimizes the number of cache misses over the entire list of cache requests. \n\
\n\
Arguments:\n\
    (Required) Name of text file containing input: input.txt\n\
\n\
Input text file contents:\n\
Line 1: Integer n and Integer k separated by a space,\n\
Line 2: n integers which are the memory requests\n\
\n\n";

int IntegerConversion(char * str){
	long lnum;
	int num;
	char *end;
	errno = 0;
	lnum = strtol(str, &end, 10);            /*10 specifies base-10*/

	if (end == str){                 /*if no characters were converted these pointers are equal*/
		ERR_MSG("ERROR: can't convert string to number\n", EINVAL);
        	exit(EXIT_FAILURE);
    	}

    	/*If sizeof(int) == sizeof(long),we have to explicitly check for overflows*/
    	if ((lnum == LONG_MAX || lnum == LONG_MIN) && errno == ERANGE){
		ERR_MSG("ERROR: number out of range for LONG\n", ERANGE);
        	exit(EXIT_FAILURE);
    	}

    	/*Because strtol produces a long, check for overflow*/
    	if ( (lnum > INT_MAX) || (lnum < INT_MIN) ){
		ERR_MSG("ERROR: number out of range for INT\n", ERANGE);
        	exit(EXIT_FAILURE);
    	}

    	/*Finally convert the result to a plain int (if that's what you want)*/
    	return num = (int) lnum;
}


//Opens input file and saves contents into appropriate arrays

void ReadFile(char * filename){

	assert((fp=fopen(filename,"r")) && "Can't open file.Incorrect filename");
	char * buf = malloc(sizeof(char) * 255);
	char * token;
	fgets(buf,255,fp);
	token = strtok(buf, " ");
	n = IntegerConversion(token);
	token = strtok(NULL, " ");



	if(n<=0){
	printf("Invalid input. The number of memory requests cannot be 0 or less\n");
	exit(EXIT_FAILURE);
	}
 
	k= IntegerConversion(token);

	if(k<=0){
	printf("Invalid input. The cache size cannot be 0 or less\n");
	exit(EXIT_FAILURE);
	}

	MemRequests = (int*)malloc(sizeof(int)*(unsigned long)(n+1));
	CacheArray = (int*)malloc(sizeof(int)*(unsigned long)(k+1));
    	int i;
     	fgets(buf,255,fp);
	token = strtok(buf, " ");

    	for(i = 0; i < n; i++){

		assert((token) && "Too few requests. ");
		MemRequests[i]=IntegerConversion(token);     
	token = strtok(NULL, " ");
		if(MemRequests[i]<=0){
		printf("Invalid refernce string. Memory request should be a positive integer.\n ");
		exit(EXIT_FAILURE);
		}
	}

	
    	assert((fclose(fp)!= EOF) && "Can't close the file.");
    	free(buf);
}

void PrintUse(){
	printf(PROG_USAGE, progname);
    	exit(EXIT_FAILURE);
}

int EvictPosition(int i){
	int p=0; 	//cache page
	int s = i+1;	//memory request
	int e=0;	//page to be evicted
	int pos = 0;	//earliest appearance, in reference string, of page to be evicted
	while(p < k && s < n){
		if (CacheArray[p] == MemRequests[s]){
			if(s > pos){
				e = p;
				pos = s;
				p = p+1;
				s = i+1;
			}
			else{
		 		p = p+1;
				s = i+1;
			}
		}
		else{
		 	s = s+1;
		}
	}
	if(p == k){
		return e;
	}
	return p;
}

void PrintInput(){
	printf("\n");
	int i;
	printf("Memory request size= %d, Cache size= %d\n",n,k);
	printf("Memory Requests:  ");
	for(i=0;i<n;i++){

		printf("%d  ",MemRequests[i]);
	}
	printf("\nCache: ");
	for(i=0;i<k;i++){

		printf("%d  ",CacheArray[i]);
	}
	printf("\n");
}
int main(int argc, char ** argv){
	char * filename;
	int i,j,l;
	EvictionNumber = (int*)malloc(sizeof(int)*(unsigned long)(n+1));
	progname = argv[0];
	if(argc != 2 ){
	        PrintUse();
	}
	filename = argv[1];
    	ReadFile(filename);
	printf("Filename: %s\n", filename);
	//PrintInput();
		// # of elements in cache
	int CacheElementCount = 0;
		// Is cache hit - true / false?
	int IsCacheHit = 0;
	int CacheEvictionCount = 0;
		// Process each of the requests in given reference string
	for(i=0;i<n;i++){
		// Current memory request is MemRequests[i]
		// Verify if it is a Cache hit
		IsCacheHit = 0;
		for (j=0; j<CacheElementCount; j++){
			if (MemRequests[i]== CacheArray[j]) {
				// cache hit; Hence no page evictions are needed
				IsCacheHit = 1;
				EvictionNumber[i]=0;
				break;
			}
		}
		if (IsCacheHit == 1)
			continue;
		//Cache Miss; New request needs to be stored in cache
		// Verify if the cache is full
		if (CacheElementCount < k){
			// Cache is not full
			// Insert new request in cache at the rear
			CacheArray[CacheElementCount]=MemRequests[i];
			CacheElementCount++;
			// No cache page evictions needed
			EvictionNumber[i]=0;
			continue;
		} 
		// Cache is full; Select a page to be evicted
		int e= EvictPosition(i);
		//Replace the cache page with new request
		CacheArray[e]=MemRequests[i];
		EvictionNumber[i]=e+1;
		CacheEvictionCount++;
		
	}

	PrintInput();
	for(i=0;i<n;i++){
		printf("%d\t",MemRequests[i]);
	}
	printf("\n");
	for(i=0;i<n;i++){
		printf("%d\t",EvictionNumber[i]);
	}
	printf("\n");
	printf("\nCache Eviction Count: %d\n",CacheEvictionCount);
	return 0;
}

