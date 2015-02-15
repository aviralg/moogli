import nsdf
from nsdf.nsdfreader import NSDFReader
import sys

dataset = NSDFReader(sys.argv[1])

try:
    tstart = dataset.tstart
    tend = dataset.tend
except KeyError, e:
    tstart = 0.0
    tend = 0.0

print tstart, tend


def uniform_data(dataset):
    for population in dataset.uniform_populations:
        print "\t{}".format(population)
        for variable in dataset.get_uniform_vars(population):
            print "\t\t{}".format(variable)
            uniform_data = dataset.get_uniform_data(population, variable)
            times, units = dataset.get_uniform_ts(population, variable)
            for source in uniform_data.get_sources():
                data = uniform_data.get_data(source)
                print "\t\t\t{}".format(source)
                print "\t\t\t\t{}".format(times[0:5])
                print "\t\t\t\t{}".format(data[0:5])

def event_data(dataset):
    for population in dataset.event_populations:
        print "\t{}".format(population)
        for variable in dataset.get_event_vars(population):
            print "\t\t{}".format(variable)
            event_data = dataset.get_event_data(population, variable)
            for source in event_data.get_sources():
                data = event_data.get_data(source)
                print "\t\t\t{}".format(source)
                print "\t\t\t\t{}".format(data[0:5])

def nonuniform_data(dataset):
    for population in dataset.nonuniform_populations:
        print "\t{}".format(population)
        for variable in dataset.get_nonuniform_vars(population):
            print "\t\t{}".format(variable)
            nonuniform_data = dataset.get_nonuniform_data(population, variable)
            # times = nonuniform_data.get_times()
            for source in nonuniform_data.get_sources():
                (data, times) = nonuniform_data.get_data(source)
                print "\t\t\t{}".format(source)
                print "\t\t\t\t{}".format(times[0:5])
                print "\t\t\t\t{}".format(data[0:5])

print "uniform populations"
print "event populations"
# event_data(dataset)
print "nonuniform populations"
nonuniform_data(dataset)


