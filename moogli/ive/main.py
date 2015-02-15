import sys
from PyQt4.QtGui import QApplication
from ive import IntegratedVisualizationEnvironment

application = QApplication(sys.argv)
environment = IntegratedVisualizationEnvironment()
environment.show()
application.exec_()
