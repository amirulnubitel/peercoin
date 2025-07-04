# Vertocoin Setup Guide

This guide will help you set up and launch your Vertocoin cryptocurrency based on the Peercoin codebase.

## Key Parameters

- **Coin Name**: Vertocoin
- **Currency Symbol**: VTO
- **Maximum Supply**: 50 billion VTO
- **Initial Supply**: 5 billion VTO (allocated to genesis block)
- **Block Time**: 1 minute (high-speed transfers)
- **Consensus**: Proof-of-Stake
- **P2P Port**: 9333
- **RPC Port**: 9332
- **Address Prefix**: V (for mainnet addresses)

## Prerequisites

### macOS (with Homebrew)

```bash
# Install Homebrew if not already installed
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"

# Install dependencies
brew install automake berkeley-db4 libtool boost miniupnpc openssl pkg-config protobuf python3 qt5 zmq libevent
```

### Ubuntu/Debian

```bash
sudo apt update
sudo apt install build-essential libtool autotools-dev automake pkg-config libssl-dev libevent-dev bsdmainutils
sudo apt install libboost-all-dev libdb4.8-dev libdb4.8++-dev libminiupnpc-dev libzmq3-dev
```

## Building Vertocoin

1. **Clone or use the modified repository**:

   ```bash
   cd /Users/amirul/Desktop/b/peercoin
   ```

2. **Run the build script**:

   ```bash
   ./build_vertocoin.sh
   ```

3. **Alternative manual build**:
   ```bash
   ./autogen.sh
   ./configure --enable-cxx --disable-shared --with-pic --prefix=/usr/local
   make -j$(nproc)
   ```

## Important: Genesis Block Mining

Before running the daemon, you MUST mine a new genesis block:

1. **Compile the genesis mining utility**:

   ```bash
   cd src
   g++ -o genesis_mine genesis_mine.cpp -I. -std=c++17 -lcrypto
   ```

2. **Mine the genesis block**:

   ```bash
   ./genesis_mine
   ```

3. **Update chainparams.cpp** with the new genesis hash and merkle root from the mining output

4. **Rebuild the project**:
   ```bash
   make clean && make -j$(nproc)
   ```

## Configuration

1. **Create data directory**:

   ```bash
   mkdir -p ~/.vertocoin
   ```

2. **Copy configuration file**:

   ```bash
   cp vertocoin.conf.example ~/.vertocoin/vertocoin.conf
   ```

3. **Edit configuration**:

   ```bash
   nano ~/.vertocoin/vertocoin.conf
   ```

   Update the following:

   - `rpcuser` and `rpcpassword` for security
   - Add your seed nodes
   - Configure other settings as needed

## Running Vertocoin

### Start the daemon:

```bash
./src/vertocoind -daemon
```

### Check status:

```bash
./src/vertocoin-cli getinfo
```

### Stop the daemon:

```bash
./src/vertocoin-cli stop
```

### Start with GUI:

```bash
./src/vertocoin-qt
```

## Network Setup

To launch your own Vertocoin network:

1. **Set up seed nodes**:

   - Configure at least 2-3 seed nodes
   - Update DNS records for your seed domains
   - Ensure ports 9333 and 9332 are open

2. **Initial distribution**:

   - The genesis block contains 5 billion VTO
   - You'll need to distribute these coins to initial users
   - Create a plan for fair distribution

3. **Mining/Staking**:
   - Configure proof-of-stake parameters
   - Set up initial staking nodes
   - Document staking rewards and requirements

## Security Considerations

1. **Change default ports** if needed for production
2. **Set up proper firewall rules**
3. **Use strong RPC credentials**
4. **Regular security updates**
5. **Monitor network health**

## Customization Options

### Block Time Adjustment

If you want to change the 1-minute block time, modify in `chainparams.cpp`:

```cpp
consensus.nStakeTargetSpacing = 60; // 60 seconds = 1 minute
```

### Supply Adjustments

To modify the maximum supply or initial distribution:

1. Update the genesis block reward
2. Adjust the block rewards if needed
3. Modify any halving intervals

### Address Prefixes

To change address prefixes, modify in `chainparams.cpp`:

```cpp
base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,70); // 'V' prefix
```

## Troubleshooting

### Common Issues:

1. **Build errors**: Ensure all dependencies are installed
2. **Genesis block errors**: Make sure you've mined a new genesis block
3. **Port conflicts**: Check if ports 9333/9332 are in use
4. **Database errors**: Delete blockchain data and resync

### Getting Help:

- Check the build logs for specific error messages
- Verify all dependencies are properly installed
- Ensure you have sufficient disk space
- Check network connectivity for seed nodes

## Next Steps

1. **Mine genesis block** (critical first step)
2. **Test on a private network** first
3. **Set up proper seed nodes**
4. **Create a website and documentation**
5. **Plan token distribution**
6. **Consider security audits**
7. **Build community and adoption**

## Legal Considerations

- Ensure compliance with local cryptocurrency regulations
- Consider consulting with legal experts
- Implement proper KYC/AML if required
- Be aware of securities laws in your jurisdiction

## Support

This is a fork of Peercoin with custom modifications for Vertocoin. For technical support with the base Peercoin code, refer to the Peercoin documentation and community resources.
