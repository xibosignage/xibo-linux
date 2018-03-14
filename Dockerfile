FROM ubuntu:16.04
MAINTAINER Alex Harrington <alex@springsignage.com>

RUN DEBIAN_FRONTEND=noninteractive apt-get update && apt-get install -y \
  software-properties-common \
  cmake \
  libgtkmm-3.0-dev \
  libgstreamer1.0-dev \
  libgstreamer-plugins-base1.0-dev \
  libgstreamermm-1.0-dev \
  libwebkitgtk-3.0-dev \
  libboost-filesystem1.58-dev \
  libboost-program-options1.58-dev \
  libgstreamer-plugins-bad1.0-dev \
  gstreamer1.0-tools \
  mesa-common-dev \
  libgl1-mesa-dev \
  libgles2-mesa-dev \
  build-essential \
  autoconf \
  autogen \
  libtool \
  curl \
  libxml-parser-perl \
  git

ENV MMCOMMON=0.9.10 GLIBMM=2.47.6 GSTREAMERMM=1.8.0 SPDLOG=0.16.3

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

RUN ACLOCAL_PATH="/usr/local/share/aclocal" && \
    export ACLOCAL_PATH && \
    curl -o /root/gstreamermm.tar.gz -SL https://github.com/GNOME/gstreamermm/archive/${GSTREAMERMM}.tar.gz && \
    cd /root && \
    tar -zxvf gstreamermm.tar.gz && \
    cd gstreamermm-${GSTREAMERMM} && \
    ./autogen.sh && \
    make && \
    make install && \
    cd /root && \
    rm -r /root/gstreamermm-${GSTREAMERMM} && \
    rm /root/gstreamermm.tar.gz

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

RUN mkdir -p /app

ADD . /app

RUN cd /app && \
    cmake -H. -B_build && \
    cd _build && \
    make && \
    make install && \
    ls -la /app/_build

VOLUME /output
CMD cp -rv /app/_build /output