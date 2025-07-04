// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2021 The Bitcoin Core developers
// Copyright (c) 2011-2025 The Peercoin developers

#include <kernel/chainparams.h>

#include <chainparamsbase.h>
#include <chainparamsseeds.h>
#include <consensus/amount.h>
#include <consensus/merkle.h>
#include <consensus/params.h>
#include <hash.h>
#include <logging.h>
#include <primitives/block.h>
#include <primitives/transaction.h>
#include <script/interpreter.h>
#include <script/script.h>
#include <uint256.h>
#include <util/strencodings.h>

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <cstring>
#include <type_traits>

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

static CBlock CreateGenesisBlockWithPremine(const char* pszTimestamp, const CScript& genesisOutputScript, const CScript& premineOutputScript, uint32_t nTimeTx, uint32_t nTimeBlock, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward, const CAmount& premineAmount)
{
    CMutableTransaction txNew;
    txNew.nVersion = 1;
    txNew.vin.resize(1);
    txNew.vout.resize(2); // Two outputs: normal coinbase + premine
    txNew.vin[0].scriptSig = CScript() << 486604799 << CScriptNum(9999) << std::vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));

    // Standard coinbase output
    txNew.vout[0].nValue = genesisReward;
    txNew.vout[0].scriptPubKey = genesisOutputScript;

    // Premine output - this will be spendable!
    txNew.vout[1].nValue = premineAmount;
    txNew.vout[1].scriptPubKey = premineOutputScript;

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

/**
 * Build the genesis block. Note that the output of its generation
 * transaction cannot be spent since it did not originally exist in the
 * database.
 *
 * CBlock(hash=000000000019d6, ver=1, hashPrevBlock=00000000000000, hashMerkleRoot=4a5e1e, nTime=1231006505, nBits=1d00ffff, nNonce=2083236893, vtx=1)
 *   CTransaction(hash=4a5e1e, ver=1, vin.size=1, vout.size=1, nLockTime=0)
 *     CTxIn(COutPoint(000000, -1), coinbase 04ffff001d0104455468652054696d65732030332f4a616e2f32303039204368616e63656c6c6f72206f6e206272696e6b206f66207365636f6e64206261696c6f757420666f722062616e6b73)
 *     CTxOut(nValue=50.00000000, scriptPubKey=0x5F1DF16B2B704C8A578D0B)
 *   vMerkleTree: 4a5e1e
 */
static CBlock CreateGenesisBlock(uint32_t nTimeTx, uint32_t nTimeBlock, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    const char* pszTimestamp = "Vertocoin start: 2025-07-01 - We build fast, secure, and private money.";
    const CScript genesisOutputScript = CScript(); // Empty script for coinbase

    // Premine address: VCNmB7ZUvzw9VeELQc3ZtRrdVSZsgAz4fw
    // This corresponds to your generated premine address hash160
    std::vector<unsigned char> premineHash160 = ParseHex("12d3443e51e18cb4bff1b243fd2b7c2a3c37b3f7");
    CScript premineOutputScript = CScript() << OP_DUP << OP_HASH160 << premineHash160 << OP_EQUALVERIFY << OP_CHECKSIG;

    const CAmount premineAmount = 5000000000 * COIN; // 5 billion VTO premine

    return CreateGenesisBlockWithPremine(pszTimestamp, genesisOutputScript, premineOutputScript, nTimeTx, nTimeBlock, nNonce, nBits, nVersion, genesisReward, premineAmount);
}

// static CBlock CreateGenesisBlock(uint32_t nTimeTx, uint32_t nTimeBlock, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
// {
//     const char* pszTimestamp = "Vertocoin start: 2025-07-01 - We build fast, secure, and private money.";
//     const CScript genesisOutputScript = CScript();
//     return CreateGenesisBlock(pszTimestamp, genesisOutputScript, nTimeTx, nTimeBlock, nNonce, nBits, nVersion, genesisReward);
// }

/**
 * Main network on which people trade goods and services.
 */
