from moogli.core import ElectricalCompartment, Neuron, Network
import moose


class MooseNetwork(Network):
    def __init__(self, network_name="", model_path=""):
        self.visualizables = {}
        Network.__init__(self, network_name)
        compartments = moose.wildcardFind(model_path + "/#[ISA=CompartmentBase]")
        moose_neurons = set(map(lambda c: c.parent, compartments))
        for moose_neuron in moose_neurons:
            neuron = Neuron(moose_neuron.path)
            self.visualizables[moose_neuron.path] = neuron
            moose_compartments = moose.wildcardFind(moose_neuron.path + "/#[ISA=CompartmentBase]")
            for moose_compartment in moose_compartments:
                compartment = ElectricalCompartment(moose_compartment.path)
                compartment.distal = (moose_compartment.x * 1.0e6,
                                      moose_compartment.y * 1.0e6,
                                      moose_compartment.z * 1.0e6,
                                      moose_compartment.diameter * 5.0e5)
                neuron.attach(compartment)
                self.visualizables[moose_compartment.path] = compartment

            for moose_compartment in moose_compartments:
                compartment = neuron[moose_compartment.path]
                try:
                    t = moose_compartment.name.startswith("head")
                    t = t or moose_compartment.name.startswith("shaft")
                    parent_compartment = moose_compartment.neighbors["raxial"][0][0]
                    parent = neuron[parent_compartment.path]
                    compartment.parent = parent
                    if not t:
                        compartment.proximal = parent.distal
                    else:
                        compartment.proximal = (parent.distal[0],
                                                parent.distal[1],
                                                parent.distal[2],
                                                compartment.distal[3])
                    compartment.add_representation()
                    compartment.set_color((1.0, 0.0, 0.0, 1.0))
                except:
                    compartment.parent = None
                    compartment.proximal = compartment.distal
                    compartment.add_representation()
                    compartment.set_color((0.0, 1.0, 00, 1.0))
                compartment.show(1)
            neuron.show(0)
            self.attach(neuron)
