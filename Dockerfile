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
   libdb5.3-dev \
   libdb5.3++-dev \
   libminiupnpc-dev \
   libzmq3-dev \
   libqt5gui5 \
   libqt5core5a \
   libqt5dbus5 \
   qttools5-dev \
   qttools5-dev-tools \
   libprotobuf-dev \
   protobuf-compiler \
   libqrencode-dev \
   git \
   curl \
   wget \
   && rm -rf /var/lib/apt/lists/*

# Set working directory
WORKDIR /vertocoin

# Copy source code
COPY . .

# Make build script executable
RUN chmod +x build_vertocoin.sh

# Use existing configure script (skip autogen.sh)
# RUN ./autogen.sh

# Configure build
RUN ./configure \
   --disable-tests \
   --disable-bench \
   --without-gui \
   --with-daemon \
   --enable-hardening \
   --disable-fuzz-binary

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
   libminiupnpc17 \
   libzmq5 \
   libevent-2.1-7 \
   libevent-pthreads-2.1-7 \
   libssl3 \
   && rm -rf /var/lib/apt/lists/*

# Create vertocoin user
RUN useradd -r -m -s /bin/bash vertocoin

# Create data directory
RUN mkdir -p /home/vertocoin/.vertocoin && \
   chown -R vertocoin:vertocoin /home/vertocoin/.vertocoin

# Copy binaries from builder stage
COPY --from=builder /vertocoin/src/vertocoind /usr/local/bin/
COPY --from=builder /vertocoin/src/vertocoin-cli /usr/local/bin/
COPY --from=builder /vertocoin/src/vertocoin-tx /usr/local/bin/
COPY --from=builder /vertocoin/src/vertocoin-util /usr/local/bin/
COPY --from=builder /vertocoin/src/vertocoin-wallet /usr/local/bin/

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
