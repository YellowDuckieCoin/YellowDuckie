// Copyright (c) 2016 The Bitcoin Core developers
// Copyright (c) @2024 The YellowDuckieCoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef YellowDuckieCoin_WALLET_TEST_FIXTURE_H
#define YellowDuckieCoin_WALLET_TEST_FIXTURE_H

#include "test/test_yellowduckiecoin.h"

/** Testing setup and teardown for wallet.
 */
struct WalletTestingSetup : public TestingSetup
{
    explicit WalletTestingSetup(const std::string &chainName = CBaseChainParams::MAIN);

    ~WalletTestingSetup();
};

#endif

