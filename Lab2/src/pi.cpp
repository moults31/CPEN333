#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include <math.h>
#include <random>

#define RADIUS 1.0

void pi_sampler(std::vector<bool>& hits, int idx);
double estimate_pi_multithread_naive(int nsamples);
void pi_hits(std::vector<int> &hits, int tidx, int nsamples);
double estimate_pi_multithread(int nsamples);

// estimates the value of pi single-threadedly
double estimate_pi(int nsamples) {
    std::vector<bool> hits(nsamples);
    double nhits = 0;
    
    for(int i=0; i<nsamples; i++)
    {
        pi_sampler(hits, i);
        if(hits[i]) nhits++;
    }
    
    double pi = nhits/nsamples;
    
    return pi*4.0;
}


// generates a random sample and sets hits[idx]
// to true if within the unit circle
void pi_sampler(std::vector<bool>& hits, int idx) {
    // single instance of random engine and distribution
    static std::default_random_engine rnd(
              std::chrono::system_clock::now().time_since_epoch().count());
    static std::uniform_real_distribution<double> dist(-1.0, 1.0);
    
    // get random [x,y]
    double x = dist(rnd);
    double y = dist(rnd);
    
    // convert to polar (r only)
    double r = sqrt(x*x + y*y);
    
    // if pt fell upon circle set hits to true else not
    if(r > RADIUS)
    {
        hits[idx] = false;
    }
    else
    {
        hits[idx] = true;
    }
}

// naively uses multithreading to try to speed up computations
double estimate_pi_multithread_naive(int nsamples) {
  // stores whether each sample falls within circle
  std::vector<bool> hits(nsamples, false);

  // create and store all threads
  std::vector<std::thread> threads;
  for (int i=0; i<nsamples; ++i) {
    threads.push_back(std::thread(pi_sampler, std::ref(hits), i));
  }

  // wait for all threads to complete
  for (int i=0; i<nsamples; ++i) {
    threads[i].join();
  }

  // estimate our value of PI
  double pi = 0;
  for (int i=0; i<nsamples; ++i) {
    if (hits[i]) {
      pi = pi + 1;
    }
  }
  pi = pi / nsamples*4;

  return pi;
}

// Increments the number of pi hits
void pi_hits(std::vector<int> &hits, int tidx, int nsamples) {
    // single instance of random engine and distribution
    static std::default_random_engine rnd(
                            std::chrono::system_clock::now().time_since_epoch().count());
    static std::uniform_real_distribution<double> dist(-1.0, 1.0);
    
    for(int i=0; i<nsamples; i++)
    {
        // get random [x,y]
        double x = dist(rnd);
        double y = dist(rnd);
        
        // get distance from centre using pythagorus
        double r = sqrt(x*x + y*y);
        
        // if pt fell upon circle increment hits for this thread
        if(r <= RADIUS)
        {
            hits[tidx]++;
        }
    }
}

// divides work among threads intelligently
double estimate_pi_multithread(int nsamples) {
    
    // number of available cores
    int nthreads = std::thread::hardware_concurrency();
    
    // hit counts
    std::vector<int> hits(nthreads, 0);
    
    // create and store threads
    std::vector<std::thread> threads;
    int msamples = 0; // samples accounted for
    for (int i=0; i<nthreads-1; ++i) {
        threads.push_back(
                          std::thread(pi_hits, std::ref(hits), i, nsamples/nthreads));
        msamples += nsamples/nthreads;
    }
    // remaining samples
    threads.push_back(
                      std::thread(pi_hits, std::ref(hits), nthreads-1, nsamples-msamples));
    
    // wait for threads to finish
    for (int i=0; i<nthreads; ++i) {
        threads[i].join();
    }
    
    // estimate pi
    double pi = 0;
    for (int i=0; i<nthreads; ++i) {
        pi += hits[i];
    }
    pi = pi/nsamples*4;
    
    return pi;
}

int main(void) {
    // Estimate pi using single threaded approach and time and report it
    auto t1 = std::chrono::high_resolution_clock::now();
    double pi = estimate_pi(100000000);
    auto t2 = std::chrono::high_resolution_clock::now();
    auto duration = t2 - t1;
    auto duration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(duration);
    long ms = duration_ms.count();
    std::cout << "Pi estimate (ST): " << pi << " (" << ms << "ms)" << std::endl;

    // Estimate pi using naive multi threaded approach and time and report it
    t1 = std::chrono::high_resolution_clock::now();
    pi = estimate_pi_multithread_naive(1000);
    t2 = std::chrono::high_resolution_clock::now();
    duration = t2 - t1;
    duration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(duration);
    ms = duration_ms.count();
    std::cout << "Pi estimate (MT naive): " << pi << " (" << ms << "ms)" << std::endl;
  
    // Estimate pi using intelligent multi threaded approach and time and report it
    t1 = std::chrono::high_resolution_clock::now();
    pi = estimate_pi_multithread(100000000);
    t2 = std::chrono::high_resolution_clock::now();
    duration = t2 - t1;
    duration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(duration);
    ms = duration_ms.count();
    std::cout << "Pi estimate (MT): " << pi << " (" << ms << "ms)" << std::endl;
    
    return 0;
}
