/* mcm.h -- Minimum cost matrix. Version in C.
 * Author: Luis Colorado <luiscoloradourcola@gmail.com>
 * Date: Tue Apr 18 12:46:54 CEST 2017
 */

#ifndef _MCM_H
#define _MCM_H

#include <iostream>

namespace MCM {

template <class T>
class node {
    const MCM::node<T> * const prnt;
    const int col, row;
    const T cost;
    const MCM::node<T> *chldrn;
    int n_chldrn;
public:
    node(const MCM::node<T> *prnt, int col, int row, T cost)
            :prnt(prnt),col(col),row(row),cost(cost) {
        std::cout
            << "node(" << prnt
            << ", row=" << row
            << ", col=" << col
            << ", cost=" << cost
            << ");" << std::endl;
    }

    friend std::ostream& operator<<(std::ostream& out, node<T>& n);
};

template <class T>
class matrix {
    const int dim;
    T **array; 
public:
    matrix(int dim):dim(dim) {
        array = new T*[dim];
        for (int i = 0; i < dim; i++)
            array[i] = new T[dim];
    }
    ~matrix() {
        for (int i = 0; i < dim; i++)
            delete[] array[i];
        delete[] array;
    }
    T get(int row, int col) {
        return array[row][col];
    }
    void set(int row, int col, T val) {
        array[row][col] = val;
    }
    friend std::ostream& operator<<(std::ostream& out, matrix<T>& it);
}; /* matrix */

    template<class T>
    std::ostream& operator<<(std::ostream& out, matrix<T>& it)
    {
        out << "{";
        for (int row = 0; row < it.dim; row++) {
            if (row) out << ",\n ";
            out << "{";
            for (int col = 0; col < it.dim; col++) {
                if (col) out << ", ";
                out << it.array[row][col];
            }
            out << "}";
        }
        out << "}";
        return out;
    }

} /* MCM */

template<class T>
std::ostream& operator<<(std::ostream& out, MCM::matrix<T>& it)
#endif /* _MCM_H */
