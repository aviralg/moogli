#ifndef _COMPARTMENT_HPP_
#define _COMPARTMENT_HPP_

#include "utility/libraries.hpp"
#include "utility/constants.hpp"
#include "core/Voxel.hpp"

class Neuron; // forward declaration to avoid cyclic dependency issues

class Compartment
{
public:
    string id;
    Neuron * neuron;
    osg::ref_ptr<osg::Switch> node;
    osg::ref_ptr<osg::Geode> voxel_group_node;
    std::vector<Voxel *> voxel_seq;
    std::unordered_map<string, Voxel *> voxel_map;

    Compartment(const char * id);

    ~Compartment();

    const char *
    get_id();

    void
    set_neuron(Neuron * Neuron);

    Neuron *
    get_neuron();

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
    add_voxel(Voxel * voxel);

    unsigned int
    remove_voxel(Voxel * voxel);

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

#endif /* _COMPARTMENT_HPP_ */
