#include <ctime>
#include <cstdlib>
#include <cmath>
#include <utility>
#include <random>
#include <algorithm>
#include <chrono>
#include <iostream>
#include <fstream>
#include "unistd.h"

#include "segmentTree.hpp"


constexpr int arrSize = 1024 * 1024;

inline double updateU_I (double* uVec, int index, int numOfClasters)
{
    return uVec[index] = numOfClasters * 2 * std::pow(index, 0.98); 

    // return uVec[index] = numOfClasters * index;
}

inline double updateV_I (double* vVec, int index, int numOfClasters)
{
    return vVec[index] = numOfClasters;

    // return vVec[index] = numOfClasters * index;
}

long* initSizes()
{
    long* sizes = (long*)calloc(arrSize + 1, sizeof(long));
    sizes[1] = arrSize;

    return sizes;
}

double* inituVec()
{
    double* uVec = (double*)calloc(arrSize + 1, sizeof(double));
    uVec[1] = arrSize * 2;

    return uVec;
}

double* initvVec()
{
    double* vVec = (double*)calloc(arrSize + 1, sizeof(double));
    vVec[1] = arrSize;

    return vVec;
}

int main()

{
    long* sizes = initSizes();
    double* uVec = inituVec();
    double* vVec = initvVec();

    long i, j;
    i = j = -1;
    long currentMaxSize = 1;
    double maxTime = 250;
    double doubleTime = 0.0;
    double currentTime = 0.0;
    double lambda = 0.005;
    // double kernelMax = kernel(currentMaxSize, currentMaxSize);
    long volume = arrSize;
    long N = arrSize;
    long maxSize = N;

    srand(time(NULL));

    double* uTree = segmentTreeCtor(uVec, arrSize);
    double* vTree = segmentTreeCtor(vVec, arrSize);

    std::ofstream output;
    output.open("oscilation.txt");
    if (!output)
        throw 1;

    double delta = maxTime / 250;
    double ifToGetPoint = delta;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> floatDist(-1, 0);

    auto start = std::chrono::high_resolution_clock::now();
    while (currentTime < maxTime)
    {
        // printf("time = %lf\n", currentTime);
        do
        {

            currentTime += 2.0 * volume / (uTree[1] * vTree[1]);
            ifToGetPoint -= 2.0 * volume / (uTree[1] * vTree[1]);

            if (ifToGetPoint < 0)
            {
                printf("outing a point; time = %lf\n", currentTime);
                double sum = 0.0;
                output << currentTime << " ";
                for (int it = 1; it < maxSize; it++)
                    sum += it * it * (sizes[it] + 0.0) / volume;

                output << (currentMaxSize / sum) << std::endl;

                ifToGetPoint = delta;
            }

        
            i = find(uTree, uTree [1] * -floatDist(gen), maxSize); // bad!!
            j = find(vTree, vTree [1] * -floatDist(gen), maxSize); // bad!!

            // printf("finding i = %d, j = %d\n", i, j);

            if ((i == j) && ((sizes[i] * -floatDist(gen)) <= 1))
            {
                // printf("continuing\n");
                i = j = -1;
                continue;
            }
        } while((i == -1) && (j == -1));

        double whichInteraction = -floatDist(gen);

        if ((whichInteraction >= (lambda / (lambda + 1))) || ((i == j) && (i == 1)))
        {

            if (i + j > currentMaxSize)
                currentMaxSize = i + j;

            if (i + j > maxSize)
            {
                int oldSize = maxSize;
                maxSize *= 2;
                sizes = (long *)realloc(sizes, maxSize * sizeof(long));
                uVec = (double *)realloc(uVec, maxSize * sizeof(double));
                vVec = (double *)realloc(vVec, maxSize * sizeof(double));
                for (int it = 0; it < oldSize; it++)
                {
                    sizes[it + oldSize] = 0;
                    uVec[it + oldSize] = 0;
                    vVec[it + oldSize] = 0;
                }

                segmentTreeDtor(vTree);
                segmentTreeDtor(uTree);
                uTree = segmentTreeCtor(uVec, maxSize);
                vTree = segmentTreeCtor(vVec, maxSize);
                // throw "cry baby";
            }

            sizes[i] -= 1;
            sizes[j] -= 1;
            sizes[i + j] += 1;
            N--;


            update(uTree, i + j, updateU_I(uVec, i + j, sizes[i + j]), maxSize);
            update(vTree, i + j, updateV_I(vVec, i + j, sizes[i + j]), maxSize);

        }
        else
        {
            sizes[1] += i + j - 2;

            if (i != 1)
                sizes[i] = 1;
            if (j != 1)
                sizes[j] = 1;

            N += i + j - 2;

            update(uTree, 1, updateU_I(uVec, 1, sizes[1]), maxSize);
            update(vTree, 1, updateV_I(vVec, 1, sizes[1]), maxSize);

        }

        update(uTree, i, updateU_I(uVec, i, sizes[i]), maxSize);
        update(uTree, j, updateU_I(uVec, j, sizes[j]), maxSize);
        update(vTree, i, updateV_I(vVec, i, sizes[i]), maxSize);
        update(vTree, j, updateV_I(vVec, j, sizes[j]), maxSize);


        if (N <= arrSize / 2)
        {
            doubleTime = currentTime;
            N *= 2;
            for (int it = 0; it < maxSize; it++)
            {
                sizes[it] *= 2;
                uVec[it] *= 2;
                vVec[it] *= 2;

                uTree[it] *= 2;
                uTree[it] *= 2;
                vTree[it + maxSize] *= 2;
                vTree[it + maxSize] *= 2;
            }
            volume *= 2;
        }

        i = j = -1;
    }

    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);

    std::cout << "kmax = " << currentMaxSize << " currentTime = " << currentTime << " volume = " << volume
              << " worked for " << duration.count()  << " double time = " << doubleTime << " Moment_0: " << N << std::endl;

    for (int it = 0; it < 15; it++)
    {
        std::cout << sizes[it] << " ";
    }
    std::cout << std::endl;

    free(uVec);
    free(vVec);

    segmentTreeDtor(uTree);
    segmentTreeDtor(vTree);
    free(sizes);
    output.close();
}