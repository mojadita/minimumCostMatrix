/* mcm.h -- Minimum cost matrix. Version in C.
 * Author: Luis Colorado <luiscoloradourcola@gmail.com>
 * Date: Tue Apr 18 12:46:54 CEST 2017
 */

#include "mcm.h"

int main()
{
    MCM::sq_matrix<double> mat(10);
    for (int row = 0; row < mat.dim; row++)
        for (int col = 0; col < mat.dim; col++)
            mat[row][col] = !row || !col ? 1 : mat[row-1][col] + mat[row][col-1];
    std::cout << mat << std::endl;
}
