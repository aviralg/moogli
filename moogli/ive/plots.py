from __future__ import unicode_literals
import sys
import os
import random
import csv
from PyQt4 import Qt, QtGui, QtCore

from PyQt4.QtGui import *
from PyQt4.QtCore import *
from matplotlib.lines import Line2D
from matplotlib.backends.backend_qt4agg import FigureCanvasQTAgg as FigureCanvas
from matplotlib.backends.backend_qt4agg import NavigationToolbar2QTAgg as NavigationToolbar
from matplotlib.figure import Figure
import numpy

class PlotWidget(QWidget):
    def __init__(self,
                 field,
                 parent=None,
                 figure_width=5.0,
                 figure_height=4.0,
                 dpi=100,
                 facecolor="",
                 legend_alpha=0.5):
        QWidget.__init__(self, parent)
        self.field = field
        self.figure_width = figure_width
        self.figure_height = figure_height
        self.dpi = dpi
        self.grid_visible = False
        self.legend_visible = True
        self.facecolor = facecolor
        self.legend_alpha = legend_alpha
        self._setup_main_window()
        self._setup_actions()
        self._setup_context_menu()

    def _setup_main_window(self):
        self.setContextMenuPolicy(QtCore.Qt.CustomContextMenu)
        self.setLayout(QGridLayout())
        self.figure = Figure((self.figure_width, self.figure_height),
                             dpi=dpi,
                             facecolor='#F3EFEE')
        self.canvas = FigureCanvas(self.figure)
        self.layout().addWidget(self.canvas, 0, 0)

        self.navigation_toolbar = NavigationToolbar(self.canvas, self)
        self.layout().addWidget(self.navigation_toolbar, 1, 0)

        self.axes = self.figure.add_subplot(1, 1, 1)
        self.legend = None
        self._set_field(self.field)

    def _setup_actions(self):
        self.toggle_grid_action = QAction(self)
        self.toggle_grid_action.setText("Show Grid")
        self.toggle_grid_action.triggered.connect(self.toggle_grid)

        self.toggle_autoscale_action = QAction(self)
        self.toggle_autoscale_action.setText("Enable Autoscaling")
        self.toggle_autoscale_action.triggered.connect(self.toggle_autoscale)

        self.toggle_axis_hold_action = QAction(self)
        self.toggle_axis_hold_action.setText("Hold Axes")
        self.toggle_axis_hold_action.triggered.connect(self.toggle_axis_hold)

        self.toggle_legend_action = QAction(self)
        self.toggle_legend_action.setText("Hide Legend")
        self.toggle_legend_action.triggered.connect(self.toggle_legend)

        self.export_to_csv_action = QAction(self)
        self.export_to_csv_action.setText("CSV")
        self.export_to_csv_action.triggered.connect(self.export_to_csv)

        self.canvas.mpl_connect('pick_event', self.pick_event_slot)

        self.connect(self,
                     SIGNAL("customContextMenuRequested(QPoint)"),
                     self,
                     SLOT("show_context_menu(QPoint)"))

    def _setup_context_menu(self):
        self.context_menu = QMenu()
        self.context_menu.addAction(self.toggle_grid_action)
        self.context_menu.addAction(self.toggle_autoscale_action)
        self.context_menu.addAction(self.toggle_axis_hold_action)
        self.context_menu.addAction(self.toggle_legend_action)
        export_menu = self.context_menu.addMenu("Export")
        export_menu.addAction(self.export_to_csv_action)


    @pyqtSlot()
    def get_title(self):
        return self.axes.get_title()

    @pyqtSlot(str)
    def set_title(self, title):
        self.axes.set_title(title)

    @pyqtSlot()
    def get_field(self):
        return self.field

    @pyqtSlot(str, str)
    def set_field(self, field, unit):
        self.field = field
        self.unit = unit
        self.axes.set_xlabel("Time (s)")
        self.axes.set_ylabel("{field} ({unit})".format(field=field, unit=unit))

    @pyqtSlot(object)
    def add(self, element):
        self.axes.plot(numpy.array([]),
                       numpy.array([]),
                       label=str(element.id()),
                       gid=str(element.id()),
                       zorder=zorder)
        self._refresh()

    @pyqtSlot(object)
    def remove(self, element):
        index = -1
        for i in range(len(self.axes.lines)):
            if self.axes.lines[i].get_gid() == element.id():
                index = i
                break
        if index == -1:
            return
        else:
            self.axes.lines.pop(index)
            self._refresh()

    def exists(self, element):
        for line in self.axes.lines:
            if line.get_gid() == element.id():
                return True
        return False

    def _create_legend(self):
        self.legend = self.axes.legend(loc='upper right',
                                       prop={'size': 10},
                                       # bbox_to_anchor=(1.0, 0.5) ,
                                       fancybox=True,
                                       shadow=False,
                                       ncol=1)
        self.legend.draggable()
        self.legend.get_frame().set_alpha(self.legend_alpha)
        if self.legend_visible:
            self.show_legend_slot()
        else:
            self.hide_legend_slot()

    @pyqtSlot()
    def draw(self):
        self.canvas.draw()

    def pick_event(self, event):
        pass

    def show_grid(self):
        self.axes.grid(True)
        self.toggle_grid_action.setText("Hide Grid")
        self.draw_slot()

    def hide_grid(self):
        self.axes.grid(False)
        self.toggle_grid_action.setText("Show Grid")
        self.draw_slot()

    def toggle_grid(self):
        self.hide_grid_slot() if self.grid_visible else self.show_grid_slot()
        self.grid_visible = not self.grid_visible

    def toggle_autoscale(self):
        pass

    def toggle_axis_hold(self):
        pass

    def show_legend(self):
        if self.legend is None:
            return
        self.legend.set_visible(True)
        self.toggle_legend_action.setText("Hide Legend")
        self.draw_slot()

    def hide_legend(self):
        if self.legend is None:
            return
        self.legend.set_visible(False)
        self.toggle_legend_action.setText("Show Legend")
        self.draw_slot()

    def toggle_legend(self):
        self.hide_legend_slot() if self.legend_visible else self.show_legend_slot()
        self.legend_visible = not self.legend_visible

    def toggle_line_visibility(self, line):
        line.set_visible(not line.get_visible())

    @pyqtSlot(str)
    def export_to_csv(self, filepath):
        with open(filepath, "wb") as csv_file:
            writer = csv.writer( csv_file
                               , delimiter  = ' '
                               , quotechar  = '"'
                               , fieldnames = ["Time"] + self._tables.keys()
                               , quoting    = csv.QUOTE_NONNUMERIC
                               )
            writer.writerow(["#MODEL             : {model}".format(model = self.instance["model"])])
            writer.writerow(["#TIMESTAMP         : {timestamp}".format(timestamp = int(time.time()))])
            writer.writerow(["#MOOSE VERSION     : {moose_version}".format(moose_version = "123")])
            writer.writerow(["#GOOSE VERSION     : {goose_version}".format(goose_version = "456")])
            writer.writeheader()
            units = ["s"] + [self._unit] * len(self._tables)
            writer.writerow(units)
            data = [ value.get_ydata() for value in self._tables.values() ]
            data = data + data[0].get_xdata()
            data = transpose(data)
            writer.writerows(data)


    @pyqtSlot(QtCore.QPoint)
    def show_context_menu(self, point):
        self.context_menu.exec_(self.mapToGlobal(point))

    @QtCore.pyqtSlot(object)
    def update_plots_slot(self, simdata):
        print("Updating " + self.path)

        # print(self.container.path)
        # print simdata["tables"][self.container.path].keys()
        # print("Updating plot")
        for line in self.axes.lines :
            # print(line.get_gid())
            # print simdata["tables"][self.container.path][line.get_gid()]
            self.update_plot_slot( line
                                 , simdata["time"]
                                 , numpy.array(simdata["tables"][self.path][line.get_gid()])
                                 )
        self.draw_slot()
        print("Updated " + self.path)
        # print("Updated plot")

    # @QtCore.pyqtSlot(Line2D, numpy.array, numpy.array)
    def update_plot_slot(self, line, simtime, ydata):
        xdata = numpy.linspace( 0.0
                              , simtime
                              , len(ydata)
                              )
        line.set_xdata(xdata)
        line.set_ydata(ydata)



class LinePlotWidget(PlotWidget):
    pass
