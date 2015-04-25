#include "core/Compartment.hpp"

Compartment::Compartment(const char * id) : id(id)
                                          , node(new osg::Switch())
                                          , voxel_group_node(new osg::Geode())
                                          , neuron(nullptr)
{
    // by default, neuron shows all its compartments, hence true
    node -> addChild(voxel_group_node.get(), true);
}

Compartment::~Compartment()
{
    for(Voxel * voxel : voxel_seq)
    {
        voxel -> set_compartment(nullptr);
    }
}

const char *
Compartment::get_id()
{
    return id.c_str();
}


void
Compartment::set_neuron(Neuron * neuron)
{
    this -> neuron = neuron;
}

Neuron *
Compartment::get_neuron()
{
    return neuron;
}

void
Compartment::hide()
{
    node -> setNodeMask(NODE_HIDE_MASK);
}

void
Compartment::show()
{
    node -> setNodeMask(NODE_SHOW_MASK);
}

bool
Compartment::is_visible()
{
    return (node -> getNodeMask() == NODE_SHOW_MASK ? true : false);
}

unsigned int
Compartment::size()
{
    return static_cast<unsigned int>(voxel_seq.size());
}

unsigned int
Compartment::add_geometry( PyObject * distal
                         , PyObject * proximal
                         , PyObject * parent
                         )
{
    Vec4f d(pysequence_to_vec4f(distal));

    osg::Geometry * geometry = new osg::Geometry();
    geometry -> setName(id);
    osg::Geode    * geode   = new osg::Geode();
    geode -> setName(id);

    if(proximal == Py_None)
    {
        sphere( osg::Vec3f(d[0], d[1], d[2])
              , d[3]
              , geometry
              , SPHERICAL_COMPARTMENT_POINTS
              , SPHERICAL_COMPARTMENT_COLOR
              );
    }
    else
    {
        Vec4f p(pysequence_to_vec4f(proximal));
        cylinder( d
                , p
                , geometry
                , CYLINDRICAL_COMPARTMENT_POINTS
                , CYLINDRICAL_COMPARTMENT_COLOR
                , pysequence_to_vec3f(parent)
                );
    }
    geode -> addDrawable(geometry);
    node -> addChild(geode);
    return node -> getNumChildren();
}

unsigned int
Compartment::add_voxel(Voxel * voxel)
{
    auto iter = voxel_map.find(voxel -> get_id());
    if(iter != voxel_map.end())
    {
        RECORD_WARNING("Voxel already exists!");
    }
    else
    {
        voxel_map[voxel -> get_id()] = voxel;
        voxel_seq.push_back(voxel);
        voxel_group_node -> addDrawable(voxel -> node.get());
        voxel -> set_compartment(this);

    }
    return static_cast<unsigned int>(voxel_seq.size());
}

unsigned int
Compartment::remove_voxel(Voxel * voxel)
{
    auto iter = voxel_map.find(voxel -> get_id());
    if(iter == voxel_map.end())
    {
        RECORD_WARNING("Voxel does not exist!");
    }
    else
    {
        voxel_map.erase(iter);
        voxel_seq.erase( std::find( voxel_seq.begin()
                                  , voxel_seq.end()
                                  , voxel
                                  )
                             );
        voxel_group_node -> removeDrawable(voxel -> node.get());
        voxel -> set_compartment(nullptr);
    }
    return static_cast<unsigned int>(voxel_seq.size());
}


void
Compartment::show_geometry(unsigned int geometry_index, bool hide_others)
{
    if(hide_others) node -> setSingleChildOn(geometry_index);
    else            node -> setValue(geometry_index, true);

}

void
Compartment::hide_geometry(unsigned int geometry_index)
{
    node -> setValue(geometry_index, false);
}

void
Compartment::show_all_geometries()
{
    node -> setAllChildrenOn();
}

void
Compartment::hide_all_geometries()
{
    node -> setAllChildrenOff();
}

bool
Compartment::set_colors(PyObject * colors)
{
    if(PySequence_Check(colors) != 1)
    {
        RECORD_ERROR("Invalid data structure provided for setting voxel colors.");
        Py_RETURN_FALSE;
    }

    unsigned int limit = std::min( static_cast<unsigned int>(PySequence_Length(colors))
                                 , static_cast<unsigned int>(voxel_seq.size())
                                 );
    unsigned int i;
    for(i = 0; i < limit;++i)
    {
        voxel_seq[i] -> set_color(PySequence_GetItem(colors, i));
    }
    Py_RETURN_TRUE;
}

void
Compartment::set_color(PyObject * color)
{
    for(unsigned int i = 1; i < node -> getNumChildren(); ++i)
    {
        Geode * geode = node -> getChild(i) -> asGeode();
        for(unsigned int j = 0; j < geode -> getNumDrawables(); ++j)
        {
            Geometry * geometry = geode -> getDrawable(j) -> asGeometry();
            Vec4Array * colors = new Vec4Array();
            colors -> push_back(pysequence_to_vec4d(color));
            geometry -> setColorArray(colors, osg::Array::BIND_OVERALL);
            geometry -> setColorBinding(osg::Geometry::BIND_OVERALL);
        }
    }
}

// void
// Compartment::create_chemical_compartments( Vec3f & proximal
//                                          , Vec3f & distal
//                                          , unsigned int count
//                                          ,
//                                          )
// {
//     Vec3f direction = distal - proximal;
//     Vec3f height    = direction.normalize();

//     Vec3f current_proximal(proximal);
//     for(uint i = 1; i < count; ++i)
//     {
//         current_distal = i * height * direction;

//         ChemicalCompartment * chemical_compartment = new ChemicalCompartment( current_proximal
//                                                                             , current_distal
//                                                                             );
//         current_proximal = current_distal;
//     }
// }
