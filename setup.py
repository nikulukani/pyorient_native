from distutils.core import setup, Extension
pyorient_native = Extension("pyorient_native", ["orientc_reader.cpp", "orientc_writer.cpp",
                                     "helpers.cpp", "parse_exception.cpp",
                                     "listener.cpp", "encoder.cpp",
                                     "pyorient_native.cpp"],
                    include_dirs = [],
                    library_dirs = [],
                    language="c++", libraries=["stdc++"])
setup(
    name = "pyorient_native",
    version="1.0",
    description="OrientDB Binary Serialization package for python",
    author="Nikul Ukani",
    author_email="nhu2001@columbia.edu",
    ext_modules = [pyorient_native]
    )
