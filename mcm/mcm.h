/* mcm.h -- Minimum cost sqmatrix. Version in C++.
 * Author: Luis Colorado <luiscoloradourcola@gmail.com>
 * Date: Tue Apr 18 12:46:54 CEST 2017
 */

#ifndef _MCM_H
#define _MCM_H

#include <iostream>
#include <vector>
#include <cassert>

namespace MCM {

    template <typename T>
    class sq_matrix {

        T **const array; 

    public:

        const int dim;

        sq_matrix(int dim):dim(dim),array(new T*[dim]) {
            for (int i = 0; i < dim; i++)
                array[i] = new T[dim];
        }

        ~sq_matrix() {
            for (int i = 0; i < dim; i++)
                delete[] array[i];
            delete[] array;
        }

        T*const operator[](int n) const
        {
            return (T*const) array[n];
        }
    }; /* sq_matrix */


    template <typename T>
    struct mc_node {

    public:

        sq_matrix<T>&       matrix;
        mc_node<T>*const    parent;
        const int           row, col;
        const T             cost;
        mc_node<T>**const   children;

        mc_node(sq_matrix<T>& matrix, const mc_node<T> *parent, int row, int col):
                matrix(matrix),
                parent(parent),
                row(row), col(col),
                cost(parent
                        ? parent->cost + matrix[row][col]
                        : T()),
                children(new mc_node*[matrix.dim])
        {
            for (int i = 0; i < matrix.dim; i++)
                children[i] = 0;
            if (parent) {
                assert(parent->children[row] == 0);
                parent->children[row] = this;
            }
        }

        ~mc_node() {
            std::cout << "deleting " << this << ", child of " << parent;
            for(int i = 0; i < matrix.dim; i++)
                if (children[i]) delete children[i];
            delete children;
        }
    };

    template <typename T>
    class mc_matrix: public sq_matrix<T> {

        std::vector<mc_node<T> > frontier;

    public:

        mc_node<T> * const root;

        mc_matrix(int dim):sq_matrix<T>(dim),root(0) {}

        ~mc_matrix() { if (root) delete root; }
    };

} /* MCM */

template <typename T>
std::ostream& operator<<(std::ostream& out, MCM::sq_matrix<T>& it) {
    out << "{";
    for (int row = 0; row < it.dim; row++) {
        if (row) out << ",\n ";
        out << "{";
        for (int col = 0; col < it.dim; col++) {
            if (col) out << ", ";
            out << it[row][col];
        }
        out << "}";
    }
    out << "}";
    return out;
}
#endif /* _MCM_H */
