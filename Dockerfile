# Vertocoin Dockerfile
# Multi-stage build for optimized image size

# Build stage
FROM ubuntu:22.04 AS builder

# Avoid interactive prompts during package installation
ENV DEBIAN_FRONTEND=noninteractive

# Install build dependencies
RUN apt-get update && apt-get install -y \
   build-essential \
   libtool \
   autotools-dev \
   automake \
   autoconf \
   pkg-config \
   libssl-dev \
   libevent-dev \
   bsdmainutils \
   python3 \
   python3-pip \
   libboost-all-dev \
   libminiupnpc-dev \
   libdb5.3-dev \
   libdb5.3++-dev \
   db5.3-util \
   libzmq3-dev \
   libqt5gui5 \
   libqt5core5a \
   libqt5dbus5 \
   qttools5-dev \
   ccache \
   qttools5-dev-tools \
   libprotobuf-dev \
   protobuf-compiler \
   libqrencode-dev \
   libcapnp-dev \
   capnproto \
   libnatpmp-dev \
   libupnp-dev \
   libsqlite3-dev \
   libevent-pthreads-2.1-7 \
   git \
   curl \
   wget \
   cmake \
   && rm -rf /var/lib/apt/lists/*

# Set working directory
WORKDIR /vertocoin

# Copy source code
COPY . .

# Make build script executable
RUN chmod +x build_vertocoin.sh

# Generate configure script
RUN ./autogen.sh

# Configure build
RUN ./configure \
   --disable-tests \
   --disable-bench \
   --without-miniupnpc \
   --enable-cxx \
   --disable-shared \
   --with-pic \
   --without-qrencode \
   --without-gui \
   --disable-qt \
   --enable-zmq \
   --enable-wallet \
   --enable-ccache \
   --enable-upnp-default \
   --with-incompatible-bdb \
   BDB_LIBS="-ldb_cxx-5.3" \
   BDB_CFLAGS="-I/usr/include/db5.3"

# Build Vertocoin
RUN make -j$(nproc)

# Runtime stage
FROM ubuntu:22.04

# Install runtime dependencies
RUN apt-get update && apt-get install -y \
   libboost-system1.74.0 \
   libboost-filesystem1.74.0 \
   libboost-thread1.74.0 \
   libboost-chrono1.74.0 \
   libdb5.3++ \
   libzmq5 \
   libevent-2.1-7 \
   libevent-pthreads-2.1-7 \
   libssl3 \
   libnatpmp1 \
   libupnp13 \
   libsqlite3-0 \
   && rm -rf /var/lib/apt/lists/*

# Copy miniupnpc library from builder stage
COPY --from=builder /usr/local/lib/libminiupnpc.* /usr/local/lib/
RUN ldconfig

# Create vertocoin user
RUN useradd -r -m -s /bin/bash vertocoin

# Create data directory
RUN mkdir -p /home/vertocoin/.vertocoin && \
   chown -R vertocoin:vertocoin /home/vertocoin/.vertocoin

# Copy binaries from builder stage
COPY --from=builder /vertocoin/src/peercoind /usr/local/bin/vertocoind
COPY --from=builder /vertocoin/src/peercoin-cli /usr/local/bin/vertocoin-cli
COPY --from=builder /vertocoin/src/peercoin-tx /usr/local/bin/vertocoin-tx
COPY --from=builder /vertocoin/src/peercoin-util /usr/local/bin/vertocoin-util
COPY --from=builder /vertocoin/src/peercoin-wallet /usr/local/bin/vertocoin-wallet

# Copy configuration file
COPY --chown=vertocoin:vertocoin vertocoin.conf /home/vertocoin/.vertocoin/

# Make binaries executable
RUN chmod +x /usr/local/bin/vertocoin*

# Switch to vertocoin user
USER vertocoin
WORKDIR /home/vertocoin

# Expose ports
EXPOSE 9333 9332

# Health check
HEALTHCHECK --interval=30s --timeout=10s --start-period=60s --retries=3 \
   CMD vertocoin-cli getblockchaininfo || exit 1

# Default command
CMD ["vertocoind", "-datadir=/home/vertocoin/.vertocoin", "-conf=/home/vertocoin/.vertocoin/vertocoin.conf"]