class CMainParams : public CChainParams
{
public:
    CMainParams()
    {
        strNetworkID = CBaseChainParams::MAIN;
        consensus.signet_blocks = false;
        consensus.signet_challenge.clear();
        // Additional height-based rules
        consensus.BIP65Height = 0; // CLTV
        consensus.BIP66Height = 0; // Strict DER
        consensus.CSVHeight = 0;   // CheckSequenceVerify
        consensus.BIP34Height = 0; // Activate immediately for new network
        // consensus.BIP34Hash = uint256S("0x385705b8fd9f4a96b6bf85c7fd5fa30bc39f1a30db3fef44eb4baf1a28b83a75");         // Will be updated with new genesis hash
        consensus.powLimit = uint256S("ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");            // MAXIMUM difficulty (easiest) for bypass
        consensus.bnInitialHashTarget = uint256S("ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff"); // Same as powLimit

        // Copyright (c) 2025 The Vertocoin developers
        consensus.nTargetTimespan = 7 * 24 * 60 * 60;                         // one week
        consensus.nStakeTargetSpacing = 1 * 60;                               // 1-minute block spacing for high speed
        consensus.nTargetSpacingWorkMax = 12 * consensus.nStakeTargetSpacing; // 12 minutes
        consensus.nPowTargetSpacing = consensus.nStakeTargetSpacing;
        consensus.nStakeMinAge = 60 * 60 * 24 * 7;  // minimum age for coin age - 7 days
        consensus.nStakeMaxAge = 60 * 60 * 24 * 30; // 30 days
        consensus.nModifierInterval = 6 * 60 * 60;  // Modifier interval: time to elapse before new modifier is computed
        consensus.nCoinbaseMaturity = 0;            // Allow immediate spending of premine (genesis block)

        consensus.fPowAllowMinDifficultyBlocks = true;   // Allows chain progress during low hash rate
        consensus.fPowNoRetargeting = true;              // Disable retargeting until stable
        consensus.nRuleChangeActivationThreshold = 1512; // 75% during bootstrap
        consensus.nMinerConfirmationWindow = 1440;       // 1 day window at 1-min blocks

        consensus.SegwitHeight = 0; // Activate immediately for new network

        consensus.nMinimumChainWork = uint256S("0x0000000000000000000000000000000000000000000000000000000000000000"); // No minimum work for new network
        consensus.defaultAssumeValid = consensus.hashGenesisBlock;                                                    // Use actual genesis hash

        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 32-bit integer with any alignment.
         */
        pchMessageStart[0] = 0x56; // 'V' in ASCII
        pchMessageStart[1] = 0x45; // 'E'
        pchMessageStart[2] = 0x52; // 'R'
        pchMessageStart[3] = 0x01; // Network version 1
        nDefaultPort = 9333;
        m_assumed_blockchain_size = 3;

        // Create Genesis Block
        genesis = CreateGenesisBlock(1751308800, 1751308800, 1, 0x207fffff, 1, 0); // No coinbase reward, all goes to premine

        consensus.hashGenesisBlock = genesis.GetHash();

        // Debug: Print actual values for verification
        // printf("DEBUG: NEW genesis hash = 0x%s\n", consensus.hashGenesisBlock.GetHex().c_str());
        // printf("DEBUG: NEW merkle root = 0x%s\n", genesis.hashMerkleRoot.GetHex().c_str());
        assert(consensus.hashGenesisBlock == uint256S("0x385705b8fd9f4a96b6bf85c7fd5fa30bc39f1a30db3fef44eb4baf1a28b83a75"));
        assert(genesis.hashMerkleRoot == uint256S("0xf61f48fecdbeac7fa51a92246e18453c20e1b3d314031295e4bd2b3c22dcfaa3"));

        // Note that of those which support the service bits prefix, most only support a subset of
        // possible options.
        // This is fine at runtime as we'll fall back to using them as an addrfetch if they don't support the
        // service bits we want, but we should get them updated to support all service bits wanted by any
        // release ASAP to avoid it where possible.
        // vSeeds.clear(); // No DNS seeds
        // vFixedSeeds.clear();
        // Vertocoin seed nodes (replace with your own)
        vSeeds.emplace_back("explorer.vertomax.com");


        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 70);  // vertocoin: addresses begin with 'V'
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1, 132); // vertocoin: script addresses begin with 'v'
        base58Prefixes[SECRET_KEY] = std::vector<unsigned char>(1, 198);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x88, 0xB2, 0x1E};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x88, 0xAD, 0xE4};

        // human readable prefix to bench32 address
        bech32_hrp = "vto";

        vFixedSeeds = std::vector<uint8_t>(std::begin(chainparams_seed_main), std::end(chainparams_seed_main));

        fMiningRequiresPeers = false; // Allow solo mining for new network startup
        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        m_is_test_chain = false;
        m_is_mockable_chain = false;

        checkpointData = {
            {
                // Vertocoin genesis block with premine to your address
                {0, consensus.hashGenesisBlock}, // Use actual genesis block hash
            }};

        // Debug: Verify checkpoint matches genesis hash
        printf("DEBUG: Checkpoint hash for height 0 = 0x%s\n", checkpointData.mapCheckpoints.at(0).GetHex().c_str());
        printf("DEBUG: Genesis hash matches checkpoint: %s\n",
               (consensus.hashGenesisBlock == checkpointData.mapCheckpoints.at(0)) ? "YES" : "NO");

        m_assumeutxo_data = MapAssumeutxo{
            // TODO to be specified in a future patch.
        };

        chainTxData = ChainTxData{
            // Data as of genesis block (height 0) - Vertocoin network start
            1751308800, // * UNIX timestamp of genesis block (2025-07-01)
            0,          // * total number of transactions at genesis (just the coinbase tx)
                        //   (the tx=... number in the ChainStateFlushed debug.log lines)
            0           // * estimated number of transactions per second (starts at 0 for new network)
        };
    }
};

