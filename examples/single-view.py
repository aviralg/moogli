#!/usr/bin/env python

import sys

from PyQt4.QtGui import QApplication

from moogli.core import View
from moogli.core import Viewer
from moogli.core import *
from moogli.core.geometry import *

import math

app = QApplication(sys.argv)

view = View()
view.set_position(Vec2f(0.0, 0.0))
view.set_size(Vec2f(1.0, 1.0))
view.set_background_color(Vec4f(0.0, 0.0, 0.0, 1.0))
cb = ColorBar("cb1")
cb.set_size(Vec2f(0.25, 0.05))
cb.set_orientation(math.pi / 2.0)
cb.set_position(Vec2f(1.0 - 0.03, 0.5))
cb.set_text_font("/usr/share/fonts/truetype/ubuntu-font-family/UbuntuMono-R.ttf")
cb.set_text_character_size(15.0)
view.attach_color_bar(cb)
print "Attached"

viewer = Viewer()
frustum = Frustum("aviral")
frustum.set_vertices(4)
frustum.set_radii(2.0, 5.0)
frustum.set_length(5.0)
frustum.set_apex_color(Vec4f(1.0, 1.0, 0.0, 0.5))
frustums = frustum.subdivide(10)
sphere = Sphere("another")
sphere.set_color(Vec4f(1.0, 1.0, 0.0, 1.0))
print sphere.set_vertices(10)
sphere.set_radius(5.1)
sphere.set_center(Vec3f(20.0, 0.0, 0.0))
viewer.add_view(view)
viewer.add_mesh(sphere)
for frustum in frustums:
    viewer.add_mesh(frustum)
viewer.show()
app.exec_()

viewer.remove_view(view)
app.exec_()
