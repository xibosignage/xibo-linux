FROM docker-snapcraft

RUN mkdir -p /app

# Add sources and snap config
ADD ./player /app/player
ADD ./snap /app/snap

# Pull forward login file
COPY snapcraft.login /app/snapcraft.login

# Switch into /app to build the snap and push
WORKDIR /app

# Build snap
RUN snapcraft

# Push snap
RUN snapcraft login --with snapcraft.login && \
    snapcraft push *.snap

CMD []
