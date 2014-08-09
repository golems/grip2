from distutils.core import setup
from distutils.extension import Extension
from Cython.Distutils import build_ext

ext_modules=[
    Extension("demo",
              ["demo.pyx"],
              libraries=["../lib/grip-interface"],
              language="c++",
              # extra_objects=["../lib/libgrip-interface.a"],
    )
]

# import ipdb; ipdb.set_trace()
setup(
    name = "demo",
    cmdclass = {"build_ext": build_ext},
    ext_modules = ext_modules
)
