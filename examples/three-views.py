#!/usr/bin/env python

import sys

from PyQt4.QtGui import QApplication

from moogli.core import View
from moogli.core import Viewer
from moogli.core import Vector2
from moogli.core import Vector4
from moogli.core import Frustum
import math

app = QApplication(sys.argv)

view1 = View()
view1.set_position(Vector2(0.0, 0.0))
view1.set_size(Vector2(0.5, 0.5))
view1.set_background_color(Vector4(1.0, 0.0, 0.0, 1.0))

view2 = View()
view2.set_position(Vector2(0.5, 0.0))
view2.set_size(Vector2(0.5, 0.5))
view2.set_background_color(Vector4(0.0, 1.0, 0.0, 1.0))

view3 = View()
view3.set_position(Vector2(0.0, 0.5))
view3.set_size(Vector2(1.0, 0.5))
view3.set_background_color(Vector4(0.0, 0.0, 1.0, 1.0))

frustum = Frustum("aviral")
frustum.set_vertices(4)
frustum.set_radii(5.0 / math.sqrt(2.0), 5.0 / math.sqrt(2.0))
frustum.set_length(5.0)
frustum.set_apex_color(Vector4(1.0, 1.0, 0.0, 0.5))
# sphere = Sphere("another")
# sphere.set_color(Vector4(1.0, 1.0, 0.0, 1.0))
# print sphere.set_vertices(100)
# viewer.add_view(view)
viewer = Viewer()
viewer.add_mesh(frustum)
viewer.add_view(view1)
viewer.add_view(view2)
viewer.add_view(view3)
viewer.show()
app.exec_()
