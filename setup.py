# -*- coding: utf-8 -*-

"""
The following resources have been really helpful in making this setup script -

https://github.com/numpy/numpy/blob/master/setup.py
https://packaging.python.org/en/latest/index.html
https://github.com/pypa/sampleproject/blob/master/setup.py
https://docs.python.org/2/distutils/apiref.html
http://stackoverflow.com/questions/3207219/how-to-list-all-files-of-a-directory-in-python
"""

from setuptools import setup, Extension
import sipdistutils
import sys
import os

remaining = sys.argv[1:]
sys.argv = [ sys.argv[0]
           , 'build_ext'
           , '--sip-opts=-I/usr/share/sip/PyQt4/ -e -g -x VendorID -t WS_X11 -x PyQt_NoPrintRangeBug -t Qt_4_8_0 -x Py_v3 -g'
           ]
sys.argv.extend(remaining)

here = os.path.dirname(__file__)

# print(sys.argv)
# RESOURCES -
# https://docs.python.org/2.7/distutils/apiref.html?highlight=setup#distutils.core.setup
# http://pyqt.sourceforge.net/Docs/sip4/distutils.html#ref-distutils
# https://wiki.python.org/moin/EmbedingPyQtTutorial


# list of object files to be passed to the linker.
# These files must not have extensions, as the default extension for the compiler is used.
os.environ["CC"]="clang++"
extra_objects           =   [
                            ]

# list of libraries to link against
libraries               =   [ "QtCore"
                            , "QtGui"
                            , "QtOpenGL"
                            , "osg"
                            , "osgFX"
                            , "osgUtil"
                            , "osgFX"
                            , "osgGA"
                            , "osgQt"
                            , "osgAnimation"
                            , "osgViewer"
                            , "osgQt"
                            , "osgManipulator"
                            , "osgText"
                            ]

# list of directories to search for libraries at link-time
library_dirs            =   [
                            ]

# # list of directories to search for shared (dynamically loaded) libraries at run-time
runtime_library_dirs    =   [ ]

# additional command line options for the compiler command line
extra_compile_args      =   [ "-O3"
                            , "-std=c++0x"
                            , "-Wno-reorder"
                            , "-Wno-overloaded-virtual"
                            ]

# additional command line options for the linker command line
extra_link_args         =   [ "-fPIC"
                            , "-shared"
                            ]

#specify include directories to search
include_dirs            =   [ "/usr/include/qt4/"
                            , "/usr/include/qt4/QtCore/"
                            , "/usr/include/qt4/QtGui/"
                            , "/usr/include/qt4/QtOpenGL/"
                            , "./moogli/include/"
                            ]

# define pre-processor macros
define_macros           =   [
                            ]

# undefine pre-processor macros
undef_macros            =   [
                            ]

moogli = Extension( name                  =   "_moogli"
                  , sources               =   [ "moogli/src/core/Network.cpp"
                                              , "moogli/src/core/Neuron.cpp"
                                              , "moogli/src/core/Compartment.cpp"
                                              , "moogli/src/core/Voxel.cpp"
                                              , "moogli/src/view/NetworkViewer.cpp"
                                              , "moogli/src/mesh/CylinderMesh.cpp"
                                              , "moogli/src/mesh/SphereMesh.cpp"
                                              , "moogli/src/handlers/GeometrySelector.cpp"
                                              , "moogli/src/utility/record.cpp"
                                              , "moogli/src/utility/globals.cpp"
                                              , "moogli/src/utility/conversions.cpp"
                                              , "moogli/moc/NetworkViewer.moc.cpp"
                                              , "moogli/sip/moogli.sip"
                                              ]
                  , include_dirs          =   include_dirs
                  , extra_compile_args    =   extra_compile_args
                  , extra_link_args       =   extra_link_args
                  , library_dirs          =   library_dirs
                  , libraries             =   libraries
                  , extra_objects         =   extra_objects
                  , runtime_library_dirs  =   runtime_library_dirs
                  , define_macros         =   define_macros
                  , undef_macros          =   undef_macros
                  )

with open(os.path.join(here, 'requirements.txt')) as f:
    requires = f.read().splitlines()

long_description = open(os.path.join(here,"README.rst")).read()

scripts_dir = os.path.join(here, "scripts")
scripts = [os.path.join(scripts_dir,fn) for fn in next(os.walk(scripts_dir))[2]]

setup( name             =   'moogli'
     , author           =   'Aviral Goel'
     , author_email     =   'aviralg@ncbs.res.in'
     , maintainer       =   'Aviral Goel'
     , maintainer_email =   'aviralg@ncbs.res.in'
     , version          =   "0.5.0"
     , url              =   ''
     , download_url     =   ''
     , description      =   "A 3D visualizer for neuronal networks"
     , long_description =   long_description
     , classifiers      =   [ 'Development Status :: 3 - Alpha'
                            , 'Environment :: X11 Applications :: Qt'
                            , 'Intended Audience :: Science/Research'
                            , "License :: OSI Approved :: GNU General Public License v2 (GPLv2)"
                            , 'Programming Language :: Python :: 2.6'
                            , 'Programming Language :: Python :: 2.7'
                            , 'Programming Language :: C++'
                            , 'Natural Language :: English'
                            , 'Operating System :: OS Independent'
                            , 'Topic :: Scientific/Engineering'
                            ]
     , license          =   'GPLv2'
     , requires         =   requires
     , packages         =   [ "moogli"
                            , "moogli.reader"
                            , "moogli.simulator"
                            ]
     , ext_modules      =   [ moogli ]
     , cmdclass         =   { 'build_ext': sipdistutils.build_ext
                            }
     , scripts          =   scripts
)



