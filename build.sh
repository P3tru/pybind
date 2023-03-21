# prerequisites

# apt-get install python3-dev
# pip install pybind11

c++ -O3 -Wall -shared -std=c++17 -fPIC -mavx2 $(python3 -m pybind11 --includes) fastp2i.cpp -o fastp2i$(python3-config --extension-suffix)