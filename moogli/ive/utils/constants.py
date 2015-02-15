from __future__ import nested_scopes
from __future__ import generators
from __future__ import division
from __future__ import absolute_import
from __future__ import with_statement
from __future__ import print_function
from __future__ import unicode_literals

import pkg_resources
import os
from time import strftime, localtime

# APPLICATION_BACKGROUND_IMAGE_PATH   = pkg_resources.resource_filename(
#     goose.__name__ , "data/images/moose.png"
#                                                                      )

# APPLICATION_ICON_PATH               = pkg_resources.resource_filename(
#     goose.__name__ , "data/icons/moose-icon.png"
#                                                                      )

# RUN_SIMULATION_ICON_PATH            = pkg_resources.resource_filename(
#     goose.__name__ , "data/icons/play.svg"
#                                                                      )

# STOP_SIMULATION_ICON_PATH           = pkg_resources.resource_filename(
#     goose.__name__ , "data/icons/pause.svg"
#                                                                      )

# RESET_SIMULATION_ICON_PATH          = pkg_resources.resource_filename(
#     goose.__name__ , "data/icons/stop.svg"
#                                                                      )

# FUNCTION_IMAGE_PATH                 = pkg_resources.resource_filename(
#     goose.__name__ , "data/images/Function.png"
#                                                                      )

# RAINBOW_COLORMAP_PATH               = pkg_resources.resource_filename(
#     goose.__name__ , "data/colormaps/rainbow.pkl"
#                                                                      )
# PLOT_ICON_PATH          = pkg_resources.resource_filename(
#     goose.__name__ , "data/icons/line-chart.svg"
#                                                                       )
# CLONE_ICON_PATH          = pkg_resources.resource_filename(
#     goose.__name__ , "data/icons/files-o.svg"
#                                                                       )
# DELETE_ICON_PATH          = pkg_resources.resource_filename(
#     goose.__name__ , "data/icons/remove.svg"
#                                                                       )
# MOVE_ICON_PATH          = pkg_resources.resource_filename(
#     goose.__name__ , "data/icons/arrows-alt.svg"
#

MORPHOLOGY_DATA_FILE_EXTENSIONS = ["swc", "nml", "txt", "dat", "csv", "tsv", "nsdf"]

SIMULATION_DATA_FILE_EXTENSIONS = ["txt", "dat", "csv", "tsv", "npz", "nsdf"]

# CHEMICAL_PLOT_CLOCK_IDS             = [18]
# CHEMICAL_SIMULATION_CLOCK_IDS       = range(11, 18)

# ELECTRICAL_PLOT_CLOCK_IDS           = [8]
# ELECTRICAL_SIMULATION_CLOCK_IDS     = range(0, 8)

# AUTOMATIC           = "AUTOMATIC"

# LINE_PLOT           = "LINE PLOT"
# SCATTER_PLOT        = "SCATTER PLOT"

# TIME                = "TIME"
# CONCENTRATION       = "CONCENTRATION"
# N           = "N"
# MEMBRANE_VOLTAGE    = "MEMBRANE VOLTAGE"
# MEMBRANE_CURRENT    = "MEMBRANE CURRENT"

# ALL_FIELDS = [ CONCENTRATION
#              , N
#              , MEMBRANE_VOLTAGE
#              , MEMBRANE_CURRENT
#              ]

# REVERSE_FIELD_DATA = { "getConc" : CONCENTRATION
#                      , "getN"    : N
#                      , "getVm"   : MEMBRANE_VOLTAGE
#                      , "getIm"   : MEMBRANE_CURRENT
#                      }

# FIELD_DATA   =   { CONCENTRATION        :   { "name"        :   "Concentration"
#                                             , "moose_field" :   "getConc"
#                                             , "unit"        :   "mM"
#                                             , "table"       :   "Table2"
#                                             , "message"     :   "getConc"
#                                             }
#                  , N                   :    { "name"        :   "N"
#                                             , "moose_field" :   "getN"
#                                             , "unit"        :    None
#                                             , "table"       :   "Table2"
#                                             , "message"     :   "getN"
#                                             }
#                  , MEMBRANE_VOLTAGE     :   { "name"        :   "Concentration"
#                                             , "moose_field" :   "getVm"
#                                             , "unit"        :   "mV"
#                                             , "table"       :   "Table"
#                                             , "message"     :   "getVm"
#                                             }
#                  , MEMBRANE_CURRENT     :   { "name"        :   "Membrane Current"
#                                             , "moose_field" :   "getIm"
#                                             , "unit"        :   "mA"
#                                             , "table"       :   "Table"
#                                             , "message"     :   "getIm"
#                                             }
#                  , TIME                 :   { "name"        :   "Time"
#                                             , "moose_field" :   "getTime"
#                                             , "unit"        :   "s"
#                                             }
#                  }

# DEFAULT_MESSAGE = "Please wait while the model is being loaded"
