FROM ubuntu:18.04
MAINTAINER Alex Harrington <alex@xibosignage.com>

RUN DEBIAN_FRONTEND=noninteractive apt-get update && apt-get install -y software-properties-common

RUN DEBIAN_FRONTEND=noninteractive add-apt-repository ppa:ubuntu-toolchain-r/test && apt-get update

RUN DEBIAN_FRONTEND=noninteractive apt-get update && apt-get install -y \
  software-properties-common \
  cmake

ENV TZ=Europe/Kiev
RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone

RUN DEBIAN_FRONTEND=noninteractive apt-get update && apt-get install -y \
  libgtkmm-3.0-dev \
  libgstreamer1.0-dev \
  libgstreamer-plugins-base1.0-dev \
  libgstreamer-plugins-bad1.0-dev \
  gstreamer1.0-libav \
  libwebkitgtk-3.0-dev \
  gstreamer1.0-tools \
  libxss-dev \
  libglibmm-2.4-dev

RUN DEBIAN_FRONTEND=noninteractive apt-get update && apt-get install -y \
  mesa-common-dev \
  libgl1-mesa-dev \
  libgles2-mesa-dev \
  build-essential \
  autoconf \
  autogen \
  libtool

RUN DEBIAN_FRONTEND=noninteractive apt-get update && apt-get install -y \
  curl \
  libxml-parser-perl \
  git \
  g++-8 \
  libffi-dev \
  libmount-dev \
  libssl-dev \
  unzip \
  libsigc++-2.0-dev

RUN update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-8 90
RUN update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-8 90

ENV BOOST_MAJOR=1 BOOST_MINOR=70
ENV BOOST=${BOOST_MAJOR}_${BOOST_MINOR}_0 
RUN curl -o /root/boost.tar.gz -SL https://dl.bintray.com/boostorg/release/${BOOST_MAJOR}.${BOOST_MINOR}.0/source/boost_${BOOST}.tar.gz && \
    cd /root && \
    tar -zxvf boost.tar.gz && \
    cd boost_${BOOST} && \
    ./bootstrap.sh --with-libraries=system,date_time,thread,filesystem && \
    ./b2 install && \
    cd /root && \
    rm -r /root/boost_${BOOST} && \
    rm /root/boost.tar.gz

ENV SPDLOG=1.4.1
RUN curl -o /root/spdlog.tar.gz -SL https://github.com/gabime/spdlog/archive/v${SPDLOG}.tar.gz && \
    cd /root && \
    tar -zxvf spdlog.tar.gz && \
    cd spdlog-${SPDLOG} && \
    cmake . && \
    make && \
    make install && \
    cd /root && \
    rm -r spdlog-${SPDLOG} && \
    rm spdlog.tar.gz

ENV CMAKE_MAJOR=3 CMAKE_MINOR=14 CMAKE_PATCH=1
ENV CMAKE=${CMAKE_MAJOR}.${CMAKE_MINOR}.${CMAKE_PATCH}
RUN curl -o /root/cmake.tar.gz https://cmake.org/files/v${CMAKE_MAJOR}.${CMAKE_MINOR}/cmake-${CMAKE}.tar.gz && \
    cd /root && \
    tar -zxvf cmake.tar.gz && \
    cd cmake-${CMAKE} && \
    cmake . && \
    make && \
    make install && \
    cd /root && \
    rm -r cmake-${CMAKE} && \
    rm cmake.tar.gz

ENV GTEST=1.8.1
RUN curl -o /root/gtest.tar.gz https://codeload.github.com/google/googletest/tar.gz/release-${GTEST} && \
    cd /root && \
    tar -zxvf gtest.tar.gz && \
    cd googletest-release-${GTEST} && \
    cmake . && \
    make && \
    make install && \
    cd /root && \
    rm -r googletest-release-${GTEST} && \
    rm gtest.tar.gz

ENV ZEROMQ=4.3.2
RUN curl -o /root/zeromq.tar.gz -SL https://github.com/zeromq/libzmq/releases/download/v${ZEROMQ}/zeromq-${ZEROMQ}.tar.gz && \
    cd /root && \
    tar -zxvf zeromq.tar.gz && \
    cd zeromq-${ZEROMQ} && \
    mkdir build && \
    cd build && \
    cmake .. && \
    make -j4 install && \
    cd /root && \
    rm -r zeromq-${ZEROMQ} && \
    rm zeromq.tar.gz

ENV CRYPTOPP=8_1_0
ENV CRYPTOPP_PEM=095f08ff2ef9bca7b81036a59f2395e4f08ce2e8
RUN curl -o /root/cryptopp.tar.gz -SL https://github.com/weidai11/cryptopp/archive/CRYPTOPP_${CRYPTOPP}.tar.gz && \
	curl -o /root/cryptopp_pem.zip -SL https://github.com/noloader/cryptopp-pem/archive/${CRYPTOPP_PEM}.zip && \
    cd /root && \
    tar -zxvf cryptopp.tar.gz && \
    unzip -a cryptopp_pem.zip && \
    cp -r /root/cryptopp-pem-${CRYPTOPP_PEM}/. /root/cryptopp-CRYPTOPP_${CRYPTOPP} && \
    cd cryptopp-CRYPTOPP_${CRYPTOPP} && \
    make -j4 && \
    make install && \
    cd /root && \
    rm -r cryptopp-CRYPTOPP_${CRYPTOPP} && \
    rm -r cryptopp-pem-095f08ff2ef9bca7b81036a59f2395e4f08ce2e8 && \
    rm cryptopp.tar.gz && \
    rm cryptopp_pem.zip

RUN mkdir -p /app

VOLUME /build
ADD . /app

RUN cd /app && \
    cmake -DCMAKE_BUILD_TYPE=Release -H. -B_build && \
    cd _build && \
    make -j4 && \
    make install

ENTRYPOINT cp -r /app/_build/. /build
