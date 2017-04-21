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

    protected:

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

        const sq_matrix<T>&         matrix;
        const mc_node<T>*const      parent;
        const int                   row, col;
        const T                     cost;
        const mc_node<T>**const     children;

        mc_node(const sq_matrix<T>& matrix, const mc_node<T>* parent = 0, const int row = -1):
                matrix(matrix), parent(parent), row(row),
                col(parent
                        ? parent->col+1
                        : -1),
                cost(parent
                        ? parent->cost + matrix[row][col]
                        : T()),
                children(new const mc_node<T>*[matrix.dim])
        {
            //std::cout << this << ": " << __func__ << "(matrix, " << parent << ", " << row << ");\n";
            for (int i = 0; i < matrix.dim; i++)
                children[i] = 0;
            //std::cout << this << ": creating [" << matrix.dim << "] children\n";
            if (parent) {
                assert(parent->children[row] == 0);
                //std::cout << this << ": adding 'this' to parent's children\n";
                parent->children[row] = this;
            }
        }

        ~mc_node() {
            for(int i = 0; i < matrix.dim; i++)
                if (children[i]) delete children[i];
            delete [] children;

            if (parent) parent->children[row] = 0;

            //std::cout << "deleting " << this;
            //if (parent) std::cout << ", child of " << parent;
            //std::cout << std::endl;
        }
        void print(std::ostream& out, const int indent = 0) const;
    };

    template <typename T>
    class mc_matrix: public sq_matrix<T> {

    public:
        std::vector<const mc_node<T>*>* frontier;
        const mc_node<T> root;


        mc_matrix(int dim):sq_matrix<T>(dim),frontier(new std::vector<const mc_node<T>*>()), root(*this) {
            //std::cout << this << ": " << __func__ << "(" << dim << ");\n";
            frontier->push_back(&root);
        }

        ~mc_matrix() {
            delete frontier;
        }

        void reset() {
            for (int i = 0; i < this->dim; i++) {
                if (root.children[i]) {
                    delete root.children[i];
                    root.children[i] = 0;
                }
            }
            frontier->clear();
            frontier->push_back(&root);
        }

        const mc_node<T>* get_mcm();

        const mc_node<T>* getRoot() { return &root; }

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
    if (node.parent == 0) return out << "ROOT(c=0)";
    return out << *node.parent << "=> [" << node.row << "](v=" << node.matrix[node.row][node.col] <<";c=" << node.cost << ")";
}

template <typename T>
void MCM::mc_node<T>::print(std::ostream& out, const int indent) const
{
    for (int i = 0; i < indent; i++) out << " ";
    out << *this << std::endl;
    for (int i = 0; i < matrix.dim; i++)
        if (children[i]) children[i]->print(out, indent + 1);
}

template <typename T>
const MCM::mc_node<T>* MCM::mc_matrix<T>::get_mcm()
{
    std::vector<const MCM::mc_node<T>*>* new_frontier = new std::vector<const MCM::mc_node<T>*>();
    const MCM::mc_node<T>* new_parent = 0;
    int new_row;
    T new_cost;

    //std::cout << this << ": " << __func__ << "(): BEGIN\n";

    for(typename std::vector<const MCM::mc_node<T>*>::iterator it = frontier->begin(); it != frontier->end(); it++) {
        const MCM::mc_node<T>* candidate_parent = *it;

        //std::cout << "Processing " << *candidate_parent << std::endl;
        int candidates = 0;
        for(int row = 0; row < this->dim; row++) {
            const MCM::mc_node<T>* n;
            for(n = candidate_parent; n->parent; n = n->parent)
                if (n->row == row) // row coincides with some ancestor's
                    break;
            if (n->parent // we broke the loop (row found in ancestors chain)
                    || candidate_parent->children[row]) // row is the same as children[row] (already visited)
                continue;
            candidates++;
            int col = candidate_parent->col+1;
            T cost = candidate_parent->cost + this->array[row][col];
            if (!new_parent || cost < new_cost) {
                // we found a candidate.  Create it.
                new_parent = candidate_parent;
                new_row = row;
                new_cost = cost;
            }
        }
        if (candidates)
            new_frontier->push_back(candidate_parent);
    }
    delete frontier;
    frontier = new_frontier;
    if (new_parent) {
        const MCM::mc_node<T>* result = new MCM::mc_node<T>(*this, new_parent, new_row);
        frontier->push_back(result);
        return result;
    }
    return 0;
}

#endif /* _MCM_H */
