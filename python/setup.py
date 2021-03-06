import sys
from distutils.core import setup
from distutils.extension import Extension
from Cython.Distutils import build_ext

if sys.platform == 'darwin':
  # assumes source/brew install on OSX 
  QT_HEADER_DIR = "/usr/local/include/"
  EIGEN_INCLUDE_DIRS = ["/usr/local/include/eigen3"]
  DART_INCLUDE_DIRS = ["/usr/local/include/dart/"]
else:
  # assumes apt install on ubuntu
  QT_HEADER_DIR = "/usr/include/qt4/"
  EIGEN_INCLUDE_DIRS = ["/usr/include/eigen3"]
  DART_INCLUDE_DIRS = ["/usr/local/include/dart/"]

QT_INCLUDE_DIRS = [QT_HEADER_DIR + dir for dir in ["QtDesigner","QtDeclarative","QtScriptTools","QtDBus","QtDesigner","QtXml",
	"QtSql","QtOpenGL","QtNetwork","QtXmlPatterns","QtWebKit","QtGui","QtCore"]]

GRIP_INCLUDE_PATH_PREFIX = "../"
GRIP_INCLUDE_DIRS = ["../include", "../osgDart/", "../osgGolems/", "../qtWidgets/", "../build/qtWidgets"]

INCLUDE_DIRS = [QT_HEADER_DIR] + QT_INCLUDE_DIRS + GRIP_INCLUDE_DIRS + EIGEN_INCLUDE_DIRS + DART_INCLUDE_DIRS

GRIP_LIBRARY_PATH_PREFIX = "../build/lib/"

ext_modules=[
    Extension("pygrip", ["pygrip.pyx"],
      libraries=[GRIP_LIBRARY_PATH_PREFIX + "grip-interface"],
      language="c++",
      include_dirs=INCLUDE_DIRS,
      extra_compile_args = ["-std=c++11"]
     )
]

setup(
    name = "pygrip",
    cmdclass = {"build_ext": build_ext},
    ext_modules = ext_modules
)
