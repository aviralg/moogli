#ifndef _NEURON_HPP_
#define _NEURON_HPP_

#include "utility/libraries.hpp"
#include "utility/constants.hpp"
#include "core/Compartment.hpp"

class Network; // forward declaration to avoid cyclic dependency issues

class Neuron
{
public:
    string id;
    Network * network;
    osg::ref_ptr<osg::Switch> node;
    osg::ref_ptr<osg::Group> compartment_group_node;
    std::vector<Compartment *> compartment_seq;
    std::unordered_map<string, Compartment *> compartment_map;

    Neuron(const char * id);

    ~Neuron();

    const char *
    get_id();

    void
    set_network(Network * network);

    Network *
    get_network();

    Compartment *
    get_compartment(const char * id);

    void
    hide();

    void
    show();

    bool
    is_visible();

    unsigned int
    size();

    unsigned int
    add_geometry( PyObject * distal
                , PyObject * proximal = Py_None
                );

    unsigned int
    add_compartment(Compartment * compartment);

    unsigned int
    remove_compartment(Compartment * compartment);

    void
    show_geometry(unsigned int geometry_index, bool hide_others);

    void
    hide_geometry(unsigned int geometry_index);

    void
    show_all_geometries();

    void
    hide_all_geometries();

    void
    set_color(PyObject * color);

    bool
    set_colors(PyObject * colors);
};

#endif /* _NEURON_HPP_ */
