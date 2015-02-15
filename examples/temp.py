#!/usr/bin/env python

import sys
from PyQt4 import QtCore, QtGui, Qt, QtOpenGL
from PyQt4.QtGui import QApplication
import moogli
import math

app = QApplication(sys.argv)

viewer = moogli.CompositeViewer()
frustum = moogli.shapes.Frustum("aviral")
frustum.set_vertices(4)
frustum.set_color(moogli.geometry.Vec4f(1.0, 1.0, 0.0, 0.5))
# frustum.set_radii(5.0 / math.sqrt(2.0), 5.0 / math.sqrt(2.0))
# frustum.set_length(105.0)
# frustum.set_apex_color(moogli.geometry.Vec4f(1.0, 1.0, 0.0, 0.5))


# view1 = moogli.CompositeView(viewer)
# view1.set_background_color(moogli.geometry.Vec4f(1.0, 0.0, 0.0, 1.0))
# view1.attach_shape(frustum)

# view2 = moogli.CompositeView(viewer)
# view2.set_background_color(moogli.geometry.Vec4f(0.0, 1.0, 0.0, 1.0))
# view2.attach_shape(frustum)

# view3 = moogli.CompositeView(viewer)
# view3.set_background_color(moogli.geometry.Vec4f(0.0, 0.0, 1.0, 1.0))
# view3.attach_shape(frustum)


format = QtOpenGL.QGLFormat()
format.setVersion(2, 0) # get expected output with (3,1) and below, else blank window
format.setProfile(QtOpenGL.QGLFormat.CoreProfile)
format.setSampleBuffers(True)
print QtOpenGL.QGLFormat.openGLVersionFlags()
print format
class ABCD(moogli.core._moogli.Viewer):
    def __init__(self, format):
        super(ABCD, self).__init__(format)
        self.setMouseTracking(True)
        self.timer = QtCore.QTimer()
        self.timer.setInterval(0)
        self.timer.timeout.connect(self.updateGL)
        self.timer.start()

   # def set_camera_fovy()
   # def set_camera_clipping_planes()
    # def event(self, event):
    #     self.updateGL()
    #     return super(ABCD, self).event(event)

viewer = ABCD(format) # moogli.core._moogli.Viewer()
viewer.set_background_color(moogli.geometry.Vec4f(0.0, 0.0, 0.0, 1.0))
viewer.attach_shape(frustum)
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
