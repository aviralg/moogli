import PyQt4
import PyQt4.Qt
import PyQt4.QtGui
import PyQt4.QtCore


import _moogli
from _moogli import Network
from _moogli import Neuron
from _moogli import Compartment
from _moogli import Voxel

class NetworkViewer(_moogli.NetworkViewer):

    def __init__( self
                , network
                , prelude   = id
                , interlude = id
                , postlude  = id
                , idletime  = 0
                ):
        PyQt4.QtGui.QApplication.instance().aboutToQuit.connect(self.stop)
        _moogli.NetworkViewer.__init__(self, network)
        self.network    = network
        self.prelude    = prelude
        self.interlude  = interlude
        self.postlude   = postlude
        self.idletime   = idletime
        self._done      = False
        self._pause     = False
        self.start()

    def start(self):
        self._done = False
        self.prelude(self)
        self.update()
        self.run()

    def pause(self):
        self._pause = True

    def resume(self):
        self._pause = False
        self.run()

    def stop(self):
        self._done = True
        self.postlude(self)

    def run(self):
        self.interlude(self)
        self.update()
        if self._done: return
        if self._pause: return
        PyQt4.QtCore.QTimer.singleShot(self.idletime, self.run)

__all__ = [ "Voxel"
          , "Compartment"
          , "Neuron"
          , "Network"
          , "NetworkViewer"
          , "simulator"
          , "reader"
          ]

# __all__ = [ "Morphology"
#           , "MorphologyViewer"
#           , "MorphologyViewerWidget"
#           , "DynamicMorphologyViewerWidget"
#           ]
