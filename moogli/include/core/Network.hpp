#ifndef _NETWORK_HPP_
#define _NETWORK_HPP_

#include "utility/libraries.hpp"
#include "utility/constants.hpp"
#include "core/Neuron.hpp"

class Network
{
public:
    string id;
    osg::ref_ptr<osg::MatrixTransform> node;
    std::vector<Neuron *> neuron_seq;
    std::unordered_map<string, Neuron *> neuron_map;

    Network(const char * id);

    ~Network();

    const char *
    get_id();

    void
    hide();

    void
    show();

    bool
    is_visible();

    unsigned int
    size();

    unsigned int
    add_neuron(Neuron * neuron);

    unsigned int
    remove_neuron(Neuron * neuron);

    Neuron *
    get_neuron(const char * id);

    bool
    set_colors(PyObject * colors);
};

#endif /* _NETWORK_HPP_ */
