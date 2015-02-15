import sys
from PyQt4 import QtCore, QtGui, Qt, QtOpenGL
from PyQt4.QtGui import QApplication
from PyQt4.QtGui import *
from PyQt4.QtCore import *
from PyQt4.Qt import *
import moogli
import math
import collections
print "Hello"
print moogli.__path__

# soma = moogli.shapes.CatmullRom("soma", moogli.geometry.Vec3f(10.0, 0.0, 2.0), 2.0)
# soma_sphere = moogli.shapes.Sphere("Soma sphere", moogli.geometry.Vec3f(10.0, 0.0, 2.0), 2.0, 20, moogli.colors.GREEN)
# axon_segment_1 = moogli.shapes.CatmullRom("axon_segment_1", moogli.geometry.Vec3f(0.0, 0.0, 10.0), 1.0)

soma = moogli.shapes.CatmullRom("soma", moogli.geometry.Vec3f(0.0, 0.0, 0.0), 2.0)
soma_sphere = moogli.shapes.Sphere("Soma sphere", moogli.geometry.Vec3f(0.0, 0.0, 0.0), 2.0, 20, moogli.colors.GREEN)
axon_segment_1 = moogli.shapes.CatmullRom("axon_segment_1", moogli.geometry.Vec3f(20.0, 20.0, 20.0), 0.25)
axon_segment_2 = moogli.shapes.CatmullRom("axon_segment_2", moogli.geometry.Vec3f(10.0, 0.0, 0.0), 0.50)
axon_segment_3 = moogli.shapes.CatmullRom("axon_segment_3", moogli.geometry.Vec3f(0.0, 10.0, 0.0), 0.75)
soma._interpolate_root_node_to_leaf_node(axon_segment_1, 20, 10)
soma._interpolate_root_node_to_leaf_node(axon_segment_2, 20, 10)
soma._interpolate_root_node_to_leaf_node(axon_segment_3, 20, 10)

app = QApplication(sys.argv)
#
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
print soma.get_color()
soma.set_color(moogli.colors.RED)
axon_segment_1.set_color(moogli.colors.BLUE)
axon_segment_2.set_color(moogli.colors.BLUE)
axon_segment_3.set_color(moogli.colors.BLUE)
viewer = ABCD(100) # moogli.core._moogli.Viewer()
viewer.set_background_color(moogli.geometry.Vec4f(1.0, 1.0, 1.0, 1.0))
viewer.attach_shape(soma)
viewer.attach_shape(axon_segment_1)
viewer.attach_shape(axon_segment_2)
viewer.attach_shape(axon_segment_3)
viewer.attach_shape(soma_sphere)
viewer.update()
viewer.show()
viewer.updateGL()
# sphere = Sphere("another")
# sphere.set_color(Vector4(1.0, 1.0, 0.0, 1.0))
# print sphere.set_vertices(100)
# viewer.add_view(view)

# layout = QtGui.QGridLayout(viewer)
# viewer.setLayout(layout)
# layout.addWidget(view1, 0, 0)
# layout.addWidget(view2, 0, 1)
# layout.addWidget(view3, 1, 0)
# # #view1.setParent(viewer)
# # view2.setParent(viewer)
# # view3.setParent(viewer)
# # #view1.show()
# # view2.show()
# # view3.show()
# viewer.show()
# viewer.update()
# view1.show()
# view2.show()
# view3.show()
app.exec_()

