#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <math.h>
#include <time.h>

using std::cin;
using std::cout;
using std::endl;
using std::ofstream;
using std::pair;
using std::string;
using std::vector;

// Define the particle
struct Particle
{ // Particle characterized by its location.
    float x = 0;
    float y = 0;
    // Particle Constructors
    Particle() {} // default constructor
    Particle(float newX, float newY)
    {
        x = newX;
        y = newY;
    }
};
// Subroutines
void randDisplace(Particle &particle, float dx, float M); // outputs a negative dx or positive dx
void InitDistribute_Center(vector<Particle> &array);
void InitDistribute_Corners(vector<Particle> &array, int M);
void calcEntropy(ofstream &f2, vector<vector<int>> &partitionCount, vector<pair<float, float>> &entropies, int n,
                 int T, float t, int N, float &finalEAvg, float &avgCount, float &S1_0);
void calcRelax(ofstream &f3, ofstream &f4, vector<pair<float, float>> &entropies, float S1_inf);
void storeState(ofstream &f, Particle particle, float t, float timeStorageInterval);

int main()
{
    // Setting up initial conditions
    int N = 2000; // number of test particles
    int M = 100;  // number of cells (100 x 100 grid)
    float D = 1;
    float dx = 1;
    float dt = pow(dx, 2) / (2 * D);
    int T = 10000;            // number of time steps
    float duration = T * dt; // in seconds

    // Output the initializations
    cout << "--------------" << endl;
    cout << "Total Diffusion Duration: " << duration << " seconds: " << endl;
    cout << "D = " << D << endl;
    cout << "dx = " << dx << endl;
    cout << "dt = " << dt << endl;
    cout << "--------------" << endl;

    // storage containers
    vector<Particle> distribution(N);        // array of 2000 particles
    vector<pair<float, float>> entropies;    // Entropy Sum over N snap shots
    InitDistribute_Corners(distribution, M); // initialize distribution

    //// COMMENCE THE DIFFUSION ////
    // pertinent variables
    float t = 0; // current snapshot
    float P = 0; // probability for a given partition
    float finalEAvg = 0;
    float avgCount = 0;
    float S1_0 = 0;          // initial entropy
    float S1_inf = log(100); // analytical asymptotal entropy limit

    ofstream f("../Data/diffuse.csv");
    f << "x,"
      << "y"
      << ","
      << "t" << endl;

    ofstream f2("../Data/entropy.csv");
    f2 << "t,"
       << "e" << endl;
    srand((unsigned)time(NULL)); // called only once - set the random seed.
    for (int n = 0; n < T; n++)
    {
        t = n * dt;
        // partition counts, intitialized to 0
        vector<vector<int>> partitionCount(10, vector<int>(10, 0));
        // update for current t snaphshot
        for (int i = 0; i < distribution.size(); i++)
        {
            // add randomly signed dx and dy to x and y of particle: distribution[i] by calling function
            randDisplace(distribution[i], dx, M);
            float timeStorageInterval = 1;
            //storeState(f, distribution[i], t, timeStorageInterval);
            int xPos = distribution[i].x;
            int yPos = distribution[i].y;
            //  add Particle Partition Count
            int partitionDivisor = 10;
            if (M == 200)
            {
               partitionDivisor = 20;
            }
            int xIndex = xPos / partitionDivisor;
            int yIndex = yPos / partitionDivisor;
            partitionCount[xIndex][yIndex] += 1; // add to particle count at specific coords on 10x10 grid
        }
        // snapshot complete, partitions done, Now calculate entropy summation
        // funciton below also stores entropy time pair into entropies array
        calcEntropy(f2, partitionCount, entropies, n, T, t, N, finalEAvg, avgCount, S1_0);
    }
    finalEAvg /= avgCount;
    cout << "Entropy at t=0 is: " << S1_0 << endl;
    cout << "Average Entropy at t = 0.95*(" << t << ") is: " << finalEAvg << endl;
    //////////////////////////////////////////////////////////////////////////////////////
    ////// CHARACTERIZING THE RELAXATION OF THE ENTROPY TO EQUILIBRIUM //////
    cout << "Analytical asymptotal entropy limit = " << S1_inf << endl;
    // calculate R(t) by using the entopy values used in the entropies array
    cout << "Calculating Relaxation of Entropy..." << endl;
    ofstream f3("../Data/relax100LIN.csv");
    f3 << "t,"
       << "R" << endl;
    ofstream f4("../Data/relaxlog100LIN.csv");
    f4 << "t,"
       << "ln(R)" << endl;
    calcRelax(f3, f4, entropies, S1_inf);
    f3.close();
    f4.close();
    return 0;
}

