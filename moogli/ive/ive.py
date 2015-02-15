from PyQt4 import QtCore, QtGui, Qt
# from QtCore import *
from PyQt4.QtCore import pyqtSlot
from PyQt4.QtGui import QMainWindow
from PyQt4.QtGui import QMdiArea
from PyQt4.QtGui import QBrush
from PyQt4.QtGui import QColor
from PyQt4.QtGui import QKeySequence
from PyQt4.QtGui import QAction
from PyQt4.QtGui import QFileDialog
# from Qt import *
import moogli
from utils import *

class NetworkVisualizer(moogli.Viewer):
    # search and focus
    # click on compartment and - get property
    # set property
    # plot property
    pass


class IntegratedVisualizationEnvironment(QMainWindow):
    def __init__(self, parent=None, flags=Qt.Qt.Window):
        super(IntegratedVisualizationEnvironment, self).__init__(parent, flags)
        self._setup_main_window()
        self._setup_actions()
        self._setup_menubar()
        # self._application.aboutToQuit.connect(self.stop_moose_servers)
        # self._setup_toolbars()
        # [self.load_slot(model) for model in models]

    # @QtCore.pyqtSlot(str)
    # def busy_slot(self, message = DEFAULT_MESSAGE):
    #     self.message_box.setText(message)
    #     self.message_box.show()

    # @QtCore.pyqtSlot()
    # def free_slot(self):
    #     self.message_box.hide()


    def _setup_main_window(self):
        self.setWindowTitle("MOOGLI - Integrated Visualization Environment")
        self.setWindowFlags(self.windowFlags() |
                            QtCore.Qt.WindowContextHelpButtonHint |
                            QtCore.Qt.CustomizeWindowHint |
                            QtCore.Qt.WindowMinimizeButtonHint |
                            QtCore.Qt.WindowMaximizeButtonHint) # | QtCore.Qt.WindowStaysOnTopHint)
        self.setDockOptions(QMainWindow.AnimatedDocks |
                            QMainWindow.AllowNestedDocks |
                            QMainWindow.AllowTabbedDocks |
                            QMainWindow.VerticalTabs)
        # self.setWindowIcon(QIcon(APPLICATION_ICON_PATH))
        self.setAcceptDrops(True)
        self.setWindowState(QtCore.Qt.WindowFullScreen)
        self.showFullScreen()

        widget = MdiArea()
        self.setCentralWidget(widget)
        self.centralWidget().setDocumentMode(True)
        self.centralWidget().setViewMode(QMdiArea.SubWindowView)
        self.centralWidget().tileSubWindows()
        # background = QBrush( QColor(255, 255, 255, 255)
        #                    , QPixmap(APPLICATION_BACKGROUND_PATH)
        #                    )
        # # background.setColor()
        # centralWidget.setBackground(background)
        # w = IPythonConsole(globals())
        # w.setWindowFlags(QtCore.Qt.Window | QtCore.Qt.FramelessWindowHint)
        # centralWidget.addSubWindow(w)
        # self.centralWidget().addSubWindow(widgets.IPythonConsole(globals()))
        # centralWidget.addSubWindow(IPythonConsole(globals() ))
        # return centralWidget

    def _setup_actions(self):

        # def create_import_morphology_data_action():
        #     action = QAction("Import Morphology Data", self)
        #     action.setToolTip("Load morphology from swc, nml, txt, dat, csv, tsv or nsdf file.")
        #     action.setShortcut(QKeySequence(QtCore.Qt.CTRL + QtCore.Qt.Key_N))
        #     action.setShortcutContext(QtCore.Qt.ApplicationShortcut)
        #     return action

        # def create_import_morphology_action():
        #     action = QAction("Import Simulation Data", self)
        #     action.setToolTip("Load simulation data from txt, dat, csv, tsv, npz or nsdf file.")
        #     action.setShortcut(QKeySequence(QtCore.Qt.CTRL + QtCore.Qt.Key_O))
        #     action.setShortcutContext(QtCore.Qt.ApplicationShortcut)
        #     return action

        # def create_quit_action():
        #     action = QAction("Quit", self)
        #     action.setToolTip("Quit Application.")
        #     action.setShortcut(QKeySequence(QtCore.Qt.CTRL + QtCore.Qt.Key_Q))
        #     action.setShortcutContext(QtCore.Qt.ApplicationShortcut)
        #     return action

        # def create_toggle_fullscreen_action():
        #     action = QAction("Full Screen", self)
        #     action.setShortcut(QKeySequence(QtCore.Qt.Key_F11))
        #     action.setShortcutContext(QtCore.Qt.ApplicationShortcut)
        #     return action

        def create_toggle_window_arrangement_action():
            action = QAction("Tabbed View", self)
            return action

        # def create_toggle_menubar_action():
        #     action = QAction("Hide Menu Bar", self)
        #     action.setShortcut(QKeySequence(QtCore.Qt.Key_F10))
        #     action.setShortcutContext(QtCore.Qt.ApplicationShortcut)
        #     return action
        def create_action(text, tooltip, shortcut, context):
            action = QAction(text, self)
            action.setToolTip(tooltip)
            action.setShortcut(shortcut)
            action.setShortcutContext(context)
            return action

        self.import_morphology_data_action = create_action("Import Morphology Data",
                                                           "Import morphology data from files.",
                                                           QKeySequence(QtCore.Qt.CTRL+
                                                                        QtCore.Qt.Key_M),
                                                           QtCore.Qt.ApplicationShortcut)

        self.import_morphology_data_action.triggered.connect(self.import_morphology_data_slot)

        self.import_simulation_data_action = create_action("Import Simulation Data",
                                                           "Import simulation data from files.",
                                                           QKeySequence(QtCore.Qt.CTRL +
                                                                        QtCore.Qt.Key_D),
                                                           QtCore.Qt.ApplicationShortcut)
        self.import_simulation_data_action.triggered.connect(self.import_simulation_data_slot)

        self.quit_application_action = create_action("Quit Application",
                                                     "Close the program.",
                                                     QKeySequence(QtCore.Qt.CTRL +
                                                                  QtCore.Qt.Key_Q),
                                                     QtCore.Qt.ApplicationShortcut)
        self.quit_application_action.triggered.connect(self.close)

        self.toggle_fullscreen_action = create_action("Toggle Fullscreen",
                                                      "Toggle Fullscreen mode.",
                                                      QKeySequence(QtCore.Qt.Key_F11),
                                                      QtCore.Qt.ApplicationShortcut)
        self.toggle_fullscreen_action.triggered.connect(self.toggle_fullscreen_slot)

        self.toggle_window_arrangement_action = create_action("Tabbed View",
                                                              "Display sub-windows with tabs in a tab bar.",
                                                              QKeySequence(QtCore.Qt.Key_F10),
                                                              QtCore.Qt.ApplicationShortcut)
        self.toggle_window_arrangement_action.triggered.connect(self.toggle_window_arrangement_slot)


    def _setup_menubar(self):

        def create_file_menu(menubar):
            menu = menubar.addMenu("File")
            menu.addAction(self.import_morphology_data_action)
            menu.addAction(self.import_simulation_data_action)
            menu.addAction(self.quit_application_action)
            return menu

        def create_view_menu(menubar):
            menu = menubar.addMenu("View")
            menu.addAction(self.toggle_fullscreen_action)
            # menu.addAction(self.toggle_menubar_action)
            menu.addAction(self.toggle_window_arrangement_action)
            return menu

        # def create_widgets_menu(menubar):
        #     menu = menubar.addMenu("Widgets")
        #     menu.addAction(self.interactive_python_console_action)
        #     return menu

        # def create_help_menu(menubar):
        #     menu = menubar.addMenu("Help")
        #     menu.addAction(self.gui_documentation_action)
        #     menu.addAction(self.moose_documentation_action)
        #     menu.addAction(self.report_bug_action)
        #     menu.addAction(self.request_feature_action)
        #     return menu

        menubar = self.menuBar()
        create_file_menu(menubar)
        create_view_menu(menubar)
        # create_widgets_menu(menubar)
        # create_help_menu(menubar)

        return menubar

    @pyqtSlot()
    def import_morphology_data_slot(self):
        nameFilters = ["All Morphology Data Files (" +
                       " ".join(map(lambda x: "*." + x, MORPHOLOGY_DATA_FILE_EXTENSIONS)) +
                       ")"]

        dialog = QFileDialog(self)
        dialog.setFileMode(QFileDialog.ExistingFiles)
        dialog.setNameFilters(nameFilters)
        files = dialog.selectedFiles() if dialog.exec_() else []
        for filename in files:
                DEBUG("Importing morphology data from '{0}'".format(filename))


    @pyqtSlot()
    def import_simulation_data_slot(self):
        nameFilters = ["All Simulation Data Files (" +
                       " ".join(map(lambda x: "*." + x, SIMULATION_DATA_FILE_EXTENSIONS)) +
                       ")"]

        dialog = QFileDialog(self)
        dialog.setFileMode(QFileDialog.ExistingFiles)
        dialog.setNameFilters(nameFilters)
        files = dialog.selectedFiles() if dialog.exec_() else []
        for filename in files:
                DEBUG("Importing simulation data from '{0}'".format(filename))

    @pyqtSlot()
    def toggle_window_arrangement_slot(self):
        if self.centralWidget().viewMode() == QMdiArea.SubWindowView:
            self.centralWidget().setViewMode(QMdiArea.TabbedView)
            self.toggle_window_arrangement_action.setText("Sub Window View")
            self.toggle_window_arrangement_action.setToolTip("Display sub-windows with window frames.")
        else:
            self.centralWidget().setViewMode(QMdiArea.SubWindowView)
            self.toggle_window_arrangement_action.setText("Tabbed View")
            self.toggle_window_arrangement_action.setToolTip("Display sub-windows with tabs in a tab bar.")

    @pyqtSlot()
    def toggle_fullscreen_slot(self):
        if self.windowState() & QtCore.Qt.WindowFullScreen:
            self.showNormal()
        else:
            self.showFullScreen()


class MdiArea(QMdiArea):
    """Multi Document Interface window. Allows any number of child widgets
       to be displayed in a tiled or tabified manner.
    """
    def __init__(self):
        super(MdiArea, self).__init__()
        brush = QBrush(QColor(255, 0, 0, 0))
        self.setBackground(brush)
