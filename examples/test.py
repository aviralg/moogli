import pprint
from moogli.visualization.pipeline import Clock
from moogli.visualization.pipeline import Pipeline
from moogli.extensions.swc import SwcNetwork
from moogli.extensions.table import TableContinuousDataProducer
from moogli.visualization.pipeline import SimulationDataMediator
from moogli.visualization import NetworkVisualizer
from moogli.visualization.plots import LinePlotter
from matplotlib.cm import rainbow
from PyQt4.QtGui import QApplication
from PyQt4.QtGui import QSplitter
import sys
from moogli.visualization import NetworkVisualizationEnvironment


def downsampler(visualizable, clock, times, values):
    # print(visualizable)
    # print(clock)
    # print(times)
    # print(values)
    if visualizable == "1":
        print clock.tick()
        print values
    return values[-1]


def mapper(visualizable, new_param):
    return rainbow(new_param)


def normalizer(visualizable, value, old_param):
    return (value + 0.08) / (0.05 + 0.08)

app = QApplication(sys.argv)

clock = Clock(0.0, 10.0, 0.5)

network = SwcNetwork("VHC-neuron.CNG.swc")

producer = TableContinuousDataProducer("./file.dat", "Vm", downsampler, " ")

visualizer = NetworkVisualizer(network)
visualization_mediator = SimulationDataMediator("color", normalizer, mapper,
                                                0.0)

plotter = LinePlotter(visualizables=["1", "2", "3", "4", "5"])
plotting_mediator = SimulationDataMediator("Vm", lambda x, y, z: y,
                                           lambda x, y: y, 0.0)

splitter = QSplitter()
splitter.addWidget(visualizer)
splitter.addWidget(plotter)

environment = NetworkVisualizationEnvironment(clock)
environment.insert(Pipeline(producer, visualization_mediator, visualizer))
environment.insert(Pipeline(producer, plotting_mediator, plotter))
# environment.layout().addWidget(visualizer, 0, 0)
# environment.layout().addWidget(plotter, 0, 1)
environment.layout().addWidget(splitter, 0, 0)
environment.show()
environment.run()
app.exec_()
