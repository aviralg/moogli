#!/usr/bin/env python

import sys

from PyQt4.QtGui import QApplication

from moogli.core import View
from moogli.core import Viewer
from moogli.core import Vector2
from moogli.core import Vector4

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
view3.set_size(Vector2(0.5, 0.5))
view3.set_background_color(Vector4(0.0, 0.0, 1.0, 1.0))

view4 = View()
view4.set_position(Vector2(0.5, 0.5))
view4.set_size(Vector2(0.5, 0.5))
view4.set_background_color(Vector4(1.0, 1.0, 1.0, 1.0))

view5 = View()
view5.set_position(Vector2(0.25, 0.25))
view5.set_size(Vector2(0.5, 0.5))
view5.set_background_color(Vector4(0.0, 0.0, 0.0, 1.0))

viewer = Viewer()
viewer.add_view(view1)
viewer.add_view(view2)
viewer.add_view(view3)
viewer.add_view(view4)
viewer.add_view(view5)
viewer.show()
app.exec_()
