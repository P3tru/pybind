from setuptools import setup, Extension
import pybind11
import sys

cpp_args = ['-std=c++17']

fastp2i_module = Extension(
    'fastp2i',
    sources=['fastp2i.cpp'],
    include_dirs=[pybind11.get_include()],
    language='c++',
    extra_compile_args=cpp_args,
    )

setup(
    name='fastp2i',
    version='1.0',
    description='Python package with fastp2i C++ extension (PyBind11)',
    ext_modules=[fastp2i_module],
    pgo={
        "profile_command": [sys.executable, 'profile.py'],
    },
)