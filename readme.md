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

## Clone the repository
```bash
git clone --recurse-submodules https://github.com/jerily/tbert.git
cd tbert
TBERT_DIR=$(pwd)
```

## Build and install the dependencies
```bash
cd ${TBERT_DIR}/bert.cpp
mkdir build
cd build
cmake .. -DBUILD_SHARED_LIBS=ON -DCMAKE_BUILD_TYPE=Release
make
make install
```

## Build the library
For TCL:
```bash
# Build the TCL extension
cd ${TBERT_DIR}
mkdir build
cd build
cmake ..
make
make install
tclsh ../example.tcl /path/to/models/all-MiniLM-L12-v2/ggml-model-q4_0.bin
```

For NaviServer using Makefile:
```
cd ${TBERT_DIR}
make
make install
```

## Documentation

See [Semantic Search with tBERT](doc/semantic-search-with-tbert.md) for an introduction to semantic search using tBERT.