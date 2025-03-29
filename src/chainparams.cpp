// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2016 The Bitcoin Core developers
// Copyright (c) 2017-2021  The Ravncore developers
// Copyright (c) @2024 		The YellowDuckie Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "chainparams.h"
#include "consensus/merkle.h"

#include "arith_uint256.h"
#include "tinyformat.h"
#include "util.h"
#include "utilstrencodings.h"

#include "chainparamsseeds.h"
#include <assert.h>

// TODO: Take these out
extern double algoHashTotal[16];
extern int algoHashHits[16];


static CBlock CreateGenesisBlock(const char* pszTimestamp, const CScript& genesisOutputScript, uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    CMutableTransaction txNew;
    txNew.nVersion = 1;
    txNew.vin.resize(1);
    txNew.vout.resize(1);
    txNew.vin[0].scriptSig = CScript() << CScriptNum(0) << 486604799 << CScriptNum(4) << std::vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
    txNew.vout[0].nValue = genesisReward;
    txNew.vout[0].scriptPubKey = genesisOutputScript;

    CBlock genesis;
    genesis.nTime = nTime;
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
static CBlock CreateGenesisBlock(uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    const char* pszTimestamp = "01/01/2025, I am a YellowDuckie, forever in love with the sea.";
    const CScript genesisOutputScript = CScript() << ParseHex("04bff80104a89615d723eea2bd862d850aa02a9964a4328f94f7aa1a666739d57b4cdfebf313339424f9110756fec556a4cfc947e9e6166f2f91fdf9c56e7f755f") << OP_CHECKSIG;
    return CreateGenesisBlock(pszTimestamp, genesisOutputScript, nTime, nNonce, nBits, nVersion, genesisReward);
}


void CChainParams::UpdateVersionBitsParameters(Consensus::DeploymentPos d, int64_t nStartTime, int64_t nTimeout)
{
    consensus.vDeployments[d].nStartTime = nStartTime;
    consensus.vDeployments[d].nTimeout = nTimeout;
}

void CChainParams::TurnOffSegwit()
{
    consensus.nSegwitEnabled = false;
}

void CChainParams::TurnOffCSV()
{
    consensus.nCSVEnabled = false;
}

void CChainParams::TurnOffBIP34()
{
    consensus.nBIP34Enabled = false;
}

void CChainParams::TurnOffBIP65()
{
    consensus.nBIP65Enabled = false;
}

void CChainParams::TurnOffBIP66()
{
    consensus.nBIP66Enabled = false;
}

bool CChainParams::BIP34()
{
    return consensus.nBIP34Enabled;
}

bool CChainParams::BIP65()
{
    return consensus.nBIP34Enabled;
}

bool CChainParams::BIP66()
{
    return consensus.nBIP34Enabled;
}

bool CChainParams::CSVEnabled() const
{
    return consensus.nCSVEnabled;
}


/**
 * Main network
 */
/**
 * What makes a good checkpoint block?
 * + Is surrounded by blocks with reasonable timestamps
 *   (no blocks before with a timestamp after, none after with
 *    timestamp before)
 * + Contains no strange transactions
 */

class CMainParams : public CChainParams
{
public:
    CMainParams()
    {
        uint32_t nGenesisTime = 1740358800;
        uint32_t timestap = 1743249828;
        strNetworkID = "main";
        consensus.nSubsidyHalvingInterval = 525600; // 1 y
        consensus.nBIP34Enabled = true;
        consensus.nBIP65Enabled = true; // 
        consensus.nBIP66Enabled = true;
        consensus.nSegwitEnabled = true;
        consensus.nCSVEnabled = true;
        consensus.powLimit = uint256S("00ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.kawpowLimit = uint256S("000000ffffffffffffffffffffffffffffffffffffffffffffffffffffffffff"); // Estimated starting diff for first 180 kawpow blocks
        consensus.nPowTargetTimespan = 86400;     //6hrs                           
        consensus.nPowTargetSpacing = 60;
        consensus.fPowAllowMinDifficultyBlocks = false;
        consensus.fPowNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 1613; // 75% of window
        consensus.nMinerConfirmationWindow = 2016;       // 2 weeks for 1min blocks
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = nGenesisTime; 
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = timestap+500;   
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nOverrideRuleChangeActivationThreshold = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nOverrideMinerConfirmationWindow = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_ASSETS].bit = 6;                 // Assets 
        consensus.vDeployments[Consensus::DEPLOYMENT_ASSETS].nStartTime = nGenesisTime; 
        consensus.vDeployments[Consensus::DEPLOYMENT_ASSETS].nTimeout = timestap+500;  
        consensus.vDeployments[Consensus::DEPLOYMENT_ASSETS].nOverrideRuleChangeActivationThreshold = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_ASSETS].nOverrideMinerConfirmationWindow = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_MSG_REST_ASSETS].bit = 7;                                       // Assets
        consensus.vDeployments[Consensus::DEPLOYMENT_MSG_REST_ASSETS].nStartTime = nGenesisTime;                      
        consensus.vDeployments[Consensus::DEPLOYMENT_MSG_REST_ASSETS].nTimeout = timestap+500;                       
        consensus.vDeployments[Consensus::DEPLOYMENT_MSG_REST_ASSETS].nOverrideRuleChangeActivationThreshold = 1; 
        consensus.vDeployments[Consensus::DEPLOYMENT_MSG_REST_ASSETS].nOverrideMinerConfirmationWindow = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_TRANSFER_SCRIPT_SIZE].bit = 8;
        consensus.vDeployments[Consensus::DEPLOYMENT_TRANSFER_SCRIPT_SIZE].nStartTime = nGenesisTime;                       
        consensus.vDeployments[Consensus::DEPLOYMENT_TRANSFER_SCRIPT_SIZE].nTimeout = timestap+500;                        
        consensus.vDeployments[Consensus::DEPLOYMENT_TRANSFER_SCRIPT_SIZE].nOverrideRuleChangeActivationThreshold = 1; 
        consensus.vDeployments[Consensus::DEPLOYMENT_TRANSFER_SCRIPT_SIZE].nOverrideMinerConfirmationWindow = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_ENFORCE_VALUE].bit = 9;
        consensus.vDeployments[Consensus::DEPLOYMENT_ENFORCE_VALUE].nStartTime = nGenesisTime;                      
        consensus.vDeployments[Consensus::DEPLOYMENT_ENFORCE_VALUE].nTimeout = timestap+500;                        
        consensus.vDeployments[Consensus::DEPLOYMENT_ENFORCE_VALUE].nOverrideRuleChangeActivationThreshold = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_ENFORCE_VALUE].nOverrideMinerConfirmationWindow = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_COINBASE_ASSETS].bit = 10;
        consensus.vDeployments[Consensus::DEPLOYMENT_COINBASE_ASSETS].nStartTime = nGenesisTime;                       
        consensus.vDeployments[Consensus::DEPLOYMENT_COINBASE_ASSETS].nTimeout = timestap+500;                        
        consensus.vDeployments[Consensus::DEPLOYMENT_COINBASE_ASSETS].nOverrideRuleChangeActivationThreshold = 1; 
        consensus.vDeployments[Consensus::DEPLOYMENT_COINBASE_ASSETS].nOverrideMinerConfirmationWindow = 1;


        // The best chain should have at least this much work

        consensus.nMinimumChainWork = uint256S("0000000000000000000000000000000000000000000000000000000000000000"); // #150000

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0000001c7a63984ac30b79b846f5ec601aae24ab237d79dc35e9dc6971fe7136"); // 

        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 32-bit integer with any alignment.
         */
        pchMessageStart[0] = 0x59; // 'Y'
        pchMessageStart[1] = 0x44; // 'D'
        pchMessageStart[2] = 0x59; // 'Y'
        pchMessageStart[3] = 0x44; // 'D'

        nDefaultPort = 23333 ;//YD  23333

        nPruneAfterHeight = 144000;

        uint32_t _nBits=0x1e00ffff;

        genesis = CreateGenesisBlock(nGenesisTime, 9655495, _nBits, 4, 5000 * COIN);
        consensus.hashGenesisBlock = genesis.GetX16RHash();


        assert(consensus.hashGenesisBlock == uint256S("0000001c7a63984ac30b79b846f5ec601aae24ab237d79dc35e9dc6971fe7136"));
        assert(genesis.hashMerkleRoot == uint256S("0297a562c53ed0654f307672e2cf2fb7ebf0898f6fdc4d2fcb930af3b89d53d8"));

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 78 ); // 78 Y
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1, 30); // D
        base58Prefixes[SECRET_KEY] = std::vector<unsigned char>(1, 128);// Bitcoin standard
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x88, 0xB2, 0x1E};// Bitcoin standard
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x88, 0xAD, 0xE4};// Bitcoin standard

        // YellowDuckie BIP44 cointype in mainnet is '233'
        nExtCoinType = 233;//233 for fun

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_main, pnSeed6_main + ARRAYLEN(pnSeed6_main));

        vSeeds.emplace_back("seed1.YellowDuckie.net", false); 
        vSeeds.emplace_back("seed2.YellowDuckie.net", false); 
        vSeeds.emplace_back("seed3.YellowDuckie.net", false); 
        vSeeds.emplace_back("seed4.YellowDuckie.net", false); 
        vSeeds.emplace_back("seed5.YellowDuckie.net", false); 
        vSeeds.emplace_back("seed6.YellowDuckie.net", false); 

        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;
        fMiningRequiresPeers = true;

        checkpointData = (CCheckpointData){
            {
                {0, uint256S("0000001c7a63984ac30b79b846f5ec601aae24ab237d79dc35e9dc6971fe7136")},
          
            }};

        chainTxData = ChainTxData{

                0, // * UNIX timestamp of last known number of transactions
                0,     // * total number of transactions between genesis and that timestamp
                            //   (the tx=... number in the SetBestChain debug.log lines)
                0      // * estimated number of transactions per second after that timestamp
            };


        /** YD Start **/
        // Burn Amounts
        nIssueAssetBurnAmount = 1 * COIN;
        nReissueAssetBurnAmount = 1 * COIN;
        nIssueSubAssetBurnAmount = 1 * COIN;
        nIssueUniqueAssetBurnAmount = 1 * COIN;
        nIssueMsgChannelAssetBurnAmount = 1 * COIN;
        nIssueQualifierAssetBurnAmount = 1 * COIN;
        nIssueSubQualifierAssetBurnAmount = 1 * COIN;
        nIssueRestrictedAssetBurnAmount = 1 * COIN;
        nAddNullQualifierTagBurnAmount = 1 * COIN;

        // Global Burn Address
        strGlobalBurnAddress = "Ygu4tknxhJQQzq4inot7JHiDkCBXwcZwMi";

        // Burn Addresses
        strIssueAssetBurnAddress = "Ygu4tknxhJQQzq4inot7JHiDkCBXwcZwMi";
        strReissueAssetBurnAddress = "Ygu4tknxhJQQzq4inot7JHiDkCBXwcZwMi";
        strIssueSubAssetBurnAddress = "Ygu4tknxhJQQzq4inot7JHiDkCBXwcZwMi";
        strIssueUniqueAssetBurnAddress = "Ygu4tknxhJQQzq4inot7JHiDkCBXwcZwMi";
        strIssueMsgChannelAssetBurnAddress = "Ygu4tknxhJQQzq4inot7JHiDkCBXwcZwMi";
        strIssueQualifierAssetBurnAddress = "Ygu4tknxhJQQzq4inot7JHiDkCBXwcZwMi";
        strIssueSubQualifierAssetBurnAddress = "Ygu4tknxhJQQzq4inot7JHiDkCBXwcZwMi";
        strIssueRestrictedAssetBurnAddress = "Ygu4tknxhJQQzq4inot7JHiDkCBXwcZwMi";
        strAddNullQualifierTagBurnAddress = "Ygu4tknxhJQQzq4inot7JHiDkCBXwcZwMi";


        strOrgAddress = "YXaT5BXKiyCenjnTUssR5QkEYkDLuJJvve";
        // DGW Activation
        nDGWActivationBlock = 1;

        nMaxReorganizationDepth = 60; // 60 at 1 minute block timespan is +/- 60 minutes.
        nMinReorganizationPeers = 4;//4
        nMinReorganizationAge = 60 * 60 * 12; // 12 hours

        nAssetActivationHeight = 1;     // Asset activated block height
        nMessagingActivationBlock = 1;  // Messaging activated block height
        nRestrictedActivationBlock =1; // Restricted activated block height

        nKAAAWWWPOWActivationTime = nGenesisTime + 1; 
        nKAWPOWActivationTime = nKAAAWWWPOWActivationTime;
        /** YD End **/
    }
};

