#ifndef BLOCKCHAIN_H
#define BLOCKCHAIN_H

#include "block.h"
#include "string.h"

#define AUTHOR_ADDR "leopnt" /* miner of the first block */
#define SYSTEM_ADDR "SYSTEM" /* bitcoin system addr for rewards */
#define NUM_BLOCKS 65535     /* see block.h id uint16_t for max id */
#define INITIAL_REWARD 50

typedef struct
{
    Block *blocks[NUM_BLOCKS];
    uint16_t size;
    uint8_t difficulty;
    Transaction pending_txs[NUM_TRANSACTIONS];
    uint8_t pending_tx_idx;
    uint32_t reward;
} Blockchain;

Blockchain bchain_new(uint8_t initial_difficulty);
void bchain_free(Blockchain *bchain);
bool bchain_verify_tx(Blockchain *bchain, Transaction tx);
void bchain_transaction(Blockchain *bchain, Transaction tx);
void bchain_mine(Blockchain *bchain, char minerAddr[ADDR_SIZE]);
void bchain_add_block(
    Blockchain *bchain, Transaction txs[NUM_TRANSACTIONS]);
Block *bchain_latest(const Blockchain *bchain);
bool bchain_is_valid(const Blockchain *bchain);
uint64_t bchain_get_balance(const Blockchain *bchain, char userAddr[ADDR_SIZE]);

#endif