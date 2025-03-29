# Yellow Duckie Coin (YD)

**Version 2.0**

## Roadmap

### ✅ Completed
- Development of YD Solana Token
- Development of YD POW L1 Mainnet

### 🔄 In Development
- YD-Solana Cross-chain Bridge
- YD Asset Listing on DEX

### 📈 In Progress
- YD Listing on Major CEX Exchanges

## Abstract
Yellow Duckie Coin (YD) is a community-driven meme cryptocurrency built on the Solana blockchain, utilizing the Kawpow proof-of-work algorithm. Our project aims to create an accessible, ASIC-resistant mining ecosystem that supports GPU mining while maintaining high security and scalability.

## Introduction
Yellow Duckie Coin emerges as a unique meme cryptocurrency that combines the fun aspects of meme coins with robust technical foundations. By implementing the Kawpow algorithm, we ensure fair distribution and accessibility for miners while maintaining network security.

## Technical Specifications

### Mining Algorithm
- **Algorithm**: Kawpow
- **ASIC Resistance**: Yes
- **Hardware Support**: GPU-optimized
- **Entry Barrier**: Low
- **Block Size**: 4MB
- **Asset Support**: Up to 256 bytes for asset naming

### Token Economics
Yellow Duckie Coin implements a unique token distribution model that combines initial liquidity with sustainable mining rewards:

1. **Initial Distribution**
   - 50% of total supply available for free trading on Solana
   - Initial price set at 0.0001 USDC
   - Ensures immediate market liquidity and accessibility

2. **Mining Distribution**
   - 50% of total supply allocated for mining rewards
   - Base mining reward: 10 YD per minute
   - Lucky miner reward: 1000 YD daily (one lucky winner per day)
   - Mining rewards will continue until block height 2025

3. **Mining Reward Structure**

```cpp
CAmount GetBlockSubsidy(int nHeight, const Consensus::Params& consensusParams)
{
    int halvings = nHeight / consensusParams.nSubsidyHalvingInterval;
    
    if (halvings >= 2025)
        return 0;
    CAmount nSubsidy = 10 * COIN;

    if (nHeight == 1) {
        nSubsidy = nSubsidy + 10500000000 * COIN;
    }
    if (nHeight % 1440 == 0) { // lucky reward!
        nSubsidy += 1000 * COIN;
    }
    return nSubsidy;
}
