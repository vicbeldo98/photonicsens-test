# Compile CPP file specifying library and output name:

g++ utilities.CPP `pkg-config --libs --cflags opencv4` -fopenmp -o out
