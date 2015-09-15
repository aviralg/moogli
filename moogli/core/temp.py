a + s
b + (a + s)


window averaging - average data over a set of simulation time points
Report maximum / minimum
Report values above and below a threshold
Spike Events


Always keep original simulation data
process based upon settings
use the processed simulation data for visualization

Report maximum
Report minimum
Report value with maximum magnitude
Report spike with minthreshold and maxthreshold

maximum = max
minimum = min
average = lambda xs : sum(xs) / len(xs)
magmax  = lambda xs : (lambda x, y : x if abs(x) > abs(y) else y)(max(xs), min(xs))
spike   = lambda limit : lambda xs, l = limit : True if maximum(xs) >= l else False


NetworkVisualizer <- NetworkViewer + DataSource

Data Source can be file system or Simulator running simulation in real time.


There is a


NetworkVisualizer



class SimulationData():
    def __init__(self, time_series, spike_)




NetworkModelSource -> from a simulator
                   -> from a file

SimulationDataSource -> from a simulator
                     -> from a file

DataTransformer(normalizer, color_mapper,   ) ->

spike_data -> color or size
vm_data    -> color or size
decay factors


MultipleDataSources ->



NetworkViewer         NetworkModelSource
                      SimulationDataSource


information about -> downsampling
                  ->


class DataSource:
    pass

There has to be a way to -> start, pause, rewind, get next item in the sequence,
step forward
step backward
fast forward
fast backward

Simulation generates data

SimulationData


Ideal scenario -

Get Data
Transform Data


Simulator
    next -> provides data for


class MooseDataSource(DataSource):
    def initialize(self):
       moose.reinit()

    def start(self):
        pass

    def stop(self):
        pass

    def pause(self):
        pass

    def step_forward(self):
        pass

    def step_backward(self):
        pass

    def fast_forward(self):
        pass

    def fast_backward(self):
        pass

NetworkViewer -> prelude
                 interlude
                 postlude

NetworkVisualizer ->





Visualizer  =
DataSource  =
Simulator   =

class Simulator():
    def next():
        pass


SimulationEvent(object):

class SpikeTimes(object):
    def __init__(self, start = 0.0, end = 0.0, dt = 0.0, data = {}):
        self.spike_times  = spike_times
        self.start = start
        self.end   = end
        self.dt    = dt

class TimeSeries(object):
    def __init__(self, begin = 0.0, end = 0.0, dt = 0.0, time_series = {}):
        self.time_series  = time_series
        self.begin = begin
        self.end   = end
        self.dt    = dt

    def _downsample(self, factor, downsampler, sequence):
        return [ downsampler(sequence[i:i + factor])
                 for i in range(0, len(sequence), factor)
               ]

    def _threshold(self, factor, thresholder, sequence):
        times = []
        for i in range(0, len(sequence), factor):
            if thresholder(sequence[i:i+points]):
                times.append(i * self.dt)
        return times

    def downsample(self, factor, downsampler = average):
        time_series = TimeSeries()
        time_series.begin = begin
        time_series.end   = end
        time_series.dt    = self.dt * factor
        size              = (self.end - self.begin) / dt
        time_series.time_series.update(
            [ (visualizable, self._downsample(factor, downsampler, sequence) for
              (visualizable, sequence) in self.time_series.items()
            ]
                                      )
        return time_series


    def threshold(self, factor, thresholder):
        spike_times = SpikeTimes()
        spike_times.begin = self.begin
        spike_times.end   = self.end
        spike_times.dt    = self.dt * factor
        for (visualizable, sequence) in self.time_series.items():
            times = self._threshold(factor, thresholder, sequence)
            if len(times) != 0:
                spike_times.spike_times[visualizable] = times
        return spike_times


    def downsample_or_threshold(self, factor, downsampler, thresholder):
        time_series = TimeSeries()
        spike_times = SpikeTimes()
        time_series.begin = spike_times.begin = self.begin
        time_series.end   = spike_times.end   = self.end
        time_series.dt    = spike_times.dt    = self.dt * factor

        for (visualizable, sequence) in self.time_series.items():
            times = self._threshold(factor, thresholder, sequence)
            if len(times) == 0:
                time_series.time_series[visualizable] =
                    self._downsample( factor
                                    , downsampler
                                    , sequence
                                    )
            else:
                spike_times.spike_times[visualizable] = times

        return (time_series, spike_times)

    def update(self, filename):
        data = parser.parse(filename)
        parser.

    def write(self, filename):
        pass




data can be read from source <- simulator
                                filesource

def update_controller( visualizable
                     , value
                     , is_spiking = False
                     , has_spiked = False
                     ):
    clamp( ( normalizer(value) + spike_addend if is_spiking else 0.0 )
           * (spike_decay_factor if has_spiked else 1.0)
         , 0.0
         , 1.0
         )
    self.set_controller()


compartment -> time series
compartment -> time points

To Update Color ->

for compartment in compartments :
    if compartment spikes:
        color_controller = 1.0
    else:
        color_controller = normalize(value)
    if compartment in history:
        color_controller = reduce(value)
    compartment.set_color(colormap(color_controller))


for compartment in compartments :
    if compartment spikes:
        radius_controller = 1.0
    else:
        radius_controller = normalize(value)
    if compartment in history:
        radius_controller = reduce(value)
    compartment.set_radius(radiusmap(radius_controller))

    read time series for that time


entities
vm          = radius color
im          =
ca          =

class SimulationEvent():
    def

class Simulation(object):

    def __init__(self):
        self._history_

    def spike(self, morphology):
        pass

    def timeseries(self):
        pass



    def __call__(self, morphology):




def normalizer(minvalue, maxvalue):
    return lambda value : max( min( 1.0
                                  , (value - minvalue) / (maxvalue - minvalue)
                                  )
                             , 0.0
                             )




class Group():
    def

class ColorGroup(Group):
    def __call__()


class SizeGroup(Group):



class Group(object):
    def __init__(self, key, elements, normalizer, mapper):
        self.key        = key
        self.elements   = elements
        self.colormap   = colormap
        self.normalizer = normalizer

    def simulation_event()
    def __hash__(self):
        return key.__hash__()

    def __eq__(self, group):
        return self.__hash__() == group.__hash__()

    def analog(self, data, index):
        [ element.set_color(data[element.id][index]) for element in elements]

    def discrete(self, data, indices):
        for element in elements:

    def spike(self,)

