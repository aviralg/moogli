#include "core/Network.hpp"

Network::Network(const char * id) : id(id)
                                  , node(new osg::MatrixTransform())
{
}


Network::~Network()
{
    for(Neuron * neuron : neuron_seq)
    {
        neuron -> set_network(nullptr);
    }
}

const char *
Network::get_id()
{
    return id.c_str();
}

void
Network::hide()
{
    node -> setNodeMask(NODE_HIDE_MASK);
}

void
Network::show()
{
    node -> setNodeMask(NODE_SHOW_MASK);
}

bool
Network::is_visible()
{
    return (node -> getNodeMask() == NODE_SHOW_MASK ? true : false);
}

unsigned int
Network::size()
{
    return static_cast<unsigned int>(neuron_seq.size());
}

unsigned int
Network::add_neuron(Neuron * neuron)
{
    auto iter = neuron_map.find(neuron -> get_id());
    if(iter != neuron_map.end())
    {
        RECORD_WARNING("Neuron already exists!");
    }
    else
    {
        neuron_map[neuron -> get_id()] = neuron;
        neuron_seq.push_back(neuron);
        neuron -> set_network(this);
        node -> addChild(neuron -> node.get());
    }
    return static_cast<unsigned int>(neuron_seq.size());
}

Neuron *
Network::get_neuron(const char * id)
{
    auto iter = neuron_map.find(id);
    if(iter == neuron_map.end())
    {
        RECORD_WARNING("Neuron does not exist!");
        // TODO : raise exception instead of returning nullptr
        return nullptr;
    }
    return iter -> second;
}


unsigned int
Network::remove_neuron(Neuron * neuron)
{
    auto iter = neuron_map.find(neuron -> get_id());
    if(iter == neuron_map.end())
    {
        RECORD_WARNING("Neuron does not exist!");
    }
    else
    {
        neuron_map.erase(iter);
        neuron_seq.erase( std::find( neuron_seq.begin()
                                   , neuron_seq.end()
                                   , neuron
                                   )
                             );
        node -> removeChild(neuron -> node.get());
        neuron -> set_network(nullptr);
    }
    return static_cast<unsigned int>(neuron_seq.size());
}

bool
Network::set_colors(PyObject * colors)
{
    if(PySequence_Check(colors) != 1)
    {
        RECORD_ERROR("Invalid data structure provided for setting neuron colors.");
        Py_RETURN_FALSE;
    }

    unsigned int limit = std::min( static_cast<unsigned int>(PySequence_Length(colors))
                                 , static_cast<unsigned int>(neuron_seq.size())
                                 );
    unsigned int i;
    for(i = 0; i < limit;++i)
    {
        neuron_seq[i] -> set_color(PySequence_GetItem(colors, i));
    }
    Py_RETURN_TRUE;
}
