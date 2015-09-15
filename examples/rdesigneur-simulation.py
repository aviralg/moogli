import numpy
import pylab
import moose
from moose import neuroml
import matplotlib.pyplot as plt
import sys
import os
from moose.neuroml.ChannelML import ChannelML
sys.path.append('/home/aviral/programming/Projects/moose-trunk-pristine-2/Demos/util')
import rdesigneur as rd
import PyQt4
from PyQt4 import QtCore, QtGui
from PyQt4.QtGui import QWidget, QGridLayout
import moogli
import pprint
from moogli.visualization.pipeline import Clock
from moogli.visualization.pipeline import Pipeline
from moogli.extensions.swc import SwcNetwork
from moogli.extensions.table import TableContinuousDataProducer
from moogli.visualization.pipeline import SimulationDataMediator
from moogli.visualization.pipeline import SimulationDataProducer
from moogli.visualization import NetworkVisualizer
from moogli.visualization.plots import LinePlotter
from moogli.extensions.moose import MooseNetwork
from matplotlib.cm import rainbow
from PyQt4.QtGui import QApplication
from PyQt4.QtGui import QSplitter
import sys
from moogli.visualization import NetworkVisualizationEnvironment


PI = 3.14159265359
frameRunTime = 0.001
runtime = 0.5
inject = 10e-10
simdt = 5e-5
FaradayConst = 96845.34
useGssa = True
combineSegments = False
spineSpacing = 5e-6
spineSpacingDistrib = 1e-6
size = 1.0
sizeDistrib = 0.5
angle = 0
angleDistrib = 2 * PI

rdes = None

def setup_model(filename):
    global rdes
    numpy.random.seed(1234)
    ######################################################################
    # Define prototypes
    cellProto = [[filename, 'elec']]
    chanProto = [['./CA1_migliore_reference/hd.xml'],
                 ['./CA1_migliore_reference/kap.xml'],
                 ['./CA1_migliore_reference/kad.xml'],
                 ['./CA1_migliore_reference/kdr.xml'],
                 ['./CA1_migliore_reference/na3.xml'],
                 ['./CA1_migliore_reference/nax.xml'],
                 ['../../Projects/07JunScanFig/chans/CaConc.xml'],
                 ['../../Projects/07JunScanFig/chans/Ca.xml'],
                 ['../../Projects/07JunScanFig/chans/NMDA.xml'],
                 ['../../Projects/07JunScanFig/chans/Glu.xml']]
    spineProto = [['makeSpineProto()', 'spine']]
    chemProto = [['psd52.g', 'chem']]
    ######################################################################
    # Specify model
    rdes = rd.rdesigneur(
        useGssa=useGssa,
        combineSegments=combineSegments,
        cellProto=cellProto,
        chanProto=chanProto,
        spineProto=spineProto,
        chemProto=chemProto,
        passiveDistrib=[[".", "#",
                         "EM", "-58e-3",
                         "initVm", "-65e-3",
                         "RM", "2.8",
                         "CM", "0.01",
                         "RA", "1.5"],
                        [".", "#axon#", "RA", "0.5"]],
        chanDistrib=[
            ["hd", "#dend#,#apical#", "Gbar", "5e-2*(1+(p*3e4))"],
            ["kdr", "#", "Gbar", "100"],
            ["na3", "#soma#,#dend#,#apical#", "Gbar", "250"],
            ["nax", "#axon#", "Gbar", "1250"],
            ["kap", "#axon#,#soma#", "Gbar", "300"],
            ["kap", "#dend#,#apical#", "Gbar", "300*H(100e-6 - p) * (1+(p*1e4))"],
            ["Ca_conc", "#dend#,#apical#", "tau", "0.0133"],
            ["kad", "#dend#,#apical#", "Gbar", "300*H(p - 100e-6) * (1+p*1e4)"],
            ["Ca", "#dend#,#apical#", "Gbar", "50"]],
        spineDistrib=[['spine', '#apical#',
                       'spineSpacing', str(spineSpacing),
                       'spineSpacingDistrib', str(spineSpacingDistrib),
                       'angle', str(angle),
                       'angleDistrib', str(angleDistrib),
                       'size', str(size),
                       'sizeDistrib', str(sizeDistrib)]],
        chemDistrib=[["chem", "#apical#", "install", "1"]],
        adaptorList=[
            ['Ca_conc', 'Ca', 'psd/Ca_input', 'concInit', 8e-5, 1.0],
            ['Ca_conc', 'Ca', 'dend/Ca_dend', 'concInit', 8e-5, 1.0],
            ['psd/tot_PSD_R', 'n', 'glu', 'Gbar', 0, 0.01],
        ]
    )

