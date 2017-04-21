/* mcm.h -- Minimum cost matrix. Version in C.
 * Author: Luis Colorado <luiscoloradourcola@gmail.com>
 * Date: Tue Apr 18 12:46:54 CEST 2017
 */

#include "mcm.h"

int main()
{
    MCM::mc_matrix<double> mat(15);
    for (int row = 0; row < mat.dim; row++)
        for (int col = 0; col < mat.dim; col++)
            mat[row][col] = !row || !col ? 1 : mat[row-1][col] + mat[row][col-1];
    std::cout << mat << std::endl;
    MCM::mc_node<double>* n1 = new MCM::mc_node<double>(mat, mat.getRoot(), 3);
    MCM::mc_node<double>* n2 = new MCM::mc_node<double>(mat, n1, 2);
    std::cout << "n1 = " << *n1 << std::endl;
    std::cout << "n2 = " << *n2 << std::endl;
    std::cout << "TREE:\n";
    mat.getRoot()->print(std::cout);
    mat.reset();
    mat.getRoot()->print(std::cout);
}