//// SUB ROUTINE DEFINITIONS ////
void InitDistribute_Center(vector<Particle> &array)
{
    cout << "Distributing Particles in the Center..." << endl;
    // All particles start out in a small square in the center:
    for (int i = 0; i < array.size(); i++)
    {
        array[i] = Particle(50, 50);
    }
    cout << "Particles Successfully Distrubuted at the Center of Grid!" << endl;
}

void InitDistribute_Corners(vector<Particle> &array, int M)
{ // Particle locations - numbers correspond to particle
    float highBound = M - 1;
    float lowBound = 1;

    cout << "Distributing Particles at the Corners..." << endl;
    // top left: 0-499
    for (int i = 0; i < 500; i++)
    {
        array[i] = Particle(lowBound, highBound);
    }
    // top right: 500, 999
    for (int i = 500; i < 1000; i++)
    {
        array[i] = Particle(highBound, highBound);
    }
    // bottom left: 1000-1499
    for (int i = 1000; i < 1500; i++)
    {
        array[i] = Particle(lowBound, lowBound);
    }
    // bottom right: 1500-1999
    for (int i = 1500; i < 2000; i++)
    {
        array[i] = Particle(highBound, lowBound);
    }
    cout << "Particles Successfully Distrubuted at the Corners of the Grid!" << endl;
}

void randDisplace(Particle &particle, float dx, float bound)
{
    float xDisplace = 0;
    float yDisplace = 0;
    // Random x - RAND_MAX is the largest random number that can be generated by rand()
    float sign1 = (float)rand() / RAND_MAX;
    if (sign1 <= 0.5)
    {
        xDisplace = -1 * dx;
    }
    else
    {
        xDisplace = dx;
    }

    // random y
    float sign2 = (float)rand() / RAND_MAX;
    if (sign2 <= 0.5)
    {
        yDisplace = -1 * dx;
    }
    else
    {
        yDisplace = dx;
    }

    // append to particle
    particle.x += xDisplace;
    particle.y += yDisplace;

    // Out of Bounds check, if out, revert to original position.
    if (particle.x >= bound)
    {
        particle.x -= dx;
    }
    if (particle.x <= 0)
    {
        particle.x += dx;
    }
    if (particle.y >= bound)
    {
        particle.y -= dx;
    }
    if (particle.y <= 0)
    {
        particle.y += dx;
    }
}

void calcRelax(ofstream &f3, ofstream &f4, vector<pair<float, float>> &entropies, float S1_inf)
{
    for (int i = 120; i < entropies.size(); i++)
    {
        float t = entropies[i].first;
        if (t == 300)
           break;
        float dS = entropies[i].second - S1_inf;
        float R = abs(dS);
        f3 << t << "," << R << endl;
        f4 << t << "," << log(R) << endl;
    }
}

void calcEntropy(ofstream &f2, vector<vector<int>> &partitionCount, vector<pair<float, float>> &entropies,
                 int n, int T, float t, int N, float &finalEAvg, float &avgCount, float &S1_0)
{
    float entropy = 0;
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            float Pi = partitionCount[i][j] / (float)N;
            if (Pi == 0) // if Pi=0 then entropy is 0 so skip since we add nothing
            {
                continue;
            }
            entropy += Pi * log(Pi);
        }
    }
    entropy *= -1; // as per the equation

    // store initial entropy and average of the last few entropies
    if (t == 0)
    {
        S1_0 = entropy;
    }
    if (n >= 0.95 * T)
    {
        finalEAvg += entropy;
        avgCount++;
    }
    // store on file for plotting
    f2 << t << "," << entropy << endl;
    // store to array for use later
    pair<float, float> ETpair = {t, entropy};
    entropies.push_back(ETpair);
}

void storeState(ofstream &f, Particle particle, float t, float timeStorageInterval)
{
    // Despite the interval, always store first 100 seconds
    if (t <= 100)
    {
        f << particle.x << "," << particle.y << "," << t << endl;
    }
    if ((fmod(t, timeStorageInterval) == 0) && (t > 100))
    {
        f << particle.x << "," << particle.y << "," << t << endl;
    }
}
