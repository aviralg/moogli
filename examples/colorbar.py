from moogli.core import NetworkViewer
from moogli.extensions.swc import SwcNetwork
from PyQt4.QtGui import QApplication
from matplotlib.cm import gnuplot
import sys

app = QApplication(sys.argv)
network = SwcNetwork("VHC-neuron.CNG.swc")
network.write("VHC-neuron.3ds")
viewer = NetworkViewer(network)
viewer.set_background_color((0.0, 0.0, 0.0, 1.0))
viewer.create_color_bar("color-bar-1",
                        "Vertical Color Bar",
                        (3.0, 10.0),
                        [(1.0, 1.0, 1.0, 0.5), (1.0, 1.0, 0.0, 0.5), (1.0, 0.0, 0.0, 1.0)],
                        5,
                        4,
                        3.14 / 4.0,
                        (500.0, 100.0),
                        (600.0, 400.0, 0.0),
                        (1.0, 1.0, 1.0, 1.0),
                        "/usr/share/fonts/truetype/liberation/LiberationMono-Italic.ttf",
                        (100, 100),
                        20.0)
viewer.show_color_bar("color-bar-1", 0)

viewer.create_color_bar("color-bar-2",
                        "Horizontal Color Bar",
                        (0.0, 10.0),
                        [(0.5, 0.5, 0.5, 1.0), (1.0, 0.0, 1.0, 1.0), (1.0, 0.0, 0.0, 1.0)],
                        1000,
                        11,
                        0.0,
                        (200.0, 50.0),
                        (0.0, 300.0, 0.0),
                        (0.0, 1.0, 1.0, 1.0),
                        "/usr/share/fonts/truetype/ubuntu-font-family/UbuntuMono-B.ttf",
                        (40, 40),
                        15.0)
viewer.show_color_bar("color-bar-2", 0)

viewer.create_color_bar("color-bar-3",
                        "Vertical Color Bar",
                        (3.0, 10.0),
                        [(0.7, 0.6, 0.2, 0.5), (0.0, 0.0, 1.0, 1.0), (1.0, 0.0, 1.0, 0.5)],
                        3,
                        4,
                        3.141  / 2.0,
                        (500.0, 100.0),
                        (600.0, 400.0, 0.0),
                        (0.0, 1.0, 0.0, 1.0),

                        "/usr/share/fonts/truetype/liberation/LiberationMono-Italic.ttf",
                        # "/usr/share/fonts/truetype/source-serif-pro/SourceSerifPro-Regular.ttf",
                        (100, 100),
                        20.0)
# viewer.show_color_bar("color-bar-3", 0)
# # viewer.capture("main", "./captures/view", "bmp", 100)
# viewer.capture("commoner", "./captures/view", "png", 100)
viewer.show()
app.exec_()
