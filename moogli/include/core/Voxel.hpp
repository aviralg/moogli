#ifndef _VOXEL_HPP_
#define _VOXEL_HPP_
#include "utility/libraries.hpp"
#include "utility/constants.hpp"

class Compartment; // forward declaration to avoid cyclic dependency issues

class Voxel
{
public:
    string id;
    Compartment * compartment;
    osg::ref_ptr<osg::Geometry> node;

    Voxel(const char * id);

    ~Voxel();

    const char *
    get_id();

    void
    set_compartment(Compartment * compartment);

    Compartment *
    get_compartment();

    void
    hide();

    void
    show();

    bool
    is_visible();

    void
    set_geometry( PyObject * distal
                , PyObject * proximal = Py_None
                , PyObject * parent   = Py_None
                );

    void
    set_color(PyObject * color);
};

#endif /* _VOXEL_HPP_ */
