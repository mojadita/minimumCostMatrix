/* mcm.h -- Minimum cost matrix. Version in C.
 * Author: Luis Colorado <luiscoloradourcola@gmail.com>
 * Date: Tue Apr 18 12:46:54 CEST 2017
 */

#include "mcm.h"
#include <cstdlib>

int main()
{
    MCM::mc_matrix<int> mat(8);
    srand(time(0));

    for (int row = 0; row < mat.dim; row++)
        for (int col = 0; col < mat.dim; col++)
            mat[row][col] = random() % 100; //!row || !col ? 1 : mat[row-1][col] + mat[row][col-1];

    std::cout << mat << std::endl;

    const MCM::mc_node<int>* res;
    int n = 1;

    do {
        res = mat.get_mcm();
        n++;
        //std::cout << "candidate: " << *res << std::endl;
    } while (res && res->col != mat.dim - 1);

    mat.getRoot()->print(std::cout);

    std::cout << mat << std::endl;
    std::cout << "SOLUTION: " << *res << std::endl;
    std::cout << "REVISED: " << n << " posiciones\n";
    std::cout << "FRONTIER: " << mat.frontier->size() << std::endl;

    return 0;
}
