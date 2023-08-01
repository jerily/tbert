FROM ubuntu:latest
ENV TZ=UTC
RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone
RUN apt update && apt install -y \
    python3 \
    python3-pip \
    python3-dev \
    build-essential \
    libssl-dev \
    libffi-dev \
    libxml2-dev \
    libxslt1-dev \
    zlib1g-dev \
    cmake \
    git \
    vim \
    curl \
    wget \
    unzip \
    && rm -rf /var/lib/apt/lists/*

RUN pip3 install --upgrade pip
RUN pip3 install --upgrade setuptools
RUN pip3 install --upgrade wheel
RUN pip3 install --upgrade virtualenv
RUN pip3 install --upgrade virtualenvwrapper

RUN cd /usr/local/src && \
    wget http://prdownloads.sourceforge.net/tcl/tcl8.6.13-src.tar.gz && \
    tar xzf tcl8.6.13-src.tar.gz && \
    cd tcl8.6.13/unix && \
    ./configure --enable-threads && \
    make && \
    make install

RUN git clone --recurse-submodules https://github.com/jerily/tbert.git

RUN cd tbert && \
    cd bert.cpp && \
    pip3 install -r requirements.txt

RUN cd tbert && \
    cd bert.cpp && \
    cd models && \
    python3 download-ggml.py download all-MiniLM-L12-v2 q4_0 && \
    ls -la

RUN cd tbert && mkdir build && cd build && \
    cmake .. -DBUILD_SHARED_LIBS=ON -DCMAKE_BUILD_TYPE=Release && \
    make && \
    make install

ENV LD_LIBRARY_PATH="$LD_LIBRARY_PATH:/usr/local/lib"

