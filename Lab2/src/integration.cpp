#include <iostream>
#include <random>
#include <thread>
#include <cmath>
#include <vector>
#include <chrono>

#define NUMSAMPLES 1000000
#define RADIUS 1.0
#define DENSITY density1

using namespace std;

// three-dimensional point
struct pt_t
{
  double x,y,z;
};

double density1(pt_t pt)
{
  return exp(-1 * pow(abs(pt.x),2));
}

double density2(pt_t pt)
{
  return abs(pt.x + pt.y + pt.z);
}

double density3(pt_t pt)
{
  return pow(pt.x - 1, 2) + pow(pt.y - 2, 2) + pow(pt.z - 3, 2);
}

// virtual base class for functions
class Function
{
 public:
  Function() {};
  virtual double operator()(pt_t pt)
  {
    return DENSITY(pt);
  };
};

// computes x*fn(x,y,z)
class XFunction : public Function
{
  Function& fn;
 public:
  XFunction(Function& fn) : fn(fn){};
  double operator()(pt_t pt) {
    
    return pt.x*fn(pt);
  }
};

// computes y*fn(x,y,z)
class YFunction : public Function
{
  Function& fn;
 public:
  YFunction(Function& fn) : fn(fn){};
  double operator()(pt_t pt)
  {
    return pt.y*fn(pt);
  }
};

// computes z*fn(x,y,z)
class ZFunction : public Function
{
  Function& fn;
 public:
  ZFunction(Function& fn) : fn(fn){};
  double operator()(pt_t pt)
  {
    return pt.z*fn(pt);
  }
};

pt_t estimateC_ST(int nsamples);
pt_t estimateC_MT(int nsamples);
void computeMoment(vector<double>& xrho, vector<double>& yrho,
                 vector<double>& zrho, vector<double>& rho, pt_t pt, int idx);
void computeFns(vector<double> xrho, vector<double> yrho, vector<double> zrho, vector<double> rho, pt_t pt, int idx);
void sample(vector<double>& xrho, vector<double>& yrho,
            vector<double>& zrho, vector<double>& rho, int idx, int nsamples);
bool isInSphere(pt_t pt);
pt_t centreOfMass(vector<double>& xrho, vector<double>& yrho,
                  vector<double>& zrho, vector<double>& rho, int n);

// Compute centre of mass of the sphere
pt_t estimateC_ST(int nsamples)
{
  vector<double> xrho(NUMSAMPLES);
  vector<double> yrho(NUMSAMPLES);
  vector<double> zrho(NUMSAMPLES);
  vector<double> rho(NUMSAMPLES);
  
  sample(xrho, yrho, zrho, rho, 0, nsamples);
  
  return centreOfMass(xrho, yrho, zrho, rho, nsamples);
}

// Compute centre of mass of the sphere using multiple threads
pt_t estimateC_MT(int nsamples)
{
  vector<double> xrho(NUMSAMPLES);
  vector<double> yrho(NUMSAMPLES);
  vector<double> zrho(NUMSAMPLES);
  vector<double> rho(NUMSAMPLES);
  vector<thread> threads;
  int nthreads = thread::hardware_concurrency();
  int samplesHandled = 0;
  int threadSamples = nsamples / nthreads;
  
  // create nthreads threads and assign each of them an equal share of samples to handle
  for(int i=0; i<nthreads-1; i++)
  {
    threads.push_back(thread(sample, ref(xrho), ref(yrho), ref(zrho), ref(rho), i, threadSamples));
    samplesHandled += threadSamples;
  }
  
  // create threads for the extra samples that were the remainder in "nsamples / nthreads"
  threads.push_back(thread(sample, ref(xrho), ref(yrho), ref(zrho), ref(rho),
                           nsamples-threadSamples-1, nsamples-threadSamples));
  
  for (int i=0; i<nthreads; ++i) {
    threads[i].join();
  }
  
  return centreOfMass(xrho, yrho, zrho, rho, nsamples);
}