/**
 * Testnet (v3): public test network which is reset from time to time.
 */
class CTestNetParams : public CChainParams
{
public:
    CTestNetParams()
    {
        strNetworkID = CBaseChainParams::TESTNET;
        consensus.signet_blocks = false;
        consensus.signet_challenge.clear();
        consensus.BIP34Height = 293368;
        consensus.BIP34Hash = uint256S("00000002c0b976c7a5c9878f1cec63fb4d88d68d614aedeaf8158c42d904795e");
        consensus.powLimit = uint256S("0000000fffffffffffffffffffffffffffffffffffffffffffffffffffffffff");            // ~arith_uint256(0) >> 28;
        consensus.bnInitialHashTarget = uint256S("00000007ffffffffffffffffffffffffffffffffffffffffffffffffffffffff"); // ~arith_uint256(0) >> 29;

        consensus.nTargetTimespan = 7 * 24 * 60 * 60;                         // one week
        consensus.nStakeTargetSpacing = 10 * 60;                              // 10-minute block spacing
        consensus.nTargetSpacingWorkMax = 12 * consensus.nStakeTargetSpacing; // 2-hour
        consensus.nPowTargetSpacing = consensus.nStakeTargetSpacing;
        consensus.nStakeMinAge = 60 * 60 * 24; // test net min age is 1 day
        consensus.nStakeMaxAge = 60 * 60 * 24 * 90;
        consensus.nModifierInterval = 60 * 20; // Modifier interval: time to elapse before new modifier is computed
        consensus.nCoinbaseMaturity = 60;

        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 1512; // 75% for testchains
        consensus.nMinerConfirmationWindow = 2016;       // nPowTargetTimespan / nPowTargetSpacing

        consensus.SegwitHeight = 394215;

        consensus.nMinimumChainWork = uint256S("0x00000000000000000000000000000000000000000000000000a39348f70f067a");  // 500000
        consensus.defaultAssumeValid = uint256S("0xa40f64181ee4a3bedda2eae0107d9da0e049fe285b6e6e2a7f1f11697f22c7ed"); // 500000

        pchMessageStart[0] = 0xfb;
        pchMessageStart[1] = 0xfc;
        pchMessageStart[2] = 0xfd;
        pchMessageStart[3] = 0xfe;
        nDefaultPort = 19333;
        m_assumed_blockchain_size = 1;

        genesis = CreateGenesisBlock(1720080000, 1720080000, 1, 0x1e0ffff0, 1, 1000000000 * COIN); // 1 billion VTO testnet initial supply

        consensus.hashGenesisBlock = genesis.GetHash();
        // Testnet genesis will also need to be mined
        // assert(consensus.hashGenesisBlock == uint256S("0x00000001f757bb737f6596503e17cd17b0658ce630cc727c0cca81aec47c9f06"));
        // assert(genesis.hashMerkleRoot == uint256S("0x3c2d8f85fab4d17aac558cc648a1a58acff0de6deb890c29985690052c5993c2"));

        vFixedSeeds.clear();
        vSeeds.clear();
        // nodes with support for servicebits filtering should be at the top
        vSeeds.emplace_back("testnet.vertocoin.net");
        vSeeds.emplace_back("testnet2.vertocoin.net");
        vSeeds.emplace_back("testnet.vertocoin.org");
        vSeeds.emplace_back("testnet.vto.info");

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 111);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1, 196);
        base58Prefixes[SECRET_KEY] = std::vector<unsigned char>(1, 239);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x35, 0x87, 0xCF};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x35, 0x83, 0x94};

        // human readable prefix to bench32 address
        bech32_hrp = "tvto";

        vFixedSeeds = std::vector<uint8_t>(std::begin(chainparams_seed_test), std::end(chainparams_seed_test));

        fMiningRequiresPeers = true;
        fDefaultConsistencyChecks = false;
        fRequireStandard = false;
        m_is_test_chain = true;
        m_is_mockable_chain = false;

        checkpointData = {
            {
                {0, uint256S("0x00000001f757bb737f6596503e17cd17b0658ce630cc727c0cca81aec47c9f06")},
                {19080, uint256S("0xb054d63d41852d71b611eaa8eca37d9fddca69b5013cf0966d453402ec8005ce")},
                {30583, uint256S("0x5179c0c496b5d25ab81ffe14273ea6928c6ff81c0a0d6a83b5d7d41d64886300")},
                {99999, uint256S("0xa7b03b14b8673683d972ab81775f3e85fea4fe689874b5956183466535dc651c")},
                {219999, uint256S("0x0691bb86c92762c5c4c5a3723585ebeb7ec59310bbb0bdb6666551ab24ad919e")},
                {336000, uint256S("0xf07adf61615c529f7c282b858d13d3e037b197324cb12e0669c461947494c4e3")},
                {372751, uint256S("0x000000000000148db599b217c117b5104f5043c55f6ca2a8a065d9fab9f9bba1")},
                {382019, uint256S("0x3ab75769d7957d9bf0857b5019d0a0e41044fa9ecf30b2f9c32aa457b0864ce5")},
                {408500, uint256S("0x1636ac08b073d26b28fa40243d58dd5deb215752efe094c92c61998e4e9baf3f")},
                {412691, uint256S("0x0e20318be88f07f521453435b37cfc516c3de07264a78ed7170985a1126126ab")},
                {441299, uint256S("0x4091d0836a37c50ceee876000ac0cb251fd10031dab901d2c0677cd86283096e")},
                {442735, uint256S("0x1b83b33894d51be0b8b323bfab093f638915236e0e40ba3b52bb33fdbc4053cd")},
                {516308, uint256S("0x0000000ed333c5fa4be2c941f3c3a0680c3a7d582cbf74f4516f8503474e2585")},
                {573702, uint256S("0xc41259778268fe3bba597c7707fe400d9a7d66b4d6d2a9593c898fd69fb56a5f")},
                {612778, uint256S("0x00000003216118fab90ea268ce526e1fcce67dc97b74c5b62119cc3d244d8f71")},
            }};

        m_assumeutxo_data = MapAssumeutxo{
            // TODO to be specified in a future patch.
        };

        chainTxData = ChainTxData{
            // Data as of block 00000003216118fab90ea268ce526e1fcce67dc97b74c5b62119cc3d244d8f71 (height 612778)
            1739494055, // * UNIX timestamp of last known number of transactions
            1221679,    // * total number of transactions between genesis and that timestamp
                        //   (the tx=... number in the SetBestChain debug.log lines)
            0.003104928 // * estimated number of transactions per second after that timestamp
                        //   1221679/(1739494055-1346029522) = 0.003104928

        };
    }
};