/**
 * Testnet (v7)
 */
class CTestNetParams : public CChainParams
{
public:
    CTestNetParams()
    {
        strNetworkID = "test";
        uint32_t nGenesisTime =1731824904;
        consensus.nSubsidyHalvingInterval = 1000; //
        consensus.nBIP34Enabled = true;
        consensus.nBIP65Enabled = true; // 
        consensus.nBIP66Enabled = true;
        consensus.nSegwitEnabled = true;
        consensus.nCSVEnabled = true;

        consensus.powLimit = uint256S("ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.kawpowLimit = uint256S("000000ffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowTargetTimespan = 600; // 10 mins
        consensus.nPowTargetSpacing = 15;
        consensus.fPowAllowMinDifficultyBlocks = false;
        consensus.fPowNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 480; // Approx 65% for testchains0
        consensus.nMinerConfirmationWindow = 600;// nPowTargetTimespan / nPowTargetSpacing0
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1731377205; 
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1731377205+1800;  
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nOverrideRuleChangeActivationThreshold = 40;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nOverrideMinerConfirmationWindow = 50;
        consensus.vDeployments[Consensus::DEPLOYMENT_ASSETS].bit = 5;
        consensus.vDeployments[Consensus::DEPLOYMENT_ASSETS].nStartTime = 1731377205; 
        consensus.vDeployments[Consensus::DEPLOYMENT_ASSETS].nTimeout = 1731377205+1800;  
        consensus.vDeployments[Consensus::DEPLOYMENT_ASSETS].nOverrideRuleChangeActivationThreshold = 40;
        consensus.vDeployments[Consensus::DEPLOYMENT_ASSETS].nOverrideMinerConfirmationWindow = 50;
        consensus.vDeployments[Consensus::DEPLOYMENT_MSG_REST_ASSETS].bit = 6;                 // Assets 
        consensus.vDeployments[Consensus::DEPLOYMENT_MSG_REST_ASSETS].nStartTime = 1731377205; 
        consensus.vDeployments[Consensus::DEPLOYMENT_MSG_REST_ASSETS].nTimeout = 1731377205+1800;  
        consensus.vDeployments[Consensus::DEPLOYMENT_MSG_REST_ASSETS].nOverrideRuleChangeActivationThreshold = 40;
        consensus.vDeployments[Consensus::DEPLOYMENT_MSG_REST_ASSETS].nOverrideMinerConfirmationWindow = 50;
        consensus.vDeployments[Consensus::DEPLOYMENT_TRANSFER_SCRIPT_SIZE].bit = 8;
        consensus.vDeployments[Consensus::DEPLOYMENT_TRANSFER_SCRIPT_SIZE].nStartTime = 1731377205; 
        consensus.vDeployments[Consensus::DEPLOYMENT_TRANSFER_SCRIPT_SIZE].nTimeout = 1731377205+1800;   
        consensus.vDeployments[Consensus::DEPLOYMENT_TRANSFER_SCRIPT_SIZE].nOverrideRuleChangeActivationThreshold = 40;
        consensus.vDeployments[Consensus::DEPLOYMENT_TRANSFER_SCRIPT_SIZE].nOverrideMinerConfirmationWindow = 50;
        consensus.vDeployments[Consensus::DEPLOYMENT_ENFORCE_VALUE].bit = 9;
        consensus.vDeployments[Consensus::DEPLOYMENT_ENFORCE_VALUE].nStartTime = 1731377205;                      
        consensus.vDeployments[Consensus::DEPLOYMENT_ENFORCE_VALUE].nTimeout = 1731377205+1800;                        
        consensus.vDeployments[Consensus::DEPLOYMENT_ENFORCE_VALUE].nOverrideRuleChangeActivationThreshold = 40;
        consensus.vDeployments[Consensus::DEPLOYMENT_ENFORCE_VALUE].nOverrideMinerConfirmationWindow = 50;
        consensus.vDeployments[Consensus::DEPLOYMENT_COINBASE_ASSETS].bit = 10;
        consensus.vDeployments[Consensus::DEPLOYMENT_COINBASE_ASSETS].nStartTime = 1731377205;                       
        consensus.vDeployments[Consensus::DEPLOYMENT_COINBASE_ASSETS].nTimeout = 1731377205+1800;                        
        consensus.vDeployments[Consensus::DEPLOYMENT_COINBASE_ASSETS].nOverrideRuleChangeActivationThreshold = 40; 
        consensus.vDeployments[Consensus::DEPLOYMENT_COINBASE_ASSETS].nOverrideMinerConfirmationWindow = 50;

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0000000000000000000000000000000000000000000000000000000000000000");//6216
        // 0000000261793a9216e324f3fcd3ad272f9f2525d0f160946c79333dbf94993b
        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("000000f0706fc1b0b388b507621bc75f623e18260909d0685cf1109b46edacf1");

        pchMessageStart[0] = 0x84; // T
        pchMessageStart[0] = 0x50; // 'P'
        pchMessageStart[1] = 0x48; // 'H'
        pchMessageStart[2] = 0x49; // 'I'

        nDefaultPort = 18965;
        nPruneAfterHeight = 1000;

        // uint32_t nGenesisTime = 1706774400; // 

        uint32_t _nBits=0x1e00ffff;

        genesis = CreateGenesisBlock(nGenesisTime, 43746675, _nBits, 4, 5000 * COIN);
        consensus.hashGenesisBlock = genesis.GetX16RHash();


        // Test MerkleRoot and GenesisBlock
        assert(consensus.hashGenesisBlock == uint256S("000000f0706fc1b0b388b507621bc75f623e18260909d0685cf1109b46edacf1"));
        assert(genesis.hashMerkleRoot == uint256S("0297a562c53ed0654f307672e2cf2fb7ebf0898f6fdc4d2fcb930af3b89d53d8"));

        vSeeds.emplace_back("seed1.test.YellowDuckie.net", false); 

        vSeeds.emplace_back("seed2.test.YellowDuckie.net", false); 

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 66); // T
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1, 66); // T
        base58Prefixes[SECRET_KEY] = std::vector<unsigned char>(1, 239);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x35, 0x87, 0xCF};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x35, 0x83, 0x94};

        // Raven BIP44 cointype in testnet
        nExtCoinType = 1;

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_test, pnSeed6_test + ARRAYLEN(pnSeed6_test));

        fDefaultConsistencyChecks = false;
        fRequireStandard = false;
        fMineBlocksOnDemand = false;
        fMiningRequiresPeers = true;

        checkpointData = (CCheckpointData){
            {
                 {0, uint256S("000000f0706fc1b0b388b507621bc75f623e18260909d0685cf1109b46edacf1")},

                }};

        chainTxData = ChainTxData{

            0, // * UNIX timestamp of last known number of transactions
            0,     // * total number of transactions between genesis and that timestamp
                        //   (the tx=... number in the SetBestChain debug.log lines)
            0      // * estimated number of transactions per second after that timestamp
        };


        // Burn Amounts

        nIssueAssetBurnAmount = 0.1 * COIN;
        nReissueAssetBurnAmount = 0.1 * COIN;
        nIssueSubAssetBurnAmount = 0.1 * COIN;
        nIssueUniqueAssetBurnAmount = 0.1 * COIN;
        nIssueMsgChannelAssetBurnAmount = 0.1 * COIN;
        nIssueQualifierAssetBurnAmount = 0.1 * COIN;
        nIssueSubQualifierAssetBurnAmount = 0.1 * COIN;
        nIssueRestrictedAssetBurnAmount = 0.1 * COIN;
        nAddNullQualifierTagBurnAmount = 0.1 * COIN;

        // Burn Addresses
        strIssueAssetBurnAddress = "Tmkx2JgVtBbPKArQwNH9LvkbhvwfakrEHs";
        strReissueAssetBurnAddress = "Tmkx2JgVtBbPKArQwNH9LvkbhvwfakrEHs";
        strIssueSubAssetBurnAddress = "Tmkx2JgVtBbPKArQwNH9LvkbhvwfakrEHs";
        strIssueUniqueAssetBurnAddress = "Tmkx2JgVtBbPKArQwNH9LvkbhvwfakrEHs";
        strIssueMsgChannelAssetBurnAddress = "Tmkx2JgVtBbPKArQwNH9LvkbhvwfakrEHs";
        strIssueQualifierAssetBurnAddress = "Tmkx2JgVtBbPKArQwNH9LvkbhvwfakrEHs";
        strIssueSubQualifierAssetBurnAddress = "Tmkx2JgVtBbPKArQwNH9LvkbhvwfakrEHs";
        strIssueRestrictedAssetBurnAddress = "Tmkx2JgVtBbPKArQwNH9LvkbhvwfakrEHs";
        strAddNullQualifierTagBurnAddress = "Tmkx2JgVtBbPKArQwNH9LvkbhvwfakrEHs";

        strOrgAddress = "Tmkx2JgVtBbPKArQwNH9LvkbhvwfakrEHs";
        // Global Burn Address
        strGlobalBurnAddress = "Tmkx2JgVtBbPKArQwNH9LvkbhvwfakrEHs";

        // DGW Activation
        nDGWActivationBlock = 1;

        nMaxReorganizationDepth = 60/4; // 60 at 1 minute block timespan is +/- 60 minutes.
        nMinReorganizationPeers = 2;
        nMinReorganizationAge = 60 * 60 * 12/4; // 12 hours

        nAssetActivationHeight = 1;      // Asset activated block height
        nMessagingActivationBlock = 1;  // Messaging activated block height
        nRestrictedActivationBlock = 1; // Restricted activated block height

        nKAAAWWWPOWActivationTime = nGenesisTime + 1; 
        nKAWPOWActivationTime = nKAAAWWWPOWActivationTime;
        /** YD End **/
    }
};

