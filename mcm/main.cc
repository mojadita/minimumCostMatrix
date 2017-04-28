/* mcm.h -- Minimum cost matrix. Version in C.
 * Author: Luis Colorado <luiscoloradourcola@gmail.com>
 * Date: Tue Apr 18 12:46:54 CEST 2017
 */

#include "mcm.h"
#include <cstdlib>

int main()
{
    MCM::mc_matrix<int> mat(10);
    std::cout.width(2);

    srand(time(0)); // intialize random number generator.

    for (int row = 0; row < mat.dim; row++)
        for (int col = 0; col < mat.dim; col++)
            mat[row][col] =
                //1;
                random() % 90 + 10;
                //!row || !col ? 1 : mat[row-1][col] + mat[row][col-1];

    //std::cout << mat << std::endl; // print original matrix.

    const MCM::mc_node<int> *res, *res2; // results.
    int n = 0, nsol = 0;
    while ((res = mat.get_mcm()) && !mat.getSol()) {// get one approx
        n++;
        //std::cout << "CANDIDATE(" << n << "): " << *res << std::endl;
    }

    std::cout << "SOLUTION: " << *res << std::endl; // the solution
    std::cout << "REVISED: " << n << " positions\n";
    std::cout << "FRONTIER: " << mat.frontier.size() << " nodes\n";
    std::cout << "COST: " << res->cost << std::endl;
    std::cout << mat << std::endl; // print the matrix again.
    nsol++;

    while ((res2 = mat.get_mcm()) && res2->cost <= res->cost) {
        n++;
        //std::cout << "CANDIDATE(" << n << "): " << *res2 << std::endl;
        if (res2->isSol()) {
            std::cout << "SOLUTION: " << *res2 << std::endl; // the solution
            std::cout << "REVISED: " << n << " positions\n";
            std::cout << "FRONTIER: " << mat.frontier.size() << " nodes\n";
            std::cout << "COST: " << res->cost << std::endl;
            std::cout << mat << std::endl; // print the matrix again.
            nsol++;
        }
    }

    std::cout << "REVISED: " << n << " positions\n";
    std::cout << "FRONTIER: " << mat.frontier.size() << " nodes\n";
    std::cout << "ENCOUNTERED: " << nsol << " solutions\n";

    // mat.getRoot()->print(std::cout); // print the whole tree.
    return 0;
}
