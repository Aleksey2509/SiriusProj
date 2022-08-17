#include "segmentTree.hpp"

constexpr int arrsize = 8;
int main()
{
    double a[arrsize] = {0};
    a[3] = 2;

    //double* tree = segmentTreeCtor(a, arrsize);
    // printf ("tree[1025] - %d, [1026] - %d\n", tree[arrsize + 3], tree[arrsize + 6]);

    a[3] -= 2;
    a[6] += 1;

    // update(tree, 3, a[3], arrsize);
    // update(tree, 6, a[6], arrsize);

    // printf ("[1024] - %lf, tree[1025] - %lf, [1026] - %lf,"
    //  "sizeof long - %zu, sizeof long double - %zu, sizeof double - %zu", tree[arrsize], tree[arrsize + 3], tree[arrsize + 6], sizeof(long long), sizeof(long double), sizeof(double));

    printf ("sizeof long - %zu, sizeof long double - %zu, sizeof double - %zu, long double* - %zu", sizeof(long long), sizeof(long double), sizeof(double), sizeof(long double*));
}