/**
 * Regression test
 */
class CRegTestParams : public CChainParams
{
public:
    CRegTestParams()
    {
        strNetworkID = "regtest";
        consensus.nBIP34Enabled = true;
        consensus.nBIP65Enabled = true; // 000000000000000004c2b624ed5d7756c508d90fd0da2c7c679febfa6c4735f0
        consensus.nBIP66Enabled = true;
        consensus.nSegwitEnabled = true;
        consensus.nCSVEnabled = true;
        consensus.nSubsidyHalvingInterval = 150;
        consensus.powLimit = uint256S("7fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.kawpowLimit = uint256S("7fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowTargetTimespan = 2016 * 60/4; 
        consensus.nPowTargetSpacing = 15;
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = true;
        consensus.nRuleChangeActivationThreshold = 108; // 75% for testchains
        consensus.nMinerConfirmationWindow = 144;       // Faster than normal for regtest (144 instead of 2016)
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 999999999999ULL;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nOverrideRuleChangeActivationThreshold = 108;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nOverrideMinerConfirmationWindow = 144;
        consensus.vDeployments[Consensus::DEPLOYMENT_ASSETS].bit = 6;
        consensus.vDeployments[Consensus::DEPLOYMENT_ASSETS].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_ASSETS].nTimeout = 999999999999ULL;
        consensus.vDeployments[Consensus::DEPLOYMENT_ASSETS].nOverrideRuleChangeActivationThreshold = 108;
        consensus.vDeployments[Consensus::DEPLOYMENT_ASSETS].nOverrideMinerConfirmationWindow = 144;
        consensus.vDeployments[Consensus::DEPLOYMENT_MSG_REST_ASSETS].bit = 7;                    // Assets (RIP5)
        consensus.vDeployments[Consensus::DEPLOYMENT_MSG_REST_ASSETS].nStartTime = 0;             
        consensus.vDeployments[Consensus::DEPLOYMENT_MSG_REST_ASSETS].nTimeout = 999999999999ULL;
        consensus.vDeployments[Consensus::DEPLOYMENT_MSG_REST_ASSETS].nOverrideRuleChangeActivationThreshold = 108;
        consensus.vDeployments[Consensus::DEPLOYMENT_MSG_REST_ASSETS].nOverrideMinerConfirmationWindow = 144;
        consensus.vDeployments[Consensus::DEPLOYMENT_TRANSFER_SCRIPT_SIZE].bit = 8;
        consensus.vDeployments[Consensus::DEPLOYMENT_TRANSFER_SCRIPT_SIZE].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_TRANSFER_SCRIPT_SIZE].nTimeout = 999999999999ULL;
        consensus.vDeployments[Consensus::DEPLOYMENT_TRANSFER_SCRIPT_SIZE].nOverrideRuleChangeActivationThreshold = 208;
        consensus.vDeployments[Consensus::DEPLOYMENT_TRANSFER_SCRIPT_SIZE].nOverrideMinerConfirmationWindow = 288;
        consensus.vDeployments[Consensus::DEPLOYMENT_ENFORCE_VALUE].bit = 9;
        consensus.vDeployments[Consensus::DEPLOYMENT_ENFORCE_VALUE].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_ENFORCE_VALUE].nTimeout = 999999999999ULL;
        consensus.vDeployments[Consensus::DEPLOYMENT_ENFORCE_VALUE].nOverrideRuleChangeActivationThreshold = 108;
        consensus.vDeployments[Consensus::DEPLOYMENT_ENFORCE_VALUE].nOverrideMinerConfirmationWindow = 144;
        consensus.vDeployments[Consensus::DEPLOYMENT_COINBASE_ASSETS].bit = 10;
        consensus.vDeployments[Consensus::DEPLOYMENT_COINBASE_ASSETS].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_COINBASE_ASSETS].nTimeout = 999999999999ULL;
        consensus.vDeployments[Consensus::DEPLOYMENT_COINBASE_ASSETS].nOverrideRuleChangeActivationThreshold = 400;
        consensus.vDeployments[Consensus::DEPLOYMENT_COINBASE_ASSETS].nOverrideMinerConfirmationWindow = 500;

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x00");

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x00");

        pchMessageStart[0] = 0x84; // T
        pchMessageStart[0] = 0x50; // 'P'
        pchMessageStart[1] = 0x48; // 'H'
        pchMessageStart[2] = 0x49; // 'I'

        nDefaultPort = 18966;
        nPruneAfterHeight = 1000;

        uint32_t nGenesisTime = 1731824904; // 

        uint32_t _nBits=0x1e00ffff;
        genesis = CreateGenesisBlock(nGenesisTime, 13763736, _nBits, 4, 5000 * COIN);
        consensus.hashGenesisBlock = genesis.GetX16RHash();

        assert(consensus.hashGenesisBlock == uint256S("000000e813673e9e9f560bd2b94b5c1a4f481234bd03cff70fb059417a8e327c"));
        assert(genesis.hashMerkleRoot == uint256S("0297a562c53ed0654f307672e2cf2fb7ebf0898f6fdc4d2fcb930af3b89d53d8"));


        vFixedSeeds.clear(); //!< Regtest mode doesn't have any fixed seeds.
        vSeeds.clear();      //!< Regtest mode doesn't have any DNS seeds.

        fDefaultConsistencyChecks = true;
        fRequireStandard = false;
        fMineBlocksOnDemand = true;

        checkpointData = (CCheckpointData){
            {}};

        chainTxData = ChainTxData{
            0,
            0,
            0};

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 66); // T
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1, 66); // T
        base58Prefixes[SECRET_KEY] = std::vector<unsigned char>(1, 239);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x35, 0x87, 0xCF};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x35, 0x83, 0x94};

        // YellowDuckieCoin BIP44 cointype in regtest
        nExtCoinType = 1;

        /** YD Start **/
        // Burn Amounts
        nIssueAssetBurnAmount = 0.1 * COIN;
        nReissueAssetBurnAmount = 0.1 * COIN;
        nIssueSubAssetBurnAmount = 0.1 * COIN;
        nIssueUniqueAssetBurnAmount = 0.1 * COIN;
        nIssueMsgChannelAssetBurnAmount = 0.1 * COIN;
        nIssueQualifierAssetBurnAmount = 0.1 * COIN;
        nIssueSubQualifierAssetBurnAmount = 0.1 * COIN;
        nIssueRestrictedAssetBurnAmount = 0.1 * COIN;
        nAddNullQualifierTagBurnAmount = 0.1 * COIN;

        // Burn Addresses
        strIssueAssetBurnAddress = "Tmkx2JgVtBbPKArQwNH9LvkbhvwfakrEHs";
        strReissueAssetBurnAddress = "Tmkx2JgVtBbPKArQwNH9LvkbhvwfakrEHs";
        strIssueSubAssetBurnAddress = "Tmkx2JgVtBbPKArQwNH9LvkbhvwfakrEHs";
        strIssueUniqueAssetBurnAddress = "Tmkx2JgVtBbPKArQwNH9LvkbhvwfakrEHs";
        strIssueMsgChannelAssetBurnAddress = "Tmkx2JgVtBbPKArQwNH9LvkbhvwfakrEHs";
        strIssueQualifierAssetBurnAddress = "Tmkx2JgVtBbPKArQwNH9LvkbhvwfakrEHs";
        strIssueSubQualifierAssetBurnAddress = "Tmkx2JgVtBbPKArQwNH9LvkbhvwfakrEHs";
        strIssueRestrictedAssetBurnAddress = "Tmkx2JgVtBbPKArQwNH9LvkbhvwfakrEHs";
        strAddNullQualifierTagBurnAddress = "Tmkx2JgVtBbPKArQwNH9LvkbhvwfakrEHs";
        strOrgAddress = "Tmkx2JgVtBbPKArQwNH9LvkbhvwfakrEHs";
        // Global Burn Address
        strGlobalBurnAddress = "Tmkx2JgVtBbPKArQwNH9LvkbhvwfakrEHs";

        // DGW Activation
        nDGWActivationBlock = 200;

        nMaxReorganizationDepth = 60; // 60 at 1 minute block timespan is +/- 60 minutes.
        nMinReorganizationPeers = 4;
        nMinReorganizationAge = 60 * 60 * 12; // 12 hours

        nAssetActivationHeight = 0;     // Asset activated block height
        nMessagingActivationBlock = 0;  // Messaging activated block height
        nRestrictedActivationBlock = 0; // Restricted activated block height

        // TODO, we need to figure out what to do with this for regtest. This effects the unit tests
        // For now we can use a timestamp very far away
        // If you are looking to test the kawpow hashing function in regtest. You will need to change this number
        nKAAAWWWPOWActivationTime = nGenesisTime + 1; 
        nKAWPOWActivationTime = nKAAAWWWPOWActivationTime;
        /** YD End **/
    }
};

