import moogli
import moose
from moose import neuroml
from PyQt4 import Qt, QtCore, QtGui
import sys
import os

app = QtGui.QApplication(sys.argv)
filename = os.path.join( os.path.split(os.path.realpath(__file__))[0]
                       , "/PurkinjeCellPassivePulseInput/PurkinjePassive.net.xml"
                       )

moose.neuroml.loadNeuroML_L123(filename)
network_distal = moogli.simulator.moose.read_network( name = ""
                                                    , path = "/cells[0]"
                                                    , radius = moogli.DISTAL
                                                    )
viewer_distal = moogli.NetworkViewer(network_distal)
viewer_distal.setWindowTitle("Distal Radius")

network_averaged = moogli.simulator.moose.read_network( name = ""
                                                      , path = "/cells[0]"
                                                      , radius = moogli.AVERAGED
                                                      )
viewer_averaged = moogli.NetworkViewer(network_averaged)
viewer_averaged.setWindowTitle("Averaged Radius")

network_proximal_distal = moogli.simulator.moose.read_network( name = ""
                                                             , path = "/cells[0]"
                                                             , radius = moogli.PROXIMAL_DISTAL
                                                             )
viewer_proximal_distal = moogli.NetworkViewer(network_proximal_distal)
viewer_proximal_distal.setWindowTitle("Continuous Variation in Radius")

viewer_distal.show()
viewer_averaged.show()
viewer_proximal_distal.show()
app.exec_()
