import moose

DISTAL          = 0
AVERAGED        = 1
PROXIMAL_DISTAL = 2

def read_network(name = "", path = "", radius = DISTAL):
    network = _moogli.Network(name, 1)
    compartments = moose.wildcardFind(path + "/##[ISA=CompartmentBase]")
    neuron_ids = set( map( lambda compartment : compartment.parent.path
                         , compartments
                         )
                    )
    for neuron_id in neuron_ids:
        neuron = moogli.Neuron(neuron_id)
        moose_compartments = moose.wildcardFind(neuron_id + "/####[ISA=CompartmentBase]")
        for moose_compartment in moose_compartments:
            compartment = moogli.Compartment(moose_compartment.path)
            distal_diameter = moose_compartment.diameter
            try:
                parent_compartment = moose_compartment.neighbors["raxial"][0]
                proximal_diameter  = parent_compartment.diameter
            except IndexError:
                proximal_diameter = distal_diameter

            if   radius == DISTAL          :
                proximal_diameter = distal_diameter
            elif radius == AVERAGED        :
                distal_diameter = proximal_diameter =  ( distal_diameter
                                                       + proximal_diameter
                                                       ) / 2.0
            compartment.add_geometry( [ moose_compartment.x * 10000000
                                      , moose_compartment.y * 10000000
                                      , moose_compartment.z * 10000000
                                      , distal_diameter     * 10000000
                                      ]
                                    , [ moose_compartment.x0 * 10000000
                                      , moose_compartment.y0 * 10000000
                                      , moose_compartment.z0 * 10000000
                                      , proximal_diameter    * 10000000
                                      ]
                                    )
            neuron.add_compartment(compartment)
        network.add_neuron(neuron)
    return network
