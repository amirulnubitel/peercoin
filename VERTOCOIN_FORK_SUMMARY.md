# Vertocoin Fork Summary

## Overview

This document summarizes the modifications made to fork Peercoin into Vertocoin with your specified parameters.

## Key Parameters Changed

### Coin Specifications

- **Name**: Vertocoin (VTO)
- **Max Supply**: 50 billion VTO
- **Initial Supply**: 5 billion VTO (in genesis block)
- **Block Time**: 1 minute (high-speed transfers)
- **P2P Port**: 9333
- **RPC Port**: 9332

### Network Features

- High-speed transfers with 1-minute block timing
- Private node capabilities
- Proof-of-stake consensus
- Enhanced privacy features

## Files Modified

### 1. Core Blockchain Parameters (`src/kernel/chainparams.cpp`)

- **Genesis Block**: Updated timestamp to "Vertocoin 04-JUL-2025 High Speed Private Cryptocurrency for the Future"
- **Block Timing**: Changed from 10 minutes to 1 minute (`consensus.nStakeTargetSpacing = 1 * 60`)
- **Initial Supply**: Set genesis block reward to 5 billion VTO
- **Network Ports**: Updated to 9333 (P2P) and 9332 (RPC)
- **Message Start**: Changed network magic bytes to unique Vertocoin values
- **Address Prefixes**: Updated to use 'V' prefix for mainnet addresses
- **Bech32 HRP**: Changed to "vto" for segwit addresses
- **Seed Nodes**: Updated to Vertocoin seed domains
- **Staking Parameters**:
  - Minimum stake age: 7 days (reduced from 30 days)
  - Maximum stake age: 30 days (reduced from 90 days)
  - Coinbase maturity: 50 blocks (reduced from 500)
- **Checkpoints**: Cleared old checkpoints, ready for new blockchain

### 2. Base Chain Parameters (`src/chainparamsbase.cpp`)

- **Main Network**: RPC port 9332, P2P port 9333
- **Testnet**: RPC port 19332, P2P port 19333

### 3. Build Configuration (`configure.ac`)

- **Project Name**: Changed to "Vertocoin"
- **Version**: Updated to 1.0.0
- **URLs**: Updated to Vertocoin GitHub and website
- **Binary Names**: Updated all binary names to use "vertocoin" prefix:
  - `vertocoind` (daemon)
  - `vertocoin-cli` (CLI)
  - `vertocoin-qt` (GUI)
  - `vertocoin-tx` (transaction utility)
  - `vertocoin-wallet` (wallet tool)

### 4. Documentation (`README.md`)

- Complete rewrite for Vertocoin
- Updated features and specifications
- Added network information
- Updated resource links

## New Files Created

### 1. Build Scripts

- **`build_vertocoin.sh`**: Automated build script for macOS
- **`mine_genesis.sh`**: Genesis block mining helper script

### 2. Configuration

- **`vertocoin.conf.example`**: Example configuration file with Vertocoin-specific settings

### 3. Documentation

- **`VERTOCOIN_SETUP.md`**: Comprehensive setup guide
- **`VERTOCOIN_FORK_SUMMARY.md`**: This summary file

### 4. Development Tools

- **`src/genesis_mine.cpp`**: Genesis block mining utility
- **`src/simple_genesis_mine.cpp`**: Simplified genesis mining example

## Critical Next Steps

### 1. Genesis Block Mining (REQUIRED)

Before the blockchain can function, you must:

1. Mine a new genesis block with the correct parameters
2. Update the genesis hash and merkle root in `chainparams.cpp`
3. Rebuild the project

### 2. Network Infrastructure

1. Set up seed nodes at the domains specified in chainparams.cpp
2. Configure DNS for seed domains
3. Ensure proper firewall and security settings

### 3. Testing

1. Test on a private network first
2. Verify all functionality works correctly
3. Test staking and transaction processing

### 4. Security

1. Review all cryptographic parameters
2. Consider professional security audit
3. Test network resilience

## Technical Notes

### Consensus Changes

- **Block Time**: Reduced to 1 minute for high-speed transfers
- **Staking**: Optimized for faster block times
- **Maturity**: Reduced coinbase maturity for faster coin availability

### Network Protocol

- **Magic Bytes**: Changed to unique values to prevent cross-network interference
- **Ports**: Updated to avoid conflicts with other cryptocurrencies
- **Address Format**: Uses 'V' prefix for easy identification

### Privacy Features

- Built on proven blockchain technology
- Enhanced privacy through optimized parameters
- Private node capabilities

## Build Instructions

1. **Prerequisites**: Install development tools and dependencies
2. **Build**: Run `./build_vertocoin.sh`
3. **Genesis Mining**: Run `./mine_genesis.sh` for guidance
4. **Configuration**: Copy and edit `vertocoin.conf.example`
5. **Testing**: Start with testnet first

## Deployment Checklist

- [ ] Mine genesis block
- [ ] Update genesis hash in code
- [ ] Test build and functionality
- [ ] Set up seed nodes
- [ ] Configure DNS
- [ ] Test network connectivity
- [ ] Plan token distribution
- [ ] Security review
- [ ] Documentation updates
- [ ] Community building

## Support and Resources

- **Setup Guide**: `VERTOCOIN_SETUP.md`
- **Build Script**: `build_vertocoin.sh`
- **Configuration**: `vertocoin.conf.example`
- **Genesis Mining**: `mine_genesis.sh`

## Legal Disclaimer

This is a cryptocurrency fork for educational and development purposes. Ensure compliance with local regulations and laws regarding cryptocurrency creation and distribution. Consider consulting with legal professionals before launching a public network.

---

**Created**: July 4, 2025
**Base**: Peercoin v0.15.1
**Target**: Vertocoin v1.0.0
