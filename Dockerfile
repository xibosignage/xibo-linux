FROM mcr.microsoft.com/vscode/devcontainers/cpp:0-ubuntu-20.04

RUN apt update -y && apt install -y \
  pkgconf \
  libgtkmm-3.0-dev \
  libsqlite3-dev \
  git

RUN git clone https://github.com/fnc12/sqlite_orm.git sqlite_orm \
  && cd sqlite_orm \
  && cmake -B build \
  && cmake --build build --target install

RUN apt install -y \
  libgstreamer1.0-dev \
  libgstreamer-plugins-base1.0-dev \
  libgstreamer-plugins-base1.0-0 \
  libspdlog-dev \
  libssl-dev \
  libzmq3-dev \
  libgtest-dev

ENV DEBIAN_FRONTEND=noninteractive

RUN echo "deb http://cz.archive.ubuntu.com/ubuntu bionic main universe" >> /etc/apt/sources.list \
  && apt update -y \
  && apt install -y libwebkitgtk-3.0-dev

RUN apt install -y software-properties-common \
  && add-apt-repository ppa:mhier/libboost-latest \
  && apt-get install -y libboost1.70-dev

RUN apt install -y \
  libcrypto++-dev \
  libgmock-dev

RUN curl -o date-tz.tar.gz -SL https://github.com/HowardHinnant/date/archive/v3.0.0.tar.gz \
    && tar -zxvf date-tz.tar.gz \
    && cd date-3.0.0 \
    && cmake . -DBUILD_TZ_LIB=ON -DBUILD_SHARED_LIBS=ON -DUSE_SYSTEM_TZ_DB=ON \
    && make -j4 \
    && make install

RUN curl -o cryptopp.tar.gz -SL https://github.com/weidai11/cryptopp/archive/CRYPTOPP_8_1_0.tar.gz \
  && curl -o cryptopp_pem.zip -SL https://github.com/noloader/cryptopp-pem/archive/095f08ff2ef9bca7b81036a59f2395e4f08ce2e8.zip \
  && tar -zxvf cryptopp.tar.gz \
  && unzip -a cryptopp_pem.zip \
  && cp -r cryptopp-pem-095f08ff2ef9bca7b81036a59f2395e4f08ce2e8/. cryptopp-CRYPTOPP_8_1_0 \
  && cd cryptopp-CRYPTOPP_8_1_0 \
  && make -j4 \
  && make install

RUN apt update -y --fix-missing \
  && apt install -y packagekit-gtk3-module
