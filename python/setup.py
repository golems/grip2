from distutils.core import setup
from distutils.extension import Extension
from Cython.Distutils import build_ext

MAC=False
if MAC:
  QT_HEADER_DIR = "/usr/local/include/" # source install on OSX (jon)
  EIGEN_INCLUDE_DIRS = ["/usr/local/include/eigen3"]
  DART_INCLUDE_DIRS = ["/usr/local/include/dart/"]
else:
  QT_HEADER_DIR = "/usr/include/qt4/"
  EIGEN_INCLUDE_DIRS = ["/usr/include/eigen3"]
  DART_INCLUDE_DIRS = ["/usr/include/dart/"]

QT_INCLUDE_DIRS = [QT_HEADER_DIR + dir for dir in ["QtDesigner","QtDeclarative","QtScriptTools","QtDBus","QtDesigner","QtXml",
	"QtSql","QtOpenGL","QtNetwork","QtXmlPatterns","QtWebKit","QtGui","QtCore"]]

GRIP_INCLUDE_PATH_PREFIX = "../"
GRIP_INCLUDE_DIRS = ["../include", "../osgDart/", "../osgGolems/", "../qtWidgets/", "../build/qtWidgets"]

INCLUDE_DIRS = [QT_HEADER_DIR] + QT_INCLUDE_DIRS + GRIP_INCLUDE_DIRS + EIGEN_INCLUDE_DIRS + DART_INCLUDE_DIRS

ext_modules=[
    # Extension("pygrip", ["pygrip.pyx"],
    #   libraries=["../lib/grip-test"],
    #   language="c++",
    #   include_dirs = ["/usr/include/qt4/QtDesigner","/usr/include/qt4/QtDeclarative","/usr/include/qt4/QtScriptTools","/usr/include/qt4/QtDBus","/usr/include/qt4/QtDesigner"
    #   ,"/usr/include/qt4/QtXml","/usr/include/qt4/QtSql","/usr/include/qt4/QtOpenGL","/usr/include/qt4/QtNetwork","/usr/include/qt4/QtXmlPatterns","/usr/include/qt4/QtWebKit",
    #   "/usr/include/qt4/QtHelp","/usr/include/qt4/QtUiTools","/usr/include/qt4/QtTest","/usr/include/qt4/QtScript","/usr/include/qt4/QtSvg","/usr/include/qt4/Qt3Support",
    #   "/usr/include/qt4/QtGui","/usr/include/qt4/QtCore","/usr/share/qt4/mkspecs/default","/usr/include/qt4","/usr/include/qt4/QtCore"]
    #  )
    Extension("pygrip", ["pygrip.pyx"],
      libraries=["../lib/grip-interface"],
      language="c++",
      include_dirs=INCLUDE_DIRS,
     )
]

setup(
    name = "pygrip",
    cmdclass = {"build_ext": build_ext},
    ext_modules = ext_modules
)
