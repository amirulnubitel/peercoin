# Vertocoin Docker Setup

This directory contains Docker configuration for running Vertocoin in containerized environments.

## Quick Start

### Prerequisites

- Docker installed on your system
- Docker Compose installed on your system
- At least 4GB of available disk space
- At least 2GB of RAM

### Building and Running

1. **Build the Docker image:**
   ```bash
   ./docker-vertocoin.sh build
   ```

2. **Start the Vertocoin node:**
   ```bash
   ./docker-vertocoin.sh start
   ```

3. **Check the status:**
   ```bash
   ./docker-vertocoin.sh status
   ```

4. **View logs:**
   ```bash
   ./docker-vertocoin.sh logs -f
   ```

### Using Vertocoin CLI

Once the node is running, you can interact with it using the CLI:

```bash
# Get blockchain information
./docker-vertocoin.sh cli getblockchaininfo

# Get network information
./docker-vertocoin.sh cli getnetworkinfo

# Get wallet information (if wallet is enabled)
./docker-vertocoin.sh cli getwalletinfo

# Generate a new address
./docker-vertocoin.sh cli getnewaddress

# Check balance
./docker-vertocoin.sh cli getbalance
```

## Configuration

### Default Configuration

The Docker setup uses the `vertocoin.conf` file in the project root. Key settings include:

- **P2P Port**: 9333 (mapped to host)
- **RPC Port**: 9332 (mapped to host)
- **RPC User**: vertouser
- **RPC Password**: vertouser2025

### Custom Configuration

To modify the configuration:

1. Edit `vertocoin.conf` in the project root
2. Restart the container: `./docker-vertocoin.sh restart`

### Environment Variables

You can set additional environment variables in the `docker-compose.yml` file:

```yaml
environment:
  - VERTOCOIN_PRINTTOCONSOLE=1
  - VERTOCOIN_DATADIR=/home/vertocoin/.vertocoin
```

## Data Persistence

Blockchain data is stored in a Docker volume named `vertocoin_data`. This ensures your blockchain data persists between container restarts.

To backup your data:
```bash
docker run --rm -v vertocoin_data:/data -v $(pwd):/backup ubuntu tar czf /backup/vertocoin-backup.tar.gz -C /data .
```

To restore from backup:
```bash
docker run --rm -v vertocoin_data:/data -v $(pwd):/backup ubuntu tar xzf /backup/vertocoin-backup.tar.gz -C /data
```

## Network Ports

The following ports are exposed:

- **9333**: P2P network port (for blockchain communication)
- **9332**: RPC port (for API access)

### Security Considerations

The default configuration allows RPC connections from any IP (`rpcallowip=0.0.0.0/0`). For production use, you should:

1. Change the RPC password in `vertocoin.conf`
2. Restrict `rpcallowip` to specific IP ranges
3. Use a firewall to limit access to these ports
4. Consider using SSL/TLS for RPC connections

## Available Commands

The `docker-vertocoin.sh` script provides the following commands:

| Command | Description |
|---------|-------------|
| `build` | Build the Docker image |
| `start` | Start the Vertocoin node |
| `stop` | Stop the Vertocoin node |
| `restart` | Restart the Vertocoin node |
| `logs` | Show container logs |
| `status` | Show container and node status |
| `cli [args]` | Run vertocoin-cli commands |
| `shell` | Open a shell in the running container |
| `clean` | Remove containers and images |

## Troubleshooting

### Container won't start

1. Check logs: `./docker-vertocoin.sh logs`
2. Verify configuration: `cat vertocoin.conf`
3. Check disk space: `df -h`
4. Rebuild image: `./docker-vertocoin.sh clean && ./docker-vertocoin.sh build`

### RPC connection issues

1. Verify the container is running: `./docker-vertocoin.sh status`
2. Check if ports are accessible: `telnet localhost 9332`
3. Verify RPC credentials in `vertocoin.conf`

### Blockchain sync issues

1. Check network connectivity: `./docker-vertocoin.sh cli getnetworkinfo`
2. Check peer connections: `./docker-vertocoin.sh cli getpeerinfo`
3. Verify blockchain sync: `./docker-vertocoin.sh cli getblockchaininfo`

### Performance optimization

For better performance:

1. Allocate more RAM to Docker (4GB+ recommended)
2. Use SSD storage for better I/O performance
3. Ensure good network connectivity
4. Consider using `txindex=0` if you don't need full transaction indexing

## Development

### Building with custom options

To build with different configure options, modify the Dockerfile:

```dockerfile
RUN ./configure \
    --disable-tests \
    --disable-bench \
    --with-gui \        # Enable GUI
    --enable-wallet \   # Enable wallet functionality
    --enable-hardening
```

### Adding monitoring

You can extend the `docker-compose.yml` to add monitoring services like Prometheus or Grafana for better observability.

## Support

For issues specific to the Docker setup, please check:

1. Docker logs: `./docker-vertocoin.sh logs`
2. Container status: `./docker-vertocoin.sh status`
3. System resources: `docker system df`

For Vertocoin-specific issues, refer to the main project documentation.
