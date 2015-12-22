import numpy
import moogli
import sys
from PyQt4 import QtCore, QtGui, Qt, QtOpenGL
from PyQt4.QtGui import QApplication
from PyQt4.QtGui import *
from PyQt4.QtCore import *
from PyQt4.Qt import *
import moogli
import math
import collections


def read(filename, axial_segments, radial_segments):
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

    tree = {str(cid): {"type": LEAF,
                       "distal": moogli.geometry.Vec3f(cx, cy, cz),
                       "radius": cr,
                       "parent": str(cpid),
                       "id": str(cid),
                       "children": []}
            for (cid, ctype, cx, cy, cz, cr, cpid) in swc_data}
    group = moogli.Group("catmull-rom")
    root = None
    print "Creating Shapes"
    for cid in tree:
        try:
            data = tree[cid]
            group.attach_shape(moogli.shapes.CatmullRom(data["id"],
                                                        data["distal"],
                                                        data["radius"]))
            cpid = tree[cid]["parent"]
            tree[cpid]["children"].append(cid)
            tree[cpid]["type"] = INTERNAL
        except KeyError, e:
            root = cid

    tree[root]["type"] = ROOT

    for cid in tree[root]["children"]:
        tree[cid]["type"] = ROOT_CHILD

    print "Interpolating Shapes"
    parent = None
    grandparent = None
    stack = [root]
    while len(stack) > 0:
        element = stack.pop()
        data = tree[element]
        stack.extend(data["children"])
        shape = group.shapes[data["id"]]
        if data["type"] == ROOT:
            group.attach_shape(moogli.shapes.Sphere(data["id"] + "-sphere",
                                                    data["distal"],
                                                    data["radius"],
                                                    axial_segments,
                                                    moogli.colors.RED))
            # shape._interpolate_root_node()  # generates a sphere
            for child_id in data["children"]:
                child_data = tree[child_id]
                child = group.shapes[child_id]
                if len(child_data["children"]) == 1:
                    grandchild = group.shapes[child_data["children"][0]]
                    shape._interpolate_root_node_to_internal_node(child,
                                                                  grandchild,
                                                                  axial_segments,
                                                                  radial_segments)
                else:
                    shape._interpolate_root_node_to_leaf_node(child,
                                                              axial_segments,
                                                              radial_segments)
            continue

        parent_id = data["parent"]
        parent = group.shapes[parent_id]

        if data["type"] == LEAF:
            shape._interpolate_leaf_node(parent,
                                         axial_segments,
                                         radial_segments)

        elif data["type"] == ROOT_CHILD:
            for child_id in data["children"]:
                child_data = tree[child_id]
                child = group.shapes[child_id]
                if len(child_data["children"]) == 1:
                    grandchild = group.shapes[child_data["children"][0]]
                    shape._interpolate_root_child_node_to_internal_node(parent,
                                                                        child,
                                                                        grandchild,
                                                                        axial_segments,
                                                                        radial_segments)
                else:
                    shape._interpolate_root_child_node_to_leaf_node(parent,
                                                                    child,
                                                                    axial_segments,
                                                                    radial_segments)

        else:
            grandparent_id = tree[parent_id]["parent"]
            grandparent = group.shapes[grandparent_id]
            for child_id in data["children"]:
                child_data = tree[child_id]
                child = group.shapes[child_id]
                if len(child_data["children"]) == 1:
                    grandchild = group.shapes[child_data["children"][0]]
                    shape._interpolate_internal_node_to_internal_node(grandparent,
                                                                      parent,
                                                                      child,
                                                                      grandchild,
                                                                      axial_segments,
                                                                      radial_segments)
                else:
                    shape._interpolate_internal_node_to_leaf_node(grandparent,
                                                                  parent,
                                                                  child,
                                                                  axial_segments,
                                                                  radial_segments)

    return group



class ABCD(moogli.core._moogli.Viewer):
    def __init__(self, f):
        super(ABCD, self).__init__(QtOpenGL.QGLFormat.defaultFormat())
        self._setup_context_menu()
        self.setMouseTracking(True)
        self.timer = QtCore.QTimer()
        self.timer.setInterval(1000.0 / 59.0)
        self.timer.timeout.connect(self.updateGL)
        self.timer.start()
        self._shapes = collections.OrderedDict()
        # self.connect(self,
        #              SIGNAL("customContextMenuRequested(QPoint)"),
        #              self,
        #              SLOT("show_context_menu(QPoint, shape)"))

    @pyqtSlot(QtCore.QPoint)
    def show_context_menu(self, point):
        self.context_menu.exec_(self.mapToGlobal(point))

    def _setup_context_menu(self):
        self.context_menu = QMenu()
        self.context_menu.addAction("Plot")
        self.context_menu.addAction("Plot")

    # def focus(self, shape):
    #     if not self.contains_shape(shape):
    #         return
    #     bounding_box = shape.bounding_box()
    #     bounding_box.get_x_min()

    def mousePressEvent(self, event):
        print "press"
        self._x = event.x()
        self._y = event.y()
        return super(ABCD, self).mousePressEvent(event)

    def mouseReleaseEvent(self, event):
        print "release"
        handled = super(ABCD, self).mouseReleaseEvent(event)
        if event.x() == self._x and event.y() == self._y:
            intersections = self.intersections(event.x(), event.y())
            if len(intersections) > 0:
                self._shapes[intersections[0][0]].toggle_outline_effect()
                self.shape_menu.exec_(QtGui.QCursor.pos())
            else:
                self.widget_menu.exec_(QtGui.QCursor.pos())
            # print event.x()
            # print event.y()
            # print type(event.x())
            # print super(ABCD, self).intersections(int(event.x()), int(event.y()))
            # x = 2.0 * (event.x()) / (self.width() - 1.0)
            # y = 2.0 * (self.height() - 1.0 - event.y()) / (self.height() - 1.0) - 1.0
            # print super(ABCD, self).intersections(x, y)
        return handled

    def attach_shape(self, shape):
        if shape.id() in self._shapes:
            return
        super(ABCD, self).attach_shape(shape)
        self._shapes[shape.id()] = shape

    # @QtCore.pyqtSlot()
    # def intersections(self):
    #     return self.intersections(self.mapFromGlobal(QtGui.QCursor.pos()))

    # @QtCore.pyqtSlot(QtCore.QPoint)
    # def intersections(self, point):
    #     return self.intersections(point.x(),
    #                               point.y())


    # @QtCore.pyqtSlot(int, int)
    # def intersections(self, x, y):
    #     return map(self._shapes.get,
    #                super(ABCD, self).intersections(x, y))

    # @QtCore.pyqtSlot(float, float)
    # def intersections(self, x, y):
    #     return map(self._shapes.get,
    #                super(ABCD, self).intersections(x, y))


   # def set_camera_fovy()
   # def set_camera_clipping_planes()
    # def event(self, event):
    #     self.updateGL()
    #     return super(ABCD, self).event(event)


def main():
    group = read(sys.argv[1], 20, 10)
    app = QApplication(sys.argv)
#print soma.get_color()

    viewer = ABCD(100) # moogli.core._moogli.Viewer()

    for shape in group.shapes.values():
        shape.set_color(moogli.colors.RED)
        viewer.attach_shape(shape)

    viewer.set_background_color(moogli.geometry.Vec4f(1.0, 1.0, 1.0, 1.0))
    viewer.update()
    viewer.show()
    viewer.updateGL()
    return app.exec_()

if __name__ == "__main__":
    main()
