/* mcm.h -- Minimum cost sqmatrix. Version in C++.
 * Author: Luis Colorado <luiscoloradourcola@gmail.com>
 * Date: Tue Apr 18 12:46:54 CEST 2017
 */

#ifndef _MCM_H
#define _MCM_H

#include <iostream>
#include <set>
#include <cassert>

namespace MCM {

    template <typename T>
    class sq_matrix {

    protected:

        T **const array;

    public:

        const int dim;

        sq_matrix(int dim):dim(dim),array(new T*[dim]) {
            //std::cout << this << ": " << __func__ << std::endl;
            for (int i = 0; i < dim; i++)
                array[i] = new T[dim];
        }

        ~sq_matrix() {
            //std::cout << this << ": " << __func__ << std::endl;
            for (int i = 0; i < dim; i++)
                delete[] array[i];
            delete[] array;
        }

        T*const operator[](int n) const {
            return (T*const) array[n];
        }

        virtual void print_element(std::ostream& out, const int row, const int col) const;
    }; /* sq_matrix */

    template <typename T>
    class mc_matrix;

    template <typename T>
    struct mc_node {

    public:

        const mc_matrix<T>&         matrix;
        const mc_node<T>*const      parent;
        const int                   row, col;
        const T                     cost;
        const mc_node<T>**const     children;

        mc_node(const mc_matrix<T>& matrix, const mc_node<T>* parent = 0, const int row = -1):
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
            //std::cout << this << ": " << __func__ << std::endl;
            for(int i = 0; i < matrix.dim; i++)
                if (children[i]) delete children[i];
            delete [] children;

            if (parent) parent->children[row] = 0;

            //std::cout << this << ": " << "deleting " << this;
            //if (parent) std::cout << ", child of " << parent;
            //std::cout << std::endl;
        }

        void print(std::ostream& out) const;

        bool isSol() const { return col == matrix.dim - 1; }
    };

    template <typename T>
    class mc_matrix: public sq_matrix<T> {

        const mc_node<T> *sol;

    public:

        std::set<const mc_node<T>*> frontier;
        const mc_node<T> root;

        mc_matrix(int dim):sq_matrix<T>(dim),sol(0), root(*this) {
            // std::cout << this << ": " << __func__ << "(" << dim << ");\n";
            frontier.insert(&root);
        }

        //~mc_matrix() {
            // std::cout << this << ": " << __func__ << "()\n";
        //}

        void reset();

        const mc_node<T>* get_mcm();

        const mc_node<T>* getRoot() { return &root; }

        const mc_node<T>* getSol() { return sol; }

        void print_element(std::ostream& out, const int row, const int col) const;
    };

} /* MCM */

template <typename T>
void MCM::sq_matrix<T>::print_element(std::ostream& out, const int row, const int col) const
{
    out << array[row][col];
}

template <typename T>
void MCM::mc_matrix<T>::print_element(std::ostream& out, const int row, const int col) const
{
    const MCM::mc_node<T>* p = 0;
    if (sol)
        for (p = sol; p; p = p->parent)
            if (p->row == row && p->col == col)
                break;
    out << (p ? "\033[32m<" : " ");
    out << this->array[row][col];
    out << (p ? ">\033[m" : " ");
}

template <typename T>
void MCM::mc_matrix<T>::reset()
{
    frontier.clear();
    frontier.insert(&root);
    for (int i = 0; i < this->dim; i++) {
        if (root.children[i]) {
            delete root.children[i];
            root.children[i] = 0;
        }
    }
}

template <typename T>
std::ostream& operator<<(std::ostream& out, const MCM::sq_matrix<T>& it)
{
    out << "{";
    for (int row = 0; row < it.dim; row++) {
        if (row) out << ",\n ";
        out << "{";
        for (int col = 0; col < it.dim; col++) {
            if (col) out << ", ";
            it.print_element(out, row, col);
        }
        out << "}";
    }
    out << "}";
    return out;
}

template <typename T>
std::ostream& operator<<(std::ostream& out, const MCM::mc_node<T>& node)
{
    if (node.parent == 0)
        return out
            << ">"
            << (node.matrix.frontier.count(&node) ? "*" : "");
    return out
        << *node.parent 
        << "=> ["
        << node.row
        << ":"
        << node.col
        << "](v="
        << node.matrix[node.row][node.col]
        << ";c="
        << node.cost
        << ")"
        << (node.matrix.frontier.count(&node) ? "*" : "");
}

template <typename T>
void MCM::mc_node<T>::print(std::ostream& out) const
{
    out << *this << std::endl;
    for (int i = 0; i < matrix.dim; i++)
        if (children[i]) children[i]->print(out);
}

template <typename T>
const MCM::mc_node<T>* MCM::mc_matrix<T>::get_mcm()
{
    const MCM::mc_node<T>* new_parent = 0;
    int new_row;
    T new_cost;

    //std::cout << this << ": " << __func__ << "(): BEGIN\n";

    for(typename std::set<const MCM::mc_node<T>*>::iterator it = frontier.begin(); it != frontier.end(); it++) {
        const MCM::mc_node<T>* parent_candidate = *it;

        //std::cout << "Processing " << *parent_candidate << std::endl;
        int candidates = 0;
        for(int row = 0; row < this->dim; row++) {
            const MCM::mc_node<T>* n;
            for(n = parent_candidate; n->parent; n = n->parent)
                if (n->row == row) // row coincides with some ancestor's
                    break;
            if (n->parent // we broke the loop (row found in ancestors chain)
                    || parent_candidate->children[row]) // row is the same as children[row] (already visited)
                continue;
            candidates++;
            int col = parent_candidate->col+1;
            T cost = parent_candidate->cost + this->array[row][col];
            if (!new_parent || cost < new_cost) {
                // we found a candidate.  Create it.
                new_parent = parent_candidate;
                new_row = row;
                new_cost = cost;
            }
        }
        if (!candidates) {
            //std::cout << "FRONTIER: ERASING " << *parent_candidate << std::endl;
            frontier.erase(parent_candidate);
        }
    }
    if (new_parent) {
        const MCM::mc_node<T>* result = new MCM::mc_node<T>(*this, new_parent, new_row);
        if (!result->isSol()) frontier.insert(result);
        sol = result->isSol() ? result : (const MCM::mc_node<T>*) 0;
        return result;
    }
    return 0;
}

#endif /* _MCM_H */
