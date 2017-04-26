/* mcm.h -- Minimum cost matrix. Version in C.
 * Author: Luis Colorado <luiscoloradourcola@gmail.com>
 * Date: Tue Apr 18 12:46:54 CEST 2017
 */

#include "mcm.h"
#include <cstdlib>

int main()
{
    MCM::mc_matrix<int> mat(8);

    srand(time(0)); // intialize random number generator.

    for (int row = 0; row < mat.dim; row++)
        for (int col = 0; col < mat.dim; col++)
            mat[row][col] =
                random() % 10000;
                //!row || !col ? 1 : mat[row-1][col] + mat[row][col-1];

    //std::cout << mat << std::endl; // print original matrix.

    const MCM::mc_node<int>* res; // results.
    int n = 1;
    do {
        res = mat.get_mcm(); // get one approx
        n++;
        //std::cout << "CANDIDATE: " << *res << std::endl;
    } while (res && res->col != mat.dim - 1);

    // mat.getRoot()->print(std::cout); // print the whole tree.

    std::cout << mat << std::endl; // print the matrix again.
    std::cout << "SOLUTION: " << *res << std::endl; // the solution
    std::cout << "REVISED: " << n << " positions\n";
    std::cout << "FRONTIER: " << mat.frontier.size() << " nodes\n";

    return 0;
}