######################################################################
    # Make the model
    rdes.buildModel('/model')

    ######################################################################
    # Run and display control
    rdes.soma.inject = inject
    for i in moose.wildcardFind('/library/##'):
        i.tick = -1

    for i in range(8):
        moose.setClock(i, simdt)
    moose.setClock(18, 0.005)
    moose.reinit()

    # Now we set up the display
    # moose.le( '/model/ca1/soma' )
    soma = moose.element('/model/elec/soma')
    kap = moose.element('/model/elec/soma/kap')

    graphs = moose.Neutral('/graphs')

    ca = moose.vec('/model/chem/spine/Ca_Spine')
    catab = moose.Table2('/graphs/catab', 20).vec
    for i in range(0, 100, 5):
        moose.connect(catab[i/5], 'requestOut', ca[i], 'getConc')
    psdR = moose.vec('/model/chem/psd/tot_PSD_R')
    print "psd", moose.element("/model/chem/psd")
    print "tot_PSD_R", moose.element('/model/chem/psd/tot_PSD_R')
    print "psd[0]", psdR[3].path
    psdRtab = moose.Table2('/graphs/psdRtab', 20).vec
    for i in range(0, 100, 5):
        moose.connect(psdRtab[i/5], 'requestOut', psdR[i], 'getN')
    vtab = moose.Table('/graphs/vtab')
    moose.connect(vtab, 'requestOut', soma, 'getVm')
    kaptab = moose.Table('/graphs/kaptab')
    moose.connect(kaptab, 'requestOut', kap, 'getGk')

    compts = moose.wildcardFind(rdes.elecid.path + "/#[ISA=CompartmentBase]")
    ecomptPath = map(lambda x: x.path, compts)
    print 'LEN COMPTS = ', len(compts)
    # print "Table", psdRtab[1].vector
    # moose.start(runtime)
    # print "table", psdRtab[1].vector
    # moose.start(5 * frameRunTime)
    # print "table", psdRtab[1].vector
    # # moose.start(5 * frameRunTime)
    # # print "table", psdRtab[1].vector
    # moose.start(5 * frameRunTime)
    # print "table", psdRtab[1].vector
    # exit(0)

setup_model("./VHC-neuron.CNG.swc")


class MooseClock(Clock):
    def advance(self):
        moose.start(self.interval)
        Clock.advance(self)


class MooseDataProducer(SimulationDataProducer):
    pass


    #     catab = moose.Table2('/graphs/catab', 20).vec
    #     for i in range(0, 100, 5):
    #         moose.connect(catab[i/5], 'requestOut', ca[i], 'getConc')
    #     psdR = moose.vec('/model/chem/psd/tot_PSD_R')
    #     print "psd", moose.element("/model/chem/psd")
    #     psdRtab = moose.Table2('/graphs/psdRtab', 20).vec
    #     for i in range(0, 100, 5):
    #         moose.connect(psdRtab[i/5], 'requestOut', psdR[i], 'getN')
    #         vtab = moose.Table('/graphs/vtab')
    # moose.connect(vtab, 'requestOut', soma, 'getVm')
    # kaptab = moose.Table('/graphs/kaptab')
    # moose.connect(kaptab, 'requestOut', kap, 'getGk')


app = QApplication(sys.argv)

clock = MooseClock(0.0, 0.5, 0.005)

# network = SwcNetwork("VHC-neuron.CNG.swc")

# producer = TableContinuousDataProducer("./file.dat", "Vm", downsampler, " ")

# visualizer = NetworkVisualizer(network)
# visualization_mediator = SimulationDataMediator("color", normalizer, mapper,
#                                                0.0)


def psd_pipeline():
    class PsdDataProducer(MooseDataProducer):
        def __init__(self):
            MooseDataProducer.__init__(self, "N", None)
            self.tables = moose.Table2('/graphs/psdRtab', 20).vec

        def produce(self):
            for i in range(20):
                self.output["psdR[" + str(5 * i) + "]"] = self.tables[i].vector[-1]

    psd_producer = PsdDataProducer()

    psd_plotter = LinePlotter(visualizables=[
        "psdR[" + str(5 * x) + "]" for x in range(20)])
    plotting_mediator = SimulationDataMediator("N", lambda x, y, z: y,
                                               lambda x, y: y, 0.0)
    return psd_producer, plotting_mediator, psd_plotter


def vm_pipeline():
    class VmDataProducer(MooseDataProducer):
        def __init__(self):
            MooseDataProducer.__init__(self, "Vm", None)
            self.table = moose.Table('/graphs/vtab')
 
        def produce(self):
            self.output["soma"] = self.table.vector[-1]

    vm_producer = VmDataProducer()

    vm_plotter = LinePlotter(visualizables=["soma"])
    vm_plotter.set_title("Soma Vm")
    plotting_mediator = SimulationDataMediator("Vm", lambda x, y, z: y,
                                               lambda x, y: y, 0.0)
    return vm_producer, plotting_mediator, vm_plotter


