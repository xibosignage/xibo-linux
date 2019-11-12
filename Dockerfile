FROM ubuntu:18.04
MAINTAINER Alex Harrington <alex@xibosignage.com>

ENV TZ=Europe/Kiev
RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone

RUN DEBIAN_FRONTEND=noninteractive apt-get update && apt-get install -y apt-transport-https ca-certificates gnupg software-properties-common wget

RUN DEBIAN_FRONTEND=noninteractive add-apt-repository ppa:ubuntu-toolchain-r/test && apt-get update

RUN DEBIAN_FRONTEND=noninteractive apt-get update && apt-get install -y \
  libgtkmm-3.0-dev \
  libgstreamer1.0-dev \
  libgstreamer-plugins-base1.0-dev \
  libwebkitgtk-3.0-dev \
  libxss-dev \
  libglibmm-2.4-dev \
  curl \
  g++-8 \
  libssl-dev \
  unzip

RUN DEBIAN_FRONTED=noninteractive wget -O - https://apt.kitware.com/keys/kitware-archive-latest.asc 2>/dev/null | apt-key add - && \
  apt-add-repository 'deb https://apt.kitware.com/ubuntu/ bionic main' && \
  apt-get update && \
  apt-get install -y cmake

RUN DEBIAN_FRONTED=noninteractive sh -c "echo 'deb http://download.opensuse.org/repositories/network:/messaging:/zeromq:/release-stable/xUbuntu_18.04/ /' > /etc/apt/sources.list.d/network:messaging:zeromq:release-stable.list" && \
  wget -nv https://download.opensuse.org/repositories/network:messaging:zeromq:release-stable/xUbuntu_18.04/Release.key -O Release.key && \
  apt-key add - < Release.key && \
  apt-get update && \
  apt-get install -y libzmq3-dev

RUN DEBIAN_FRONTED=noninteractive add-apt-repository ppa:mhier/libboost-latest && apt-get update && apt-get install -y libboost1.70-dev

RUN update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-8 90
RUN update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-8 90

ENV SPDLOG=1.4.1
RUN curl -o /root/spdlog.tar.gz -SL https://github.com/gabime/spdlog/archive/v${SPDLOG}.tar.gz && \
    cd /root && \
    tar -zxvf spdlog.tar.gz && \
    cd spdlog-${SPDLOG} && \
    cmake . && \
    make -j4 && \
    make install && \
    cd /root && \
    rm -r spdlog-${SPDLOG} && \
    rm spdlog.tar.gz

ENV GTEST=1.8.1
RUN curl -o /root/gtest.tar.gz https://codeload.github.com/google/googletest/tar.gz/release-${GTEST} && \
    cd /root && \
    tar -zxvf gtest.tar.gz && \
    cd googletest-release-${GTEST} && \
    cmake . && \
    make -j4 && \
    make install && \
    cd /root && \
    rm -r googletest-release-${GTEST} && \
    rm gtest.tar.gz

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
ADD player /app

RUN cd /app && \
    cmake -DCMAKE_BUILD_TYPE=Release -DSNAP_BUILD=ON -H. -B_build && \
    cd _build && \
    make -j4

ENTRYPOINT cp -r /app/_build/. /build
