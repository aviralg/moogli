from moogli.core import NetworkViewer
from moogli.extensions.swc import SwcNetwork
from PyQt4.QtGui import QApplication
from matplotlib.cm import gnuplot
import sys

app = QApplication(sys.argv)
network = SwcNetwork("VHC-neuron.CNG.swc")
viewer = NetworkViewer(network)
viewer.set_background_color((0.0, 0.0, 0.0, 1.0))
viewer.create_color_bar("color-bar-1",
                        "Vertical Color Bar",
                        -0.08,
                        0.05,
                        [(1.0, 1.0, 1.0, 0.5), (1.0, 1.0, 0.0, 0.5), (1.0, 0.0, 0.0, 0.5)],
                        5,
                        4,
                        3.141 / 2,
                        0.02,
                        0.8,
                        (1400.0, 100.0, 0.0),
                        (1.0, 1.0, 1.0, 1.0),
                        "/usr/share/fonts/truetype/liberation/LiberationMono-Italic.ttf",
                        (100, 100),
                        0.03)
viewer.show_color_bar("color-bar-1", 0)

viewer.create_color_bar("color-bar-2",
                        "Horizontal Color Bar",
                        0.0,
                        10.0,
                        [(0.5, 0.5, 0.5, 1.0), (1.0, 0.0, 1.0, 1.0), (1.0, 0.0, 0.0, 1.0)],
                        1000,
                        11,
                        0.0,
                        0.01,
                        1.0,
                        (100.0, 850.0, 0.0),
                        (0.0, 1.0, 1.0, 1.0),
                        "/usr/share/fonts/truetype/ubuntu-font-family/UbuntuMono-B.ttf",
                        (40, 40),
                        0.05)
viewer.show_color_bar("color-bar-2", 0)

viewer.create_color_bar("color-bar-3",
                        "Vertical Color Bar",
                        3,
                        10,
                        [(0.7, 0.6, 0.2, 0.5), (0.0, 0.0, 1.0, 1.0), (1.0, 0.0, 1.0, 0.5)],
                        3,
                        6,
                        3.141 * 1.5,
                        0.02,
                        0.5,
                        (5.0, 500.0, 0.0),
                        (0.0, 1.0, 0.0, 1.0),
                        "/usr/share/fonts/truetype/source-serif-pro/SourceSerifPro-Regular.ttf",
                        (100, 100),
                        0.06)
viewer.show_color_bar("color-bar-3", 0)


viewer.show()
app.exec_()
