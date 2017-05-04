/* mcm.h -- Minimum cost matrix. Version in C.
 * Author: Luis Colorado <luiscoloradourcola@gmail.com>
 * Date: Tue Apr 18 12:46:54 CEST 2017
 */

#include <cstdlib>
#include <inttypes.h>
#include <getopt.h>
#include <sys/time.h>
#include "mcm.h"

int dim = 9;

int main(int argc, char **argv)
{
    int opt;
    char *p = NULL;
    struct timeval now;

    gettimeofday(&now, 0);
    int seed = now.tv_sec ^ now.tv_usec;

    while((opt = getopt(argc, argv, "n:s:")) != -1) {
        switch(opt) {
        case 'n': dim = strtol(optarg, &p, 10); break;
        case 's': seed = strtol(optarg, &p, 10); break;
        } /* switch */
    } /* while */

    MCM::mc_matrix<int> mat(dim);

    std::cout << "SEED: " << seed << std::endl;
    srandom(seed); // intialize random number generator.

    for (int row = 0; row < mat.dim; row++)
        for (int col = 0; col < mat.dim; col++)
            mat[row][col] =
                //1;
                random() % 90 + 10;
                //!row || !col ? 1 : mat[row-1][col] + mat[row][col-1];

    mat.reset();
    //std::cout << mat << std::endl; // print original matrix.

    const MCM::mc_node<int> *res, *res2; // results.
    int n = 0, nsol = 0;
    while ((res = mat.get_mcm()) && !mat.getSol()) {// get one approx
        ++n;
        //std::cout << "CANDIDATE1(" << n << "): " << *res << std::endl;
    }

    if (res) {
        ++n;
        std::cout << "SOLUTION(" << n << "): " << *res << std::endl; // the solution
        std::cout << mat << std::endl; // print the matrix again.
        std::cout << "REVISED: " << n << " positions\n";
        std::cout << "FRONTIER: " << mat.frontier.size() << " nodes\n";
        std::cout << "COST: " << res->cost << std::endl;
        nsol++;
    }

    while ((res2 = mat.get_mcm()) && (res2->cost <= res->cost)) {
        ++n;
        if (res2->isSol()) {
            std::cout << "SOLUTION(" << n << "): " << *res2 << std::endl; // the solution
            std::cout << mat << std::endl; // print the matrix again.
            std::cout << "REVISED: " << n << " positions\n";
            std::cout << "FRONTIER: " << mat.frontier.size() << " nodes\n";
            std::cout << "COST: " << res->cost << std::endl;
            nsol++;
        } else {
            //std::cout << "CANDIDATE2(" << n << "): " << *res2 << std::endl;
        }
    }

    if (res2) {
        n++; // one more revised position.

    }
    std::cout << "\n";
    std::cout << "REVISED: " << n << " positions\n";
    std::cout << "FRONTIER: " << mat.frontier.size() << " nodes\n";
    std::cout << "FOUND: " << nsol << " solutions\n";

    //mat.getRoot()->print(std::cout); // print the whole tree.
    return 0;
}
