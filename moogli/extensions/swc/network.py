import numpy
import os

from moogli.core import ElectricalCompartment, Neuron, Network


class SwcNetwork(Network):
    def __init__(self, filename):
        self.visualizables = {}
        network_name = neuron_name = os.path.splitext(
            os.path.basename(filename))[0]
        Network.__init__(self, network_name)
        dtype = numpy.dtype([('cid', numpy.int),
                             ('ctype', numpy.int),
                             ('cx', numpy.float64),
                             ('cy', numpy.float64),
                             ('cz', numpy.float64),
                             ('cr', numpy.float64),
                             ('cpid', numpy.int)])
        swc_data = numpy.genfromtxt(filename, dtype=dtype)
        neuron = Neuron(neuron_name)
        self.visualizables[neuron_name] = neuron
        print swc_data
        for (cid, ctype, cx, cy, cz, cr, cpid) in swc_data:
            # if cid > 200 : break
            cid = str(cid)
            compartment = ElectricalCompartment(cid)
            compartment.distal = (cx, cy, cz, cr)
            neuron.attach(compartment)
            self.visualizables[cid] = compartment

        for (cid, ctype, cx, cy, cz, cr, cpid) in swc_data:
            # if cid > 200 : break
            cid = str(cid)
            cpid = str(cpid)
            try:
                neuron[cid].parent = neuron[cpid]
                neuron[cid].proximal = neuron[cpid].distal
                neuron[cid].add_representation()
                neuron[cid].set_color((1.0, 1.0, 0.0, 1.0))

            except KeyError:
                neuron[cid].parent = None
                neuron[cid].proximal = neuron[cid].distal
                # neuron[cid].add_representation()
                # neuron[cid].set_color((0.0, 1.0, 0.0, 1.0))

            neuron[cid].show(1)

        neuron.show(0)
        self.attach(neuron)


def read(filename):
    ROOT = 0
    ROOT_CHILD = 1
    INTERNAL = 2
    LEAF = 3

    dtype = numpy.dtype([('cid', numpy.int),
                         ('ctype', numpy.int),
                         ('cx', numpy.float64),
                         ('cy', numpy.float64),
                         ('cz', numpy.float64),
                         ('cr', numpy.float64),
                         ('cpid', numpy.int)])

    swc_data = numpy.genfromtxt(filename, dtype=dtype)

    tree = {cid: {"type": LEAF,
                  "distal": moogli.geometry.Vec3f(cx, cy, cz),
                  "radius": cr,
                  "parent": cpid,
                  "id": cid,
                  "children": []}
            for (cid, ctype, cx, cy, cz, cr, cpid) in swc_data}
    group = moogli.Group("catmull_rom")
    root = None
    for cid in tree:
        try:
            cpid = tree[cid]["parent"]
            tree[cpid]["children"].append(cid)
            tree[cpid]["type"] = INTERNAL
        except KeyError, e:
            tree[cid]["type"] = ROOT
            root = cid

    for cid in tree[root]["children"]:
        tree[cid]["type"] = ROOT_CHILD

    for content in tree.values():
        group.attach_shapes([moogli.shapes.CatmullRom(content["id"],
                                                      content["distal"],
                                                      content["radius"])
                             for content in tree.values()])
    parent = None
    grandparent = None
    stack = [root]
    while len(stack) > 0:
        element = stack.pop()
        data = tree[element]
        stack.extend(content["children"])
        shape = group.shapes[data["id"]]
        if data["type"] == ROOT:
            shape._interpolate_root_node()  # generates a sphere
            for child_id in content["children"]:
                child_data = tree[child_id]
                child = group.shapes[child_id]
                if len(child_data["children"]) == 1:
                    grandchild = group.shapes[child_data["children"][0]]
                    shape._interpolate_root_node_to_internal_node(child,
                                                                  grandchild)
                else:
                    shape._interpolate_root_node_to_leaf_node(child)
            return

        parent_id = data["parent"]
        parent = group.shapes[parent_id]

        if data["type"] == LEAF:
            shape._interpolate_leaf_node(parent)

        elif data["type"] == ROOT_CHILD:
            for child_id in data["children"]:
                child_data = tree[child_id]
                child = group.shapes[child_id]
                if len(child_data["children"]) == 1:
                    grandchild = group.shapes[child_data["children"][0]]
                    shape._interpolate_root_child_node_to_internal_node(parent,
                                                                        child,
                                                                        grandchild)
                else:
                    shape._interpolate_root_child_node_to_leaf_node(parent,
                                                                    child)

        # grandparent_id = tree[parent_id]["parent"]
        # grandparent = group.shapes[grandparent_id]
        else:
            for child_id in data["children"]:
                child_data = tree[child_id]
                child = group.shapes[child_id]
                if len(child_data["children"]) == 1:
                    grandchild = group.shapes[child_data["children"][0]]
                    shape._interpolate_internal_node_to_internal_node(parent,
                                                                      child,
                                                                      grandchild)
                else:
                    shape._interpolate_internal_node_to_leaf_node(parent,
                                                                  child)
