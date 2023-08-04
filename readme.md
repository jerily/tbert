# tbert

TCL/C extension for computing BERT vector embeddings

## Tcl Commands
```
# Load model from file.
# ::tbert::load_model model_name model_file
::tbert::load_model mymodel "./models/all-MiniLM-L12-v2/ggml-model-q4_0.bin"

# Compute the BERT vector embedding for a given text.
# ::tbert::ev model_name text
::tbert::ev mymodel "This is a test."

# Unload model
# ::tbert::unload_model model_name
::tbert::unload_model mymodel
```

## Try it out using docker
```
git clone --recurse-submodules https://github.com/jerily/tbert.git
cd tbert
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

Or, download a model from huggingface:
```
git lfs install
git clone https://huggingface.co/skeskinen/ggml
```

## Build the library
For TCL:
```
git clone --recurse-submodules git@github.com:jerily/tbert.git
cd tbert
mkdir build
cd build
cmake .. -DBUILD_SHARED_LIBS=ON -DCMAKE_BUILD_TYPE=Release
make
make install
tclsh ../example.tcl /path/to/models/all-MiniLM-L12-v2/ggml-model-q4_0.bin
```

For NaviServer using Makefile:
```
git clone --recurse-submodules git@github.com:jerily/tbert.git
cd tbert/bert.cpp
mkdir build
cd build
cmake .. -DBUILD_SHARED_LIBS=ON -DCMAKE_BUILD_TYPE=Release
make
make install
cd ..
make
make install
```
