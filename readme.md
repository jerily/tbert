```
mkdir repos
cd repos

tar xzvf tbert.tar.gz
tar xvf models.tar

git clone https://github.com/skeskinen/bert.cpp.git
git submodule update --init --recursive

cd bert.cpp
git apply ../tbert/neo-2023-07-24.bert_cpp.patch
mkdir build
cd build
cmake .. -DBUILD_SHARED_LIBS=ON -DCMAKE_BUILD_TYPE=Release
make
cd ..

cd tbert
cmake .
make
tclsh example.tcl ../models/all-MiniLM-L12-v2/ggml-model-q4_0.bin
```