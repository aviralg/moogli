#include "core/Neuron.hpp"

Neuron::Neuron(const char * id) : id(id)
                                , node(new osg::Switch())
                                , compartment_group_node(new osg::Group())
                                , network(nullptr)
{
    // by default, neuron shows all its compartments, hence true
    node -> addChild(compartment_group_node.get(), true);
}

Compartment *
Neuron::get_compartment(const char * id)
{
    auto iter = compartment_map.find(id);
    if(iter == compartment_map.end())
    {
        RECORD_WARNING("Compartment does not exist!");
        // TODO : raise exception instead of returning nullptr
        return nullptr;
    }
    return iter -> second;
}

Neuron::~Neuron()
{
    for(Compartment * compartment : compartment_seq)
    {
        compartment -> set_neuron(nullptr);
    }
}

const char *
Neuron::get_id()
{
    return id.c_str();
}


void
Neuron::set_network(Network * network)
{
    this -> network = network;
}

Network *
Neuron::get_network()
{
    return network;
}

void
Neuron::hide()
{
    node -> setNodeMask(NODE_HIDE_MASK);
}

void
Neuron::show()
{
    node -> setNodeMask(NODE_SHOW_MASK);
}

bool
Neuron::is_visible()
{
    return (node -> getNodeMask() == NODE_SHOW_MASK ? true : false);
}

unsigned int
Neuron::size()
{
    return static_cast<unsigned int>(compartment_seq.size());
}


unsigned int
Neuron::add_geometry( PyObject * distal
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
              , SPHERICAL_NEURON_POINTS
              , SPHERICAL_NEURON_COLOR
              );
    }
    else
    {
        Vec4f p(pysequence_to_vec4f(proximal));
        cylinder( d
                , p
                , geometry
                , CYLINDRICAL_NEURON_POINTS
                , CYLINDRICAL_NEURON_COLOR
                , pysequence_to_vec3f(parent)
                );
    }
    geode -> addDrawable(geometry);
    node -> addChild(geode);
    return node -> getNumChildren();
}

unsigned int
Neuron::add_compartment(Compartment * compartment)
{
    auto iter = compartment_map.find(compartment -> get_id());
    if(iter != compartment_map.end())
    {
        RECORD_WARNING("Compartment already exists!");
    }
    else
    {
        compartment_map[compartment -> get_id()] = compartment;
        compartment_seq.push_back(compartment);
        compartment_group_node -> addChild(compartment -> node.get());
        compartment -> set_neuron(this);

    }
    return static_cast<unsigned int>(compartment_seq.size());
}

unsigned int
Neuron::remove_compartment(Compartment * compartment)
{
    auto iter = compartment_map.find(compartment -> get_id());
    if(iter == compartment_map.end())
    {
        RECORD_WARNING("Compartment does not exist!");
    }
    else
    {
        compartment_map.erase(iter);
        compartment_seq.erase( std::find( compartment_seq.begin()
                                        , compartment_seq.end()
                                        , compartment
                                        )
                             );
        compartment_group_node -> removeChild(compartment -> node.get());
        compartment -> set_neuron(nullptr);
    }
    return static_cast<unsigned int>(compartment_seq.size());
}


void
Neuron::show_geometry(unsigned int geometry_index, bool hide_others)
{
    if(hide_others) node -> setSingleChildOn(geometry_index);
    else            node -> setValue(geometry_index, true);

}

void
Neuron::hide_geometry(unsigned int geometry_index)
{
    node -> setValue(geometry_index, false);
}

void
Neuron::show_all_geometries()
{
    node -> setAllChildrenOn();
}

void
Neuron::hide_all_geometries()
{
    node -> setAllChildrenOff();
}

void
Neuron::set_color(PyObject * color)
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

bool
Neuron::set_colors(PyObject * colors)
{
    if(PySequence_Check(colors) != 1)
    {
        RECORD_ERROR("Invalid data structure provided for setting compartment colors.");
        Py_RETURN_FALSE;
    }

    unsigned int limit = std::min( static_cast<unsigned int>(PySequence_Length(colors))
                                 , static_cast<unsigned int>(compartment_seq.size())
                                 );
    unsigned int i;
    for(i = 0; i < limit;++i)
    {
        compartment_seq[i] -> set_color(PySequence_GetItem(colors, i));
    }
    Py_RETURN_TRUE;
}
