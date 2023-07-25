## Try it out using docker
```
git clone --recurse-submodules git@github.com:jerily/tbert.git
docker build . -t tbert:latest
docker run --rm -it --entrypoint bash tbert:latest
```

Inside the container:
```
cd tbert/build
tclsh8.6 ../example.tcl ../bert.cpp/models/all-MiniLM-L12-v2/ggml-model-q4_0.bin
```

Copy the model from the container:
```
id=$(docker create tbert:latest)
docker cp $id:/tbert/bert.cpp/models/all-MiniLM-L12-v2/ggml-model-q4_0.bin .
docker rm -v $id
ls -la ggml-model-q4_0.bin
```

## Build the library
```
git clone --recurse-submodules git@github.com:jerily/tbert.git
cd tbert
mkdir build
cd build
cmake .. -DBUILD_SHARED_LIBS=ON -DCMAKE_BUILD_TYPE=Release
make
tclsh ../example.tcl /path/to/models/all-MiniLM-L12-v2/ggml-model-q4_0.bin
```