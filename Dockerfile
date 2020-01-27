FROM ubuntu:bionic

RUN mkdir -p /app
RUN apt-get update && apt-get install -y snapcraft

ENV LC_ALL C.UTF-8
ENV SNAPCRAFT_SETUP_CORE 1

# Add sources and snap config
ADD ./player /app/player
ADD ./snap /app/snap

# Pull forward login file
COPY snapcraft.login /app/snapcraft.login

# Switch into /app to build the snap and push
WORKDIR /app

RUN snapcraft --version
# Build snap
RUN snapcraft

# Push snap
RUN snapcraft login --with snapcraft.login && \
    snapcraft push *.snap

CMD []
