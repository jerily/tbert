```
git clone --recurse-submodules git@github.com:jerily/tbert.git
cd tbert
mkdir build
cd build
cmake .. -DBUILD_SHARED_LIBS=ON -DCMAKE_BUILD_TYPE=Release
make
tclsh ../example.tcl /path/to/models/all-MiniLM-L12-v2/ggml-model-q4_0.bin
```