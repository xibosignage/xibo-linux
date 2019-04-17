FROM ubuntu:16.04
MAINTAINER Alex Harrington <alex@springsignage.com>

RUN DEBIAN_FRONTEND=noninteractive apt-get update && apt-get install -y software-properties-common

RUN DEBIAN_FRONTEND=noninteractive add-apt-repository ppa:ubuntu-toolchain-r/test && apt-get update

RUN DEBIAN_FRONTEND=noninteractive apt-get install -y \
  software-properties-common \
  cmake

RUN DEBIAN_FRONTEND=noninteractive apt-get install -y \
  libgtkmm-3.0-dev \
  libgstreamer1.0-dev \
  libgstreamer-plugins-base1.0-dev \
  libgstreamer-plugins-bad1.0-dev \
  gstreamer1.0-libav \
  libwebkitgtk-3.0-dev \
  gstreamer1.0-tools

RUN DEBIAN_FRONTEND=noninteractive apt-get install -y \
  mesa-common-dev \
  libgl1-mesa-dev \
  libgles2-mesa-dev \
  build-essential \
  autoconf \
  autogen \
  libtool

RUN DEBIAN_FRONTEND=noninteractive apt-get install -y \
  curl \
  libxml-parser-perl \
  git \
  g++-8 \
  libffi-dev \
  libmount-dev \
  libssl-dev \
  unzip

RUN update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-8 90
RUN update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-8 90

ENV MMCOMMON=0.9.10
RUN curl -o /root/mm-common.tar.gz -SL https://github.com/GNOME/mm-common/archive/${MMCOMMON}.tar.gz && \
    cd /root && \
    tar -zxvf mm-common.tar.gz && \
    cd mm-common-${MMCOMMON} && \
    ./autogen.sh && \
    make && \
    make install && \
    cd /root && \
    rm -r /root/mm-common-${MMCOMMON} && \
    rm /root/mm-common.tar.gz

ENV SIGC=2.10.0
RUN ACLOCAL_PATH="/usr/local/share/aclocal" && \
    export ACLOCAL_PATH && \
    curl -o /root/libsigcplusplus.tar.gz -SL https://github.com/libsigcplusplus/libsigcplusplus/archive/${SIGC}.tar.gz && \
    cd /root && \
    tar -zxvf libsigcplusplus.tar.gz && \
    cd libsigcplusplus-${SIGC} && \
    ./autogen.sh && \
    make && \
    make install && \
    cd /root && \
    rm -r /root/libsigcplusplus-${SIGC} && \
    rm /root/libsigcplusplus.tar.gz

ENV GLIB=2.56.1
RUN curl -o /root/glib.tar.gz -SL https://github.com/GNOME/glib/archive/${GLIB}.tar.gz && \
    cd /root && \
    tar -zxvf glib.tar.gz && \
    cd glib-${GLIB} && \
    ./autogen.sh && \
    make && \
    make install && \
    cd /root && \
    rm -r /root/glib-${GLIB} && \
    rm /root/glib.tar.gz

ENV GLIBMM=2.56.0
RUN ACLOCAL_PATH="/usr/local/share/aclocal" && \
    export ACLOCAL_PATH && \
    curl -o /root/glibmm.tar.gz -SL https://github.com/GNOME/glibmm/archive/${GLIBMM}.tar.gz && \
    cd /root && \
    tar -zxvf glibmm.tar.gz && \
    cd glibmm-${GLIBMM} && \
    ./autogen.sh && \
    make && \
    make install && \
    cd /root && \
    rm -r /root/glibmm-${GLIBMM} && \
    rm /root/glibmm.tar.gz

ENV BOOST_MAJOR=1 BOOST_MINOR=69
ENV BOOST=${BOOST_MAJOR}_${BOOST_MINOR}_0 
RUN curl -o /root/boost.tar.gz -SL https://dl.bintray.com/boostorg/release/${BOOST_MAJOR}.${BOOST_MINOR}.0/source/boost_${BOOST}.tar.gz && \
    cd /root && \
    tar -zxvf boost.tar.gz && \
    cd boost_${BOOST} && \
    ./bootstrap.sh --with-libraries=system,date_time,thread && \
    ./b2 install && \
    cd /root && \
    rm -r /root/boost_${BOOST} && \
    rm /root/boost.tar.gz

ENV SPDLOG=1.1.0
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

ENV ZEROMQ=4.3.0
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

RUN curl -o /root/cppzmq.tar.gz -SL https://github.com/zeromq/cppzmq/archive/v${ZEROMQ}.tar.gz && \
    cd /root && \
    tar -zxvf cppzmq.tar.gz && \
    cd cppzmq-${ZEROMQ} && \
    mkdir build && \
    cd build && \
    cmake .. && \
    make -j4 install && \
    cd /root && \
    rm -r cppzmq-${ZEROMQ} && \
    rm cppzmq.tar.gz

ENV CRYPTOPP=8_1_0
RUN curl -o /root/cryptopp.tar.gz -SL https://github.com/weidai11/cryptopp/archive/CRYPTOPP_${CRYPTOPP}.tar.gz && \
	curl -o /root/cryptopp_pem.zip -SL https://github.com/noloader/cryptopp-pem/archive/master.zip && \
    cd /root && \
    tar -zxvf cryptopp.tar.gz && \
    unzip -a cryptopp_pem.zip && \
    cp -r /root/cryptopp-pem-master/. /root/cryptopp-CRYPTOPP_${CRYPTOPP} && \
    cd cryptopp-CRYPTOPP_${CRYPTOPP} && \
    make -j4 && \
    make install && \
    cd /root && \
    rm -r cryptopp-CRYPTOPP_${CRYPTOPP} && \
    rm -r cryptopp-pem-master && \
    rm cryptopp.tar.gz && \
    rm cryptopp_pem.zip

RUN mkdir -p /app

ADD . /app

RUN cd /app && \
    cmake -H. -B_build && \
    cd _build && \
    make -j4 && \
    make install

VOLUME /build
ENTRYPOINT ["./app/run.sh"]
CMD []