static std::unique_ptr<CChainParams> globalChainParams;

const CChainParams& GetParams()
{
    assert(globalChainParams);
    return *globalChainParams;
}

std::unique_ptr<CChainParams> CreateChainParams(const std::string& chain)
{
    if (chain == CBaseChainParams::MAIN)
        return std::unique_ptr<CChainParams>(new CMainParams());
    else if (chain == CBaseChainParams::TESTNET)
        return std::unique_ptr<CChainParams>(new CTestNetParams());
    else if (chain == CBaseChainParams::REGTEST)
        return std::unique_ptr<CChainParams>(new CRegTestParams());
    throw std::runtime_error(strprintf("%s: Unknown chain %s.", __func__, chain));
}

void SelectParams(const std::string& network, bool fForceBlockNetwork)
{
    SelectBaseParams(network);
    if (fForceBlockNetwork) {
        bNetwork.SetNetwork(network);
    }
    globalChainParams = CreateChainParams(network);
}

void UpdateVersionBitsParameters(Consensus::DeploymentPos d, int64_t nStartTime, int64_t nTimeout)
{
    globalChainParams->UpdateVersionBitsParameters(d, nStartTime, nTimeout);
}

void TurnOffSegwit()
{
    globalChainParams->TurnOffSegwit();
}

void TurnOffCSV()
{
    globalChainParams->TurnOffCSV();
}

void TurnOffBIP34()
{
    globalChainParams->TurnOffBIP34();
}

void TurnOffBIP65()
{
    globalChainParams->TurnOffBIP65();
}

void TurnOffBIP66()
{
    globalChainParams->TurnOffBIP66();
}
