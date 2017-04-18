/* mcm.h -- Minimum cost matrix. Version in C.
 * Author: Luis Colorado <luiscoloradourcola@gmail.com>
 * Date: Tue Apr 18 12:46:54 CEST 2017
 */

#include "mcm.h"

int main()
{
    MCM::matrix<double> mat(10);
    std::cout << mat;
    std::cout << std::endl;
}
