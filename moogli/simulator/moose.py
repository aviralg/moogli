from __future__ import absolute_import
import moose
import moogli
import operator
import PyQt4

def _compartment_parameters(moose_compartment):
    distal_radius = moose_compartment.diameter / 2.0
    distal = [ moose_compartment.x * 10000000
             , moose_compartment.y * 10000000
             , moose_compartment.z * 10000000
             , distal_radius       * 10000000
             ]
    try:
        parent_compartment = moose_compartment.neighbors["raxial"][0][0]
        proximal_radius  = parent_compartment.diameter / 2.0
        proximal = [ moose_compartment.x0 * 10000000
                   , moose_compartment.y0 * 10000000
                   , moose_compartment.z0 * 10000000
                   , proximal_radius      * 10000000
                   ]
        center = [ (proximal[0] + distal[0]) / 2.0
                 , (proximal[1] + distal[1]) / 2.0
                 , (proximal[2] + distal[2]) / 2.0
                 , distal[3]
                 ]
    except IndexError:
        center   = distal
        proximal = None

    return (proximal, distal, center)


def read_network(name = "", path = "", track_parent = True):
    network = moogli.Network(name)
    compartments = moose.wildcardFind(path + "/##[ISA=CompartmentBase]")
    neuron_ids = set( map( lambda compartment : "" if compartment.parent.path == "/" else compartment.parent.path
                         , compartments
                         )
                    )
    for neuron_id in neuron_ids:
        neuron = moogli.Neuron(neuron_id)
        soma_center = None
        moose_compartments = moose.wildcardFind(neuron_id + "/#[ISA=CompartmentBase]")
        for moose_compartment in moose_compartments:
            compartment = moogli.Compartment(moose_compartment.path)
            (proximal, distal, center) = _compartment_parameters(moose_compartment)
            if proximal is None : soma_center = distal
            compartment.add_geometry(distal, proximal)
            neuron.add_compartment(compartment)
        neuron.add_geometry(soma_center)

        # neuron.show_geometry(1, True)
        network.add_neuron(neuron)

    meshes = moose.wildcardFind(path + "/##[ISA=NeuroMesh]")
    for mesh in meshes:
        for i in range(len(mesh.elecComptList)):
            moose_compartment = mesh.elecComptList[i][0]
            neuron      = network.get_neuron(moose_compartment.parent.path)
            compartment = neuron.get_compartment(moose_compartment.path)
            voxel_count = mesh.endVoxelInCompt[i] - mesh.startVoxelInCompt[i]
            (proximal, distal, center) = _compartment_parameters(moose_compartment)
            # print(proximal, distal, center)
            total_height    = map(operator.sub, distal, proximal)
            previous_distal = proximal
            for j in range(0, voxel_count):
                voxel_proximal = previous_distal
                temp = [ (j + 1) * dim / voxel_count
                         for dim in total_height
                       ]
                previous_distal = voxel_distal  = map(operator.add, proximal, temp)
                # print(voxel_proximal, voxel_distal)
                voxel = moogli.Voxel(moose_compartment.path + "/" + str(j + mesh.startVoxelInCompt[i]))
                voxel.set_geometry(voxel_distal, voxel_proximal)
                compartment.add_voxel(voxel)

            compartment.show_geometry(0, True)
    return network

def view(path="", callbacks = [(id, id, id)]):
    application = PyQt4.QtGui.QApplication([])
    viewers = []
    for (prelude, interlude, postlude) in callbacks:
        network = read_network(path)
        viewer = moogli.NetworkViewer( network
                                     , prelude
                                     , interlude
                                     , postlude
                                     )
        viewer.show()
        viewers.append(viewer)

# def delete_gl_widget():
#     global vm_visualizer
#     global rm_visualizer
#     vm_visualizer.setParent(None)
#     del vm_visualizer
#     rm_visualizer.setParent(None)
#     del rm_visualizer

# QApplication.instance().aboutToQuit.connect( delete_gl_widget )

    # print viewers
    # map(lambda viewer : viewer.show(), viewers)
    return application.exec_()