def morph_vm_pipeline():

    def mapper(visualizable, new_param):
        return rainbow(new_param)

    def normalizer(visualizable, value, old_param):
        return (value + 0.08) / (0.05 + 0.08)

    class MorphVmProducer(MooseDataProducer):
        def __init__(self):
            MooseDataProducer.__init__(self, "Vm", None)
            self.compts = moose.wildcardFind(rdes.elecid.path + "/#[ISA=CompartmentBase]")
            self.produce()

        def produce(self):
            for compt in self.compts:
               self.output[compt.path] = compt.Vm

    morph_producer = MorphVmProducer()
    morph_visualizer = NetworkVisualizer(MooseNetwork(model_path = rdes.elecid.path))
    morph_visualizer.forward(1250.0, 0)
    morph_mediator = SimulationDataMediator("color", normalizer, mapper,0.0)
    return morph_producer, morph_mediator, morph_visualizer

def kap_pipeline():
    class KapDataProducer(MooseDataProducer):
        def __init__(self):
            MooseDataProducer.__init__(self, "Gk", None)
            self.table = moose.Table('/graphs/kaptab')

        def produce(self):
            self.output["soma"] = self.table.vector[-1]

    kap_producer = KapDataProducer()

    kap_plotter = LinePlotter(visualizables=["soma"])
    kap_plotter.set_title("Soma Kap")
    plotting_mediator = SimulationDataMediator("Gk", lambda x, y, z: y,
                                               lambda x, y: y, 0.0)
    return kap_producer, plotting_mediator, kap_plotter


def main():
    environment = NetworkVisualizationEnvironment(clock)
    splitter = QSplitter()
    plots = QWidget()
    plots.setLayout(QGridLayout())
    splitter.setOrientation(QtCore.Qt.Horizontal)
    (p, m, c) = morph_vm_pipeline()
    environment.insert(Pipeline(p, m, c))
    splitter.addWidget(c)
    (p, m, c) = psd_pipeline()
    environment.insert(Pipeline(p, m, c))
    plots.layout().addWidget(c)
    # splitter.addWidget(c)
    (p, m, c) = vm_pipeline()
    environment.insert(Pipeline(p, m, c))
    plots.layout().addWidget(c)
    # splitter.addWidget(c)
    (p, m, c) = kap_pipeline()
    environment.insert(Pipeline(p, m, c))
    plots.layout().addWidget(c)
    # splitter.addWidget(c)
    splitter.addWidget(plots)
    environment.layout().addWidget(splitter)
    environment.show()
    environment.run()
    app.exec_()


    # # Graphics stuff here.
    # app = QtGui.QApplication(sys.argv)
    # morphology = moogli.read_morphology_from_moose(name = "", path = rdes.elecid.path )
    # morphology.create_group( "group_all", ecomptPath, -0.08, 0.02, \
    #         [0.0, 0.0, 1.0, 1.0], [1.0, 0.0, 0.0, 0.1] )

    # viewer = moogli.DynamicMorphologyViewerWidget(morphology)

    # def callback( morphology, viewer ):
    #     moose.start( frameRunTime )
    #     Vm = map( lambda x: moose.element( x ).Vm, compts )
    #     morphology.set_color( "group_all", Vm )
    #     currTime = moose.element( '/clock' ).currentTime
    #     #print currTime, compts[0].Vm
    #     if ( currTime < runtime ):
    #         return True
    #     else:
    #         finalizePlots( runtime, vtab, catab, psdRtab, kaptab )
    #         return False

    # viewer.set_callback( callback, idletime = 0 )
    # viewer.showMaximized()
    # viewer.show()
    # app.exec_()
    # return "/model"


def finalizePlots( runtime, vtab, catab, psdRtab, kaptab):
    t = numpy.arange( 0, runtime, vtab.dt )
    t2 = numpy.arange( 0, runtime, catab[0].dt )
    fig = plt.figure( figsize=(14,12) )
    p1 = fig.add_subplot(311)
    for i in range( 20 ):
        #p1.plot( t2,  catab[i].vector, label = 'Ca Spine' + str( i * 5 ) )
        p1.plot( t2,  psdRtab[i].vector, label = 'psdR' + str( i * 5 ) )
    p2 = fig.add_subplot(312)
    p2.plot( t,  vtab.vector, label = 'Vm Soma' )
    p2.legend()
    p3 = fig.add_subplot(313)
    p3.plot( t, kaptab.vector, label = 'kap Soma' )
    p3.legend()
    plt.show()




if __name__ == '__main__':
    main()