/**
 * Signet: test network with an additional consensus parameter (see BIP325).
 */
class SigNetParams : public CChainParams
{
public:
    explicit SigNetParams(const SigNetOptions& options)
    {
        std::vector<uint8_t> bin;
        vSeeds.clear();

        if (!options.challenge) {
            bin = ParseHex("512103ad5e0edad18cb1f0fc0d28a3d4f1f3e445640337489abb10404f2d1e086be430210359ef5021964fe22d6f8e05b2463c9540ce96883fe3b278760f048f5189f2e6c452ae");
            vSeeds.emplace_back("seed.signet.bitcoin.sprovoost.nl.");

            // Hardcoded nodes can be removed once there are more DNS seeds
            vSeeds.emplace_back("178.128.221.177");
            vSeeds.emplace_back("v7ajjeirttkbnt32wpy3c6w3emwnfr3fkla7hpxcfokr3ysd3kqtzmqd.onion:38333");

            consensus.nMinimumChainWork = uint256S("0x000000000000000000000000000000000000000000000000000001291fc22898");
            consensus.defaultAssumeValid = uint256S("0x000000d1a0e224fa4679d2fb2187ba55431c284fa1b74cbc8cfda866fd4d2c09"); // 105495
            m_assumed_blockchain_size = 1;
            m_assumed_chain_state_size = 0;
            chainTxData = ChainTxData{
                // Data from RPC: getchaintxstats 4096 000000d1a0e224fa4679d2fb2187ba55431c284fa1b74cbc8cfda866fd4d2c09
                .nTime = 1661702566,
                .nTxCount = 1903567,
                .dTxRate = 0.02336701143027275,
            };
        } else {
            bin = *options.challenge;
            consensus.nMinimumChainWork = uint256{};
            consensus.defaultAssumeValid = uint256{};
            m_assumed_blockchain_size = 0;
            m_assumed_chain_state_size = 0;
            chainTxData = ChainTxData{
                0,
                0,
                0,
            };
            LogPrintf("Signet with challenge %s\n", HexStr(bin));
        }

        if (options.seeds) {
            vSeeds = *options.seeds;
        }

        strNetworkID = CBaseChainParams::SIGNET;
        consensus.signet_blocks = true;
        consensus.signet_challenge.assign(bin.begin(), bin.end());
        // consensus.nSubsidyHalvingInterval = 210000;
        consensus.BIP34Height = 1;
        consensus.BIP34Hash = uint256{};
        consensus.BIP65Height = 1;
        consensus.BIP66Height = 1;
        consensus.CSVHeight = 1;
        consensus.SegwitHeight = 1;
        // consensus.nTargetTimespan = 7 * 24 * 60 * 60; // two weeks
        consensus.nPowTargetSpacing = 10 * 60;
        consensus.fPowAllowMinDifficultyBlocks = false;
        consensus.fPowNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 1815; // 90% of 2016
        consensus.nMinerConfirmationWindow = 2016;       // nPowTargetTimespan / nPowTargetSpacing
        consensus.MinBIP9WarningHeight = 0;
        consensus.powLimit = uint256S("00000377ae000000000000000000000000000000000000000000000000000000");
        /*
                consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
                consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = Consensus::BIP9Deployment::NEVER_ACTIVE;
                consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = Consensus::BIP9Deployment::NO_TIMEOUT;
                consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].min_activation_height = 0; // No activation delay

                // Activation of Taproot (BIPs 340-342)
                consensus.vDeployments[Consensus::DEPLOYMENT_TAPROOT].bit = 2;
                consensus.vDeployments[Consensus::DEPLOYMENT_TAPROOT].nStartTime = Consensus::BIP9Deployment::ALWAYS_ACTIVE;
                consensus.vDeployments[Consensus::DEPLOYMENT_TAPROOT].nTimeout = Consensus::BIP9Deployment::NO_TIMEOUT;
                consensus.vDeployments[Consensus::DEPLOYMENT_TAPROOT].min_activation_height = 0; // No activation delay
        */
        // message start is defined as the first 4 bytes of the sha256d of the block script
        HashWriter h{};
        h << consensus.signet_challenge;
        uint256 hash = h.GetHash();
        memcpy(pchMessageStart, hash.begin(), 4);

        nDefaultPort = 38333;

        genesis = CreateGenesisBlock(1720080000, 1720080000, 1, 0x1e0ffff0, 1, 1000000000 * COIN); // Vertocoin SigNet genesis
        consensus.hashGenesisBlock = genesis.GetHash();
        // assert(consensus.hashGenesisBlock == uint256S("0x6b0bea195c0dc4212b91aee36be332dbd6b59fc317ce744ae5a413a8d051b903"));
        // assert(genesis.hashMerkleRoot == uint256S("0x6e4d62e871f3b3380995885ff3136712d4059fe3132e060c8a374acd2b63a278"));

        vFixedSeeds.clear();

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 111);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1, 196);
        base58Prefixes[SECRET_KEY] = std::vector<unsigned char>(1, 239);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x35, 0x87, 0xCF};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x35, 0x83, 0x94};

        bech32_hrp = "tb";

        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        m_is_test_chain = true;
        m_is_mockable_chain = false;
    }
};

