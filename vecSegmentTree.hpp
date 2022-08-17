#include <vector>
#include <iostream>
#include <numeric>


void buildSubTree(std::vector<double>& segmentTree, std::vector<double>& values, int current, int left, int right);
void update(std::vector<double>& segmentTree, int index, double value, int maxSize);
int find(std::vector<double>& segmentTree, double randomNum, int maxSize);
void segmentTreeDtor(double* segmentTree);


    // buildSubTree(segmentTree, values, 1, 0, size - 1);



void buildSubTree(std::vector<double>& segmentTree, std::vector<double>& values, int current, int left, int right)
    {
        if (left == right)
        {
            segmentTree[current] = values[right];
        } else
        {
            int mid = (left + right) / 2;
            buildSubTree(segmentTree, values, current * 2, left, mid);
            buildSubTree(segmentTree, values, current * 2 + 1, mid + 1, right);
            segmentTree[current] = segmentTree[current * 2] + segmentTree[current * 2 + 1];
        }
    }

inline void update(std::vector<double>& segmentTree, int index, double value, int maxSize)
{
    int i = index + maxSize;
    segmentTree[i] = value;

    while (i > 1)
    {
        segmentTree[i / 2] = segmentTree [i] + segmentTree [i ^ 1];
        i /= 2;
    }
}

inline int find(std::vector<double>& segmentTree, double randomNum, int maxSize)
{
    int i = 1;
    while (i >= 1)
    {
        i *= 2;
        if (segmentTree [i] < randomNum)
        {
            randomNum -= segmentTree[i];
            i += 1;
        }
        if (i >= maxSize)
            break;
    }

    return i - maxSize;
}


inline void segmentTreeDtor(double* segmentTree)
{
    free(segmentTree);
}

void printSegmentTree(double* tree, int maxSize)
{
    for (int i = 0; i < maxSize * 4 + 4; i++)
        std::cout << tree[i] << " ";
    std::cout << std::endl;
}