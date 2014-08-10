from distutils.core import setup
from distutils.extension import Extension
from Cython.Distutils import build_ext

ext_modules=[
    Extension("demo",
              ["demo.pyx"],
              libraries=["../lib/grip-interface"],
              language="c++",
              extra_objects=["../lib/libgrip-interface.so"],
    ),
    Extension("pygrip", ["pygrip.pyx"],
      libraries=["../lib/grip-test"],
      language="c++",
      include_dirs = ["/usr/include/qt4/QtDesigner","/usr/include/qt4/QtDeclarative","/usr/include/qt4/QtScriptTools","/usr/include/qt4/QtDBus","/usr/include/qt4/QtDesigner"
      ,"/usr/include/qt4/QtXml","/usr/include/qt4/QtSql","/usr/include/qt4/QtOpenGL","/usr/include/qt4/QtNetwork","/usr/include/qt4/QtXmlPatterns","/usr/include/qt4/QtWebKit",
      "/usr/include/qt4/QtHelp","/usr/include/qt4/QtUiTools","/usr/include/qt4/QtTest","/usr/include/qt4/QtScript","/usr/include/qt4/QtSvg","/usr/include/qt4/Qt3Support",
      "/usr/include/qt4/QtGui","/usr/include/qt4/QtCore","/usr/share/qt4/mkspecs/default","/usr/include/qt4","/usr/include/qt4/QtCore"]
     )
]

# import ipdb; ipdb.set_trace()
setup(
    name = "pydemo",
    cmdclass = {"build_ext": build_ext},
    ext_modules = ext_modules
)
