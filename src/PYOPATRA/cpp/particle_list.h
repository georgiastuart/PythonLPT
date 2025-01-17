//
// Created by Georgia Stuart on 5/11/21.
//

#ifndef PYOPATRA_PARTICLE_LIST_H
#define PYOPATRA_PARTICLE_LIST_H

#include "list.h"
#include "particle.h"
#include "util.h"

template <int dim>
class ParticleList{
public:
    using ParticleN = ParticleBase<dim>;
    using Vector = typename ParticleN::Vector;

    ParticleList() {
        ptr_wrapper.set_pointer(this);
    };
    ~ParticleList() {
        ParticleN* current = get_head();

        while (current) {
            ParticleN* temp = current->get_next();
            delete current;
            current = temp;
        }

        list.reset_head();
        list.reset_tail();
    }

    void create_particle(const Vector& location) {
        auto temp = new ParticleN();
        temp->set_location(location);
        list.push(temp->get_node());
    }

    int get_num_particles() { return list.length; }
    Eigen::MatrixXd get_all_particle_locations() const {
        Eigen::MatrixXd temp = Eigen::MatrixXd::Zero(list.length, dim);
        auto current = list.get_head();
        size_t index = 0;
        while (current) {
            temp.row(index++) = current->owner->get_location();
            current = current->next;
        }
        return temp;
    }

    Eigen::VectorXi get_all_particle_column_indices() const {
        Eigen::VectorXi temp = Eigen::VectorXi::Zero(list.length);
        auto current = list.get_head();
        size_t index = 0;
        while (current) {
            temp(index++) = current->owner->get_last_known_water_column_index();
            current = current->next;
        }
        return temp;
    }

    ParticleN* get_head() const {
        if (list.get_head()) {
            return list.get_head()->owner;
        } else {
            return nullptr;
        }
    }
    ParticleN* get_tail() const {
        if (list.get_tail()) {
            return list.get_tail()->owner;
        } else {
            return nullptr;
        }
    }

    size_t get_length() const {
        return list.length;
    }

    void delete_all_particles() {
        list.length = 0;

        ParticleN* current = get_head();
        ParticleN* prev;

        while (current) {
            prev = current;
            current = prev->get_next();
            delete prev;
        }

        list.reset_head();
        list.reset_tail();
    }

    void increment_length() {
        list.length++;
    }

    void decrement_length() {
        list.length--;
    }

    PointerWrapper<ParticleList<dim>> get_pointer_wrapper() { return ptr_wrapper; }

private:
    List<ParticleN> list;
    PointerWrapper<ParticleList<dim>> ptr_wrapper;
};

using ParticleList2D = ParticleList<2>;
using ParticleList2DPtrWrapper = PointerWrapper<ParticleList2D>;

#endif //PYOPATRA_PARTICLE_LIST_H
