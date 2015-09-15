#include "core/ElectricalCompartment.hpp"

ElectricalCompartment::ElectricalCompartment(const char * id) : Visualizable(id)
                                                              , chemical_compartment_group_node(new osg::Group())
                                                              , neuron(nullptr)
                                                              , parent(nullptr)
{
    // by default, neuron shows all its compartments, hence true
    node -> addChild(chemical_compartment_group_node.get(), true);
}

ElectricalCompartment::~ElectricalCompartment()
{
    for (auto & chemical_compartment_pair : chemical_compartment_map)
    {
        chemical_compartment_pair.second -> electrical_compartment = nullptr;
    }
}


void
ElectricalCompartment::set_parent(ElectricalCompartment * electrical_compartment)
{
    if(parent != nullptr)
    {
        auto iter = parent -> children.find(electrical_compartment -> id);
        parent -> children.erase(iter);
    }
    parent = electrical_compartment;
    if(parent != nullptr)
    {
        parent -> children[id] = this;
    }
}


ElectricalCompartment *
ElectricalCompartment::get_parent() const
{
    return parent;
}


size_t
ElectricalCompartment::size() const
{
    return chemical_compartment_map.size();
}


bool
ElectricalCompartment::attach(ChemicalCompartment * chemical_compartment)
{
    auto iter = chemical_compartment_map.find(chemical_compartment -> id);
    if(iter != chemical_compartment_map.end()) { return false; }
    chemical_compartment_map[chemical_compartment -> id] = chemical_compartment;
    chemical_compartment_group_node -> addChild(chemical_compartment -> node.get());
    chemical_compartment -> electrical_compartment = this;
    return true;
}

bool
ElectricalCompartment::detach(ChemicalCompartment * chemical_compartment)
{
    auto iter = chemical_compartment_map.find(chemical_compartment -> id);
    if(iter == chemical_compartment_map.end()) { return false; }
    chemical_compartment_map.erase(iter);
    chemical_compartment_group_node -> removeChild(chemical_compartment -> node.get());
    chemical_compartment -> electrical_compartment = nullptr;
    return true;
}

ChemicalCompartment *
ElectricalCompartment::get(const std::string & id) const
{
    auto iter = chemical_compartment_map.find(id);
    if(iter == chemical_compartment_map.end()) { return nullptr; }
    return iter -> second;
}


void
ElectricalCompartment::subdivide(PyObject * ids)
{
    if(proximal == distal) { subdivide_sphere(ids);   }
    else                   { subdivide_cylinder(ids); }
}

void
ElectricalCompartment::subdivide_cylinder(PyObject * ids)
{
    Vec4f electrical_proximal(proximal[0], proximal[1], proximal[2], proximal[3]);
    Vec4f electrical_distal(distal[0], distal[1], distal[2], distal[3]);

    Vec4f direction = electrical_distal - electrical_proximal;
    Vec4f chemical_proximal = electrical_proximal;
    Vec4f chemical_distal;
    uint size = static_cast<uint>(PySequence_Length(ids));
    for(uint i = 0; i < size; ++i)
    {
        chemical_proximal = electrical_proximal + direction * (i + 0.0f) / size;
        chemical_distal = electrical_proximal + direction * (i + 1.0) / size;
        const char * id = PyString_AsString(PySequence_GetItem(ids, i));
        ChemicalCompartment * chemical_compartment = new ChemicalCompartment(id);
        chemical_compartment -> proximal.set( chemical_proximal[0]
                                            , chemical_proximal[1]
                                            , chemical_proximal[2]
                                            , chemical_proximal[3]
                                            );
        chemical_compartment -> distal.set( chemical_distal[0]
                                          , chemical_distal[1]
                                          , chemical_distal[2]
                                          , chemical_distal[3]
                                          );
        chemical_compartment -> add_representation(false);
        attach(chemical_compartment);
    }
}

void
ElectricalCompartment::subdivide_sphere(PyObject * ids)
{

}
