## Spring-2015-Advanced-Algorithms

###Problem statement
Given,
A sequence of n memory requests - R1, R2, R3 ... Rn;
Size of cache C is k
For each request Ri in given sequence:
* check whether Ri is in cache
* If yes, then it is a cache hit
* if no, then cache miss
  * check whether the cache is full
  * If not full, then add the element to cache
  * If full, then evict an element from cache to make room for Ri
  
###Goal
To minimize the number of cache misses over the entire
sequence of requests. 

###Greedy strategy (furthest-in-future)
To evict an element from cache, select the item in cache whose next access in the request
sequence comes furthest in the future. 
