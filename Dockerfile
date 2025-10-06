#
# Ubuntu
#
FROM ubuntu:noble as base
ARG DEBIAN_FRONTEND=noninteractive
RUN apt update && apt -y --no-install-recommends install \
        make \
        ca-certificates \
        cmake \
        antlr4 \
        libantlr4-runtime-dev \
        libboost-all-dev \
        m4 \
        uuid \
        uuid-dev \
        python3 \
        python3-pip \
        python3-venv \
        pkg-config \
        clang-17 \
        libc++1-17 \
        libc++-17-dev \
        libc++abi1-17 \
        libc++abi-17-dev \
        libgtest-dev \
        git \
        z3 \
        gdb \
        build-essential  \
        lcov  \
        gcovr  \
        xdg-utils \
        zsh

#setup clang
RUN ln -s /usr/bin/clang-17 /usr/bin/clang
RUN ln -s /usr/bin/clang++-17 /usr/bin/clang++
ENV CC clang
ENV CXX clang++

#
# Google Test
#
WORKDIR /usr/src/gtest
RUN cmake CMakeLists.txt
RUN make
RUN cp lib/*.a /usr/lib

#
# MATA
#
WORKDIR /mata
RUN git clone https://github.com/VeriFIT/mata .
RUN make release-lib
RUN make install
#
# CVC5
# commit hash version: 5a10c1778600189ae31e41ddab3a889580ed19ae
#
WORKDIR /cvc5
RUN git clone https://github.com/cvc5/cvc5 .
RUN git reset --hard 5a10c1778600189ae31e41ddab3a889580ed19ae
RUN python3 -m venv ./venv
RUN venv/bin/python -m pip install pyparsing
RUN venv/bin/python -m pip install tomli
RUN ./configure.sh --auto-download -DPython_EXECUTABLE=/cvc5/venv/bin/python
WORKDIR /cvc5/build
RUN make -j$((`nproc`+1))
RUN make check
RUN make install

#
# Tool
#
COPY . /artifact/synth
WORKDIR /artifact/synth/build
RUN cmake .. -DCMAKE_BUILD_TYPE=Release
RUN make -j$((`nproc`+1))
WORKDIR /artifact/synth/build/_deps/z3-build
RUN make install

##
## Start
##
WORKDIR /artifact/synth
ENTRYPOINT [ "/bin/zsh" ]