// get random coords [x,y,z]. If they do not fall inside
// unit sphere, keep trying until they do.
// repeat this nsamples times starting at idx and incrementing each sample.
void sample(vector<double>& xrho, vector<double>& yrho,
            vector<double>& zrho, vector<double>& rho, int idx, int nsamples)
{
  static std::default_random_engine rnd(
                                        std::chrono::system_clock::now().time_since_epoch().count());
  static std::uniform_real_distribution<double> dist(-1.0, 1.0);
  pt_t pt;
  
  for(int i=0; idx+i<nsamples; i++)
  {
    do
    {
      pt.x = dist(rnd);
      pt.y = dist(rnd);
      pt.z = dist(rnd);
    }while(!isInSphere(pt));
  
    computeMoment(xrho, yrho, zrho, rho, pt, idx+i);
  }
}

// compute the moment at this sample in [x,y,z] and store it in the vects
void computeMoment(vector<double>& xrho, vector<double>& yrho,
                    vector<double>& zrho, vector<double>& rho, pt_t pt, int idx)
{
  Function fn;
  rho[idx] = fn(pt);
  
  XFunction xfn(fn);
  xrho[idx] = xfn(pt);
  
  YFunction yfn(fn);
  yrho[idx] = yfn(pt);
  
  ZFunction zfn(fn);
  zrho[idx] = zfn(pt);
}

// Return true if point is within sphere of radius RADIUS, else return false
bool isInSphere(pt_t pt)
{
  return sqrt(pt.x*pt.x + pt.y*pt.y + pt.z*pt.z) > RADIUS ? false : true;
}

// Calculate centre of mass given densities and moments
pt_t centreOfMass(vector<double>& xrho, vector<double>& yrho,
                      vector<double>& zrho, vector<double>& rho, int n)
{
  double rhosum = 0, xsum = 0, ysum = 0, zsum = 0;
  double rhoavg, xavg, yavg, zavg;
  pt_t c;
  
  for(int i=0; i<n; i++)
  {
    rhosum += rho[i];
    xsum += xrho[i];
    ysum += yrho[i];
    zsum += zrho[i];
  }
  
  rhoavg = rhosum/n;
  xavg = xsum/n;
  yavg = ysum/n;
  zavg = zsum/n;
  
  c.x = xavg/rhoavg;
  c.y = yavg/rhoavg;
  c.z = zavg/rhoavg;
  
  return c;
}

int main()
{
  pt_t c;
  
  // Estimate centre of mass using single threaded approach and time and report it
  auto t1 = std::chrono::high_resolution_clock::now();
  c = estimateC_ST(NUMSAMPLES);
  auto t2 = std::chrono::high_resolution_clock::now();
  auto duration = t2 - t1;
  auto duration_ms_st = std::chrono::duration_cast<std::chrono::milliseconds>(duration);
  long ms = duration_ms_st.count();
  cout << endl << "Centre of mass is located at:" << endl;
  cout << "x: " << c.x << " y: " << c.y << " z: " << c.z << endl;
  cout << "Found in " << ms << "ms using " << NUMSAMPLES << " samples and single threaded program" << endl << endl;
  
  // Estimate centre of mass using multi threaded approach and time and report it
  t1 = std::chrono::high_resolution_clock::now();
  c = estimateC_MT(NUMSAMPLES);
  t2 = std::chrono::high_resolution_clock::now();
  duration = t2 - t1;
  auto duration_ms_mt = std::chrono::duration_cast<std::chrono::milliseconds>(duration);
  ms = duration_ms_mt.count();
  cout << "Centre of mass is located at:" << endl;
  cout << "x: " << c.x << " y: " << c.y << " z: " << c.z << endl;
  cout << "Found in " << ms << "ms using " << NUMSAMPLES << " samples and multi threaded program" << endl;
  
  // Compute speedup and report it
  double speedup = (double)duration_ms_st.count()/(double)duration_ms_mt.count();
  cout << endl << "Speedup factor: " << speedup << endl;
  
  return 0;
}
