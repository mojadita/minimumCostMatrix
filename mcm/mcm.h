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

        mc_node(sq_matrix<T>& matrix,
                mc_node<T> * const parent,
                int row):

                matrix(matrix),
                parent(parent),
                row(row),
                col(parent
                        ? parent->col+1
                        : -1),
                cost(parent
                        ? parent->cost + matrix[row][col]
                        : T()),
                children(new mc_node<T>*[matrix.dim])
        {
            std::cout << this << ": " << __func__ << "(matrix, " << parent << ", " << row << ");\n";
            for (int i = 0; i < matrix.dim; i++)
                children[i] = 0;
            std::cout << this << ": creating " << matrix.dim << " children\n";
            if (parent) {
                assert(parent->children[row] == 0);
                std::cout << this << ": adding this to parent's children\n";
                parent->children[row] = this;
            }
        }

        ~mc_node() {
            for(int i = 0; i < matrix.dim; i++)
                if (children[i]) delete children[i];
            delete [] children;

            std::cout << "deleting " << this;
            if (parent) std::cout << ", child of " << parent;
            std::cout << std::endl;
        }
        void print(std::ostream& out, const int indent = 0);
    };

    template <typename T>
    class mc_matrix: public sq_matrix<T> {

        std::vector<mc_node<T>*> frontier;
        mc_node<T> * root;

    public:


        mc_matrix(int dim):sq_matrix<T>(dim) {
            std::cout << this << ": " << __func__ << "(" << dim << ");\n";
            frontier.push_back(root = new mc_node<T>(*this, 0, -1));
        }

        ~mc_matrix() { delete root; }

        void reset() {
            frontier.clear();
            delete root;
            frontier.push_back(root = new mc_node<T>(*this, 0, -1));
        }

        mc_node<T> *const getRoot() { return root; }

    };

} /* MCM */

template <typename T>
std::ostream& operator<<(std::ostream& out, const MCM::sq_matrix<T>& it)
{
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

template <typename T>
std::ostream& operator<<(std::ostream& out, const MCM::mc_node<T>& node)
{
    if (node.parent == 0) return out << "@";
    return out << *node.parent << "=>" << node.row << "(v=" << node.matrix[node.row][node.col] <<";c=" << node.cost << ")";
}

template <typename T>
void MCM::mc_node<T>::print(std::ostream& out, const int indent)
{
    for (int i = 0; i < indent; i++) out << " ";
    out << *this << std::endl;
    for (int i = 0; i < matrix.dim; i++)
        if (children[i]) children[i]->print(out, indent + 1);
}

#endif /* _MCM_H */
