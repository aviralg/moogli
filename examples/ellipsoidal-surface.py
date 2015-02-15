#!/usr/bin/env python

import sys

from PyQt4.QtGui import QApplication

import moogli
import math

app = QApplication(sys.argv)

ellipsoidal_surface = moogli.shapes.EllipsoidalSurface("surface")
ellipsoidal_surface.set_vertices(100)
# ellipsoidal_surface.enable_specular_highlights_effect()
#ellipsoidal_surface.enable_wireframe_effect()
ellipsoidal_surface.enable_outline_effect()
#ellipsoidal_surface.enable_cartoon_effect()
ellipsoidal_surface.set_theta(0.0, 0.0)
ellipsoidal_surface.set_phi(0.0, 0.0)
ellipsoidal_surface.set_abc(5, 5, 5)
ellipsoidal_surface.set_color(moogli.colors.RED)

delta_theta = math.pi / 40.0
delta_phi = math.pi / 20.0

def interlude(view):
    theta_max = ellipsoidal_surface.get_theta_max()
    phi_max = ellipsoidal_surface.get_phi_max()
    if theta_max >= math.pi:
        theta_max = 0.0
    else:
        theta_max += delta_theta
    ellipsoidal_surface.set_theta_max(theta_max)
    if phi_max >= 2 * math.pi:
        phi_max = 0.0
    else:
        phi_max += delta_phi
    ellipsoidal_surface.set_phi_max(phi_max)
view = moogli.View("view", interlude=interlude, idletime=100)
view.set_position(moogli.geometry.Vec2f(0.0, 0.0))
view.set_size(moogli.geometry.Vec2f(0.5, 1.0))
view2 = moogli.View("view", interlude=interlude, idletime=100)
view2.set_position(moogli.geometry.Vec2f(0.5, 0.0))
view2.set_size(moogli.geometry.Vec2f(0.5, 1.0))
viewer = moogli.Viewer("viewer")
viewer.attach_shape(ellipsoidal_surface)
viewer.attach_view(view2)
viewer.attach_view(view)
viewer.show()
viewer.start()
app.exec_()