/**
 * Regression test: intended for private networks only. Has minimal difficulty to ensure that
 * blocks can be found instantly.
 */
class CRegTestParams : public CChainParams
{
public:
    explicit CRegTestParams(const RegTestOptions& opts)
    {
        strNetworkID = CBaseChainParams::REGTEST;
        consensus.signet_blocks = false;
        consensus.signet_challenge.clear();
        // consensus.nSubsidyHalvingInterval = 150;
        consensus.BIP34Height = 1; // Always active unless overridden
        consensus.BIP34Hash = uint256();
        consensus.BIP65Height = 1;  // Always active unless overridden
        consensus.BIP66Height = 1;  // Always active unless overridden
        consensus.CSVHeight = 1;    // Always active unless overridden
        consensus.SegwitHeight = 0; // Always active unless overridden
        consensus.MinBIP9WarningHeight = 0;
        consensus.powLimit = uint256S("7fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.bnInitialHashTarget = uint256S("00000007ffffffffffffffffffffffffffffffffffffffffffffffffffffffff"); // ~arith_uint256(0) >> 29;

        consensus.nTargetTimespan = 7 * 24 * 60 * 60;                         // two weeks
        consensus.nStakeTargetSpacing = 10 * 60;                              // 10-minute block spacing
        consensus.nTargetSpacingWorkMax = 12 * consensus.nStakeTargetSpacing; // 2-hour
        consensus.nPowTargetSpacing = consensus.nStakeTargetSpacing;

        consensus.nStakeMinAge = 60 * 60 * 24; // test net min age is 1 day
        consensus.nStakeMaxAge = 60 * 60 * 24 * 90;
        consensus.nModifierInterval = 60 * 20; // Modifier interval: time to elapse before new modifier is computed
        consensus.nCoinbaseMaturity = 60;

        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = true;
        consensus.nRuleChangeActivationThreshold = 108; // 75% for testchains
        consensus.nMinerConfirmationWindow = 144;       // Faster than normal for regtest (144 instead of 2016)

        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = Consensus::BIP9Deployment::NO_TIMEOUT;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].min_activation_height = 0; // No activation delay

        consensus.vDeployments[Consensus::DEPLOYMENT_TAPROOT].bit = 2;
        consensus.vDeployments[Consensus::DEPLOYMENT_TAPROOT].nStartTime = Consensus::BIP9Deployment::ALWAYS_ACTIVE;
        consensus.vDeployments[Consensus::DEPLOYMENT_TAPROOT].nTimeout = Consensus::BIP9Deployment::NO_TIMEOUT;
        consensus.vDeployments[Consensus::DEPLOYMENT_TAPROOT].min_activation_height = 0; // No activation delay

        consensus.nMinimumChainWork = uint256{};
        consensus.defaultAssumeValid = uint256{};

        pchMessageStart[0] = 0xcb;
        pchMessageStart[1] = 0xf2;
        pchMessageStart[2] = 0xc0;
        pchMessageStart[3] = 0xef;
        nDefaultPort = 9903;
        m_assumed_blockchain_size = 0;
        m_assumed_chain_state_size = 0;

        for (const auto& [dep, height] : opts.activation_heights) {
            switch (dep) {
            case Consensus::BuriedDeployment::DEPLOYMENT_SEGWIT:
                consensus.SegwitHeight = int{height};
                break;
            case Consensus::BuriedDeployment::DEPLOYMENT_HEIGHTINCB:
                consensus.BIP34Height = int{height};
                break;
            case Consensus::BuriedDeployment::DEPLOYMENT_DERSIG:
                consensus.BIP66Height = int{height};
                break;
            case Consensus::BuriedDeployment::DEPLOYMENT_CLTV:
                consensus.BIP65Height = int{height};
                break;
            case Consensus::BuriedDeployment::DEPLOYMENT_CSV:
                consensus.CSVHeight = int{height};
                break;
            }
        }

        for (const auto& [deployment_pos, version_bits_params] : opts.version_bits_parameters) {
            consensus.vDeployments[deployment_pos].nStartTime = version_bits_params.start_time;
            consensus.vDeployments[deployment_pos].nTimeout = version_bits_params.timeout;
            consensus.vDeployments[deployment_pos].min_activation_height = version_bits_params.min_activation_height;
        }

        genesis = CreateGenesisBlock(1720080000, 1720080000, 1, 0x1e0ffff0, 1, 1000000000 * COIN); // Vertocoin regtest genesis

        consensus.hashGenesisBlock = genesis.GetHash();
        // assert(consensus.hashGenesisBlock == uint256S("0x6b0bea195c0dc4212b91aee36be332dbd6b59fc317ce744ae5a413a8d051b903"));
        // assert(genesis.hashMerkleRoot == uint256S("0x6e4d62e871f3b3380995885ff3136712d4059fe3132e060c8a374acd2b63a278"));

        vFixedSeeds.clear(); //!< Regtest mode doesn't have any fixed seeds.
        vSeeds.clear();
        vSeeds.emplace_back("dummySeed.invalid.");

        fDefaultConsistencyChecks = true;
        fRequireStandard = true;
        m_is_test_chain = true;
        m_is_mockable_chain = true;

        fMiningRequiresPeers = false;

        checkpointData = {
            {{0, uint256S("0x00000001f757bb737f6596503e17cd17b0658ce630cc727c0cca81aec47c9f06")}}};

        m_assumeutxo_data = MapAssumeutxo{
            {
                110,
                {AssumeutxoHash{uint256S("0x1ebbf5850204c0bdb15bf030f47c7fe91d45c44c712697e4509ba67adb01c618")}, 110},
            },
            {
                200,
                {AssumeutxoHash{uint256S("0x51c8d11d8b5c1de51543c579736e786aa2736206d1e11e627568029ce092cf62")}, 200},
            },
        };

        chainTxData = ChainTxData{
            0,
            0,
            0};

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 111);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1, 196);
        base58Prefixes[SECRET_KEY] = std::vector<unsigned char>(1, 239);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x35, 0x87, 0xCF};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x35, 0x83, 0x94};

        bech32_hrp = "pcrt";
    }
};

std::unique_ptr<const CChainParams> CChainParams::SigNet(const SigNetOptions& options)
{
    return std::make_unique<const SigNetParams>(options);
}

std::unique_ptr<const CChainParams> CChainParams::RegTest(const RegTestOptions& options)
{
    return std::make_unique<const CRegTestParams>(options);
}

std::unique_ptr<const CChainParams> CChainParams::Main()
{
    return std::make_unique<const CMainParams>();
}

std::unique_ptr<const CChainParams> CChainParams::TestNet()
{
    return std::make_unique<const CTestNetParams>();
}
