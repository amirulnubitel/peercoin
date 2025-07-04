// Genesis block mining utility for Vertocoin
// Copyright (c) 2025 The Vertocoin developers
// Distributed under the MIT software license

#include <arith_uint256.h>
#include <kernel/chainparams.h>
#include <primitives/block.h>
#include <uint256.h>
#include <util/strencodings.h>

#include <iostream>

static CBlock CreateGenesisBlock(const char* pszTimestamp, const CScript& genesisOutputScript, uint32_t nTimeTx, uint32_t nTimeBlock, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    CMutableTransaction txNew;
    txNew.nVersion = 1;
    txNew.vin.resize(1);
    txNew.vout.resize(1);
    txNew.vin[0].scriptSig = CScript() << 486604799 << CScriptNum(9999) << std::vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
    txNew.vout[0].nValue = genesisReward;
    txNew.vout[0].scriptPubKey = genesisOutputScript;
    txNew.nTime = nTimeTx;

    CBlock genesis;
    genesis.nTime = nTimeBlock;
    genesis.nBits = nBits;
    genesis.nNonce = nNonce;
    genesis.nVersion = nVersion;
    genesis.vtx.push_back(MakeTransactionRef(std::move(txNew)));
    genesis.hashPrevBlock.SetNull();
    genesis.hashMerkleRoot = BlockMerkleRoot(genesis);
    return genesis;
}

int main()
{
    const char* pszTimestamp = "Vertocoin 04-JUL-2025 High Speed Private Cryptocurrency for the Future";
    const CScript genesisOutputScript = CScript();
    uint32_t nTimeTx = 1720080000;
    uint32_t nTimeBlock = 1720080000;
    uint32_t nBits = 0x1e0ffff0;
    int32_t nVersion = 1;
    CAmount genesisReward = 5000000000LL * 100000000LL; // 5 billion VTO in satoshis

    arith_uint256 hashTarget = arith_uint256().SetCompact(nBits);

    std::cout << "Mining genesis block for Vertocoin..." << std::endl;
    std::cout << "Target: " << hashTarget.ToString() << std::endl;

    for (uint32_t nNonce = 0; nNonce < 0x100000; nNonce++) {
        CBlock genesis = CreateGenesisBlock(pszTimestamp, genesisOutputScript, nTimeTx, nTimeBlock, nNonce, nBits, nVersion, genesisReward);

        arith_uint256 hash = UintToArith256(genesis.GetHash());
        if (hash <= hashTarget) {
            std::cout << "Found genesis block!" << std::endl;
            std::cout << "Genesis hash: " << genesis.GetHash().ToString() << std::endl;
            std::cout << "Merkle root: " << genesis.hashMerkleRoot.ToString() << std::endl;
            std::cout << "Nonce: " << nNonce << std::endl;
            std::cout << "Time: " << nTimeBlock << std::endl;
            std::cout << "Bits: 0x" << std::hex << nBits << std::endl;
            return 0;
        }

        if (nNonce % 10000 == 0) {
            std::cout << "Nonce: " << nNonce << ", Hash: " << genesis.GetHash().ToString() << std::endl;
        }
    }

    std::cout << "Failed to find genesis block within nonce range" << std::endl;
    return 1;
}
