#include <iostream>
#include <thread>
#include <vector>
#include <chrono>

#define _MAXTHREADS 6

// partitions elements low through high (inclusive)
// around a pivot and returns the pivot index
size_t partition(std::vector<int>& data, int low, int high)
{
    int pivot = data[high];
    int i = low-1;
    int tmp;
    
    for(size_t j = low; j < high; j++)
    {
        // If jth element <= pivot, do i++ and swap data @ i,j
        if(data[j] <= pivot)
        {
            i++;
            tmp = data[i];
            data[i] = data[j];
            data[j] = tmp;
        }
    }
    
    // Place pivot at the upper end of the left partition
    // by swapping it with the index just greater than i
    tmp = data[high];
    data[high] = data[i+1];
    data[i+1] = tmp;
    
    return i+1;
}

// sorts elements low through high (inclusive) using a single thread
void quicksort(std::vector<int>& data, int low, int high)
{
    size_t pi;
    
    if(low < high)
    {
        pi = partition(data, low, high);
        
        quicksort(data, low, pi-1); // Up to partition
        quicksort(data, pi+1, high); // Above partition
    }
}

void thread_fn(int idx)
{
    std::cout << "hellow thrad" << idx << std::endl;
}

// sorts elements low through high (inclusive) using multiple threads
void parallel_quicksort(std::vector<int>& data, int low, int high, int* numThreads, int MAXTHREADS)
{
    if(low < high)
    {
        const size_t &pi = partition(data, low, high);
        const size_t &lopi = pi - 1;
        const size_t &hipi = pi + 1;

        //std::cout << *numThreads << std::endl << std::endl;
        
        if(*numThreads >= MAXTHREADS)
        {
            parallel_quicksort(data, low, lopi, numThreads, MAXTHREADS);
            parallel_quicksort(data, hipi, high, numThreads, MAXTHREADS);
        }
        else
        {
            (*numThreads)++;
            std::thread t1(     parallel_quicksort,
                                std::ref(data),
                                low,
                                lopi,
                                std::ref(numThreads),
                                std::ref(MAXTHREADS)
                                );
            
            (*numThreads)++;
            std::thread t2(     parallel_quicksort,
                                std::ref(data),
                                hipi,
                                high,
                                std::ref(numThreads),
                                std::ref(MAXTHREADS)
                                );
            
            t1.join();
            (*numThreads)--;
            t2.join();
            (*numThreads)--;
        }
    }
}

// Tests single threaded quicksort for accuracy
bool unitTest_quicksort()
{
    std::vector <int> test {1, 5, 4, 3, 2};
    std::vector <int> expected {1, 2, 3, 4, 5};
    
    quicksort(test, 0, test.size()-1);
    
    for(int i=0; i<test.size(); i++)
    {
        if(test[i] != expected[i])
        {
            std::cout << "Quicksort test failed" << std::endl;
            return false;
        }
    }
    
    std::cout << "Quicksort test passed" << std::endl;
    return true;
}

// Tests multi threaded quicksort for accuracy
bool unitTest_parallel_quicksort()
{
    std::vector <int> test {1, 5, 4, 3, 2};
    std::vector <int> expected {1, 2, 3, 4, 5};
    std::vector <std::thread> threads;
    
    parallel_quicksort(test, 0, test.size()-1, 0, 4);
    
    for(int i=0; i<test.size(); i++)
    {
        if(test[i] != expected[i])
        {
            std::cout << "Prll Quicksort test failed" << std::endl;
            return false;
        }
    }
    
    std::cout << "Prll Quicksort test passed" << std::endl;
    return true;
}


int main()
{
    // create two copies of random data
    const int VECTOR_SIZE = 10000000;
    std::vector<int> v1(VECTOR_SIZE, 0);
    std::vector<std::thread> threads;
    int MAXTHREADS = _MAXTHREADS;
 
    // fill with random integers
    for (int i=0; i<VECTOR_SIZE; ++i)
    {
        v1[i] = rand();
    }
    std::vector<int> v2 = v1;  // copy all contents

    // sort v1 using sequential algorithm and measure time taken
    auto t1 = std::chrono::high_resolution_clock::now();
    quicksort(v1, 0, v1.size()-1);
    auto t2 = std::chrono::high_resolution_clock::now();
    auto duration_seq = t2 - t1;
    
    // cast duration of single threaded qsort to ms,
    // and report it on stdout
    auto duration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(duration_seq);
    long ms = duration_ms.count();
    std::cout << "Quicksort took " << ms << " ms" << std::endl;
    
    // report how many threads are supported on this hardware
    unsigned int n = std::thread::hardware_concurrency();
    std::cout << n << " concurrent threads are supported.\n";
    
    // sort v2 using parallel algorithm and measure time taken
    int *numThreads = new int;
    t1 = std::chrono::high_resolution_clock::now();
    parallel_quicksort(v2, 0, v2.size()-1, numThreads, MAXTHREADS);
    t2 = std::chrono::high_resolution_clock::now();
    auto duration_prll = t2 - t1;
    delete numThreads;

    // cast duration of single threaded qsort to ms,
    // and report it on stdout
    duration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(duration_prll);
    ms = duration_ms.count();
    std::cout << "Prll quicksort took " << ms << " ms" << std::endl;
    
    double speedup = (double)duration_seq.count()/(double)duration_prll.count();
    std::cout << "num threads: " << MAXTHREADS << std::endl;
    std::cout << "Speedup factor: " << speedup << std::endl << std::endl;
    return 0;
}
