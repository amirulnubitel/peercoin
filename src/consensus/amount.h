// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2021 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BITCOIN_CONSENSUS_AMOUNT_H
#define BITCOIN_CONSENSUS_AMOUNT_H

#include <cstdint>
#include <string>

/** Amount in satoshis (Can be negative) */
typedef int64_t CAmount;

static constexpr CAmount COIN = 1000000;
static constexpr CAmount CENT = 10000;

static const CAmount MIN_TX_FEE_PREV7 = CENT;
static const CAmount MIN_TX_FEE = CENT / 10;
static const CAmount PERKB_TX_FEE = CENT;
static const CAmount MIN_TXOUT_AMOUNT = CENT;
static const CAmount MAX_MINT_PROOF_OF_WORK = 1 * COIN;
static const CAmount MAX_MINT_PROOF_OF_WORK_V10 = 5000000000LL * COIN; // 5 billion VTO for Vertocoin genesis and early blocks
static const std::string CURRENCY_UNIT = "VTO";
static const std::string CURRENCY_ATOM = "sat"; // One indivisible minimum value unit

/** No amount larger than this (in satoshi) is valid.
 *
 * Note that this constant is *not* the total money supply, which in Vertocoin
 * currently happens to be 50,000,000,000 VTO for the maximum supply, but
 * rather a sanity check. As this sanity check is used by consensus-critical
 * validation code, the exact value of the MAX_MONEY constant is consensus
 * critical; in unusual circumstances like a(nother) overflow bug that allowed
 * for the creation of coins out of thin air modification could lead to a fork.
 * */
static constexpr CAmount MAX_MONEY = 50000000000LL * COIN; // 50 billion VTO maximum supply
inline bool MoneyRange(const CAmount& nValue) { return (nValue >= 0 && nValue <= MAX_MONEY); }

#endif // BITCOIN_CONSENSUS_AMOUNT_H
