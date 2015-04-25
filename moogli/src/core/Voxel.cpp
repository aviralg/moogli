#include "core/Voxel.hpp"

Voxel::Voxel(const char * id) : id(id)
                              , node(new osg::Geometry())
                              , compartment(nullptr)
{
    node -> setName(id);
}


Voxel::~Voxel()
{ }

const char *
Voxel::get_id()
{
    return id.c_str();
}

void
Voxel::set_compartment(Compartment * compartment)
{
    this -> compartment = compartment;
}

Compartment *
Voxel::get_compartment()
{
    return compartment;
}

void
Voxel::hide()
{
    // node -> setNodeMask(NODE_HIDE_MASK);
}

void
Voxel::show()
{
    // node -> setNodeMask(NODE_SHOW_MASK);
}

bool
Voxel::is_visible()
{
    return true;
    // return (node -> getNodeMask() == NODE_SHOW_MASK ? true : false);
}

void
Voxel::set_geometry( PyObject * distal
                   , PyObject * proximal
                   , PyObject * parent
                   )
{
    Vec4f d(pysequence_to_vec4f(distal));

    if(proximal == Py_None)
    {
        sphere( osg::Vec3f(d[0], d[1], d[2])
              , d[3]
              , node.get()
              , SPHERICAL_VOXEL_POINTS
              , SPHERICAL_VOXEL_COLOR
              );
    }
    else
    {
        Vec4f p(pysequence_to_vec4f(proximal));
        cylinder( d
                , p
                , node.get()
                , CYLINDRICAL_VOXEL_POINTS
                , CYLINDRICAL_VOXEL_COLOR
                , pysequence_to_vec3f(parent)
                );
    }
}

void
Voxel::set_color(PyObject * color)
{
    Vec4Array * colors = new Vec4Array();
    colors -> push_back(pysequence_to_vec4d(color));
    node -> setColorArray(colors, osg::Array::BIND_OVERALL);
    node -> setColorBinding(osg::Geometry::BIND_OVERALL);
}
