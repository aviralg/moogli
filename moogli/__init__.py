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
        self.voxels    = { "base_value"     :   100.0
                         , "peak_value"     :   200.0
                         , "base_color"     :   [1.0, 0.0, 0.0, 1.0]
                         , "peak_color"     :   [0.0, 1.0, 0.0, 1.0]
                         , "voxels"         :   []
                         }
        self.compartments = { "base_value"     :   -0.65
                            , "peak_value"     :   -0.60
                            , "base_color"     :   [0.0, 1.0, 0.0, 1.0]
                            , "peak_color"     :   [0.0, 0.0, 1.0, 1.0]
                            , "compartments"   :   []
                            }

        self.neurons    = { "base_value"     :   -0.65
                            , "peak_value"     :   -0.60
                            , "base_color"     :   [0.0, 1.0, 0.0, 1.0]
                            , "peak_color"     :   [0.0, 0.0, 1.0, 1.0]
                            , "neurons"     :   []
                            }
        self.spines    = { "base_value"     :   -0.65
                            , "peak_value"     :   -0.60
                            , "base_color"     :   [0.0, 1.0, 0.0, 1.0]
                            , "peak_color"     :   [0.0, 0.0, 1.0, 1.0]
                            , "spines"     :   []
                            }

        self.start()

    def _interpolate_color(self, factor, base_color, peak_color):
        def interpolate(a, b):
            return (b - a) * factor + a
        return map(interpolate, base_color, peak_color)

    def _normalize(self, value, minimum, maximum):
        result = (value - minimum) / (maximum - minimum)
        if result < 0.0 : return 0.0
        if result > 1.0 : return 1.0
        return result

    def _set_colors( self
                   , values
                   , base_value
                   , peak_value
                   , base_color
                   , peak_color
                   , members
                   ) :
        normalized_values = [ self._normalize(value, base_value, peak_value)
                              for value in values
                            ]
        colors = [ self._interpolate_color(normalized_value, base_color, peak_color)
                   for normalized_value in normalized_values
                 ]
        [member.set_color(color) for (member, color) in zip(members, colors)]

    def set_voxel_colors(self, values):
        self._set_colors( values
                        , self.voxels["base_value"]
                        , self.voxels["peak_value"]
                        , self.voxels["base_color"]
                        , self.voxels["peak_color"]
                        , self.voxels["voxels"]
                        )

    def set_compartment_colors(self, values):
        self._set_colors( values
                        , self.compartments["base_value"]
                        , self.compartments["peak_value"]
                        , self.compartments["base_color"]
                        , self.compartments["peak_color"]
                        , self.compartments["compartments"]
                        )

    def set_neuron_colors(self, values):
        self._set_colors( values
                        , self.neurons["base_value"]
                        , self.neurons["peak_value"]
                        , self.neurons["base_color"]
                        , self.neurons["peak_color"]
                        , self.neurons["neurons"]
                        )

    def set_spine_colors(self, values):
        self._set_colors( values
                        , self.spines["base_value"]
                        , self.spines["peak_value"]
                        , self.spines["base_color"]
                        , self.spines["peak_color"]
                        , self.spines["spines"]
                        )


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
