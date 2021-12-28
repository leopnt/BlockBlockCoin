#include "blockchain.h"

Blockchain bchain_new(uint8_t initial_difficulty)
{
    /* inititalize */
    Blockchain bchain;
    bchain.difficulty = initial_difficulty;
    memset(bchain.blocks, 0, NUM_BLOCKS);

    /* add first block */
    Transaction txs[NUM_TRANSACTIONS];
    for (uint8_t i = 0; i < NUM_TRANSACTIONS; i++)
        txs[i] = transaction_new(SYSTEM_ADDR, SYSTEM_ADDR, 0);

    uint8_t initial_hash[SHA256_BLOCK_SIZE] = {0};
    Block *genesis_block = block_new(0, initial_hash, txs);
    block_mine(genesis_block, bchain.difficulty);
    bchain.blocks[0] = genesis_block;

    /* update size */
    bchain.size = 1;
    bchain.pending_tx_idx = 0;

    bchain.reward = INITIAL_REWARD;

    /* reward the first miner i.e. the author */
    bchain_transaction(
        &bchain, transaction_new(SYSTEM_ADDR, AUTHOR_ADDR, INITIAL_REWARD));

    return bchain;
}

/* free each block of the blockchain memory */
void bchain_free(Blockchain *bchain)
{
    for (uint16_t i = 0; i < bchain->size; i++)
        free(bchain->blocks[i]);
}

bool bchain_verify_tx(Blockchain *bchain, Transaction tx)
{
    /* validate SYSTEM transactions
    check that it comes from mining reward (first transaction of a block)
    */
    if (strcmp(tx.fromAddr, SYSTEM_ADDR) == 0)
    {
        if (bchain->pending_tx_idx != 0)
        {
            printf(
                "DEBUG injection without mining is prohibited\n");
            return false;
        }
        return true;
    }

    uint64_t sender_balance = bchain_get_balance(bchain, tx.fromAddr);
    int64_t diff = (int64_t)(sender_balance - tx.amount);
    printf("DEBUG %s has %llu and tries to send %u\t-> diff=%lld\n",
           tx.fromAddr, sender_balance, tx.amount, diff);

    if (diff < 0)
        return false;

    return true;
}

/* add a transaction to the pending list */
void bchain_transaction(Blockchain *bchain, Transaction tx)
{
    if (bchain->pending_tx_idx >= NUM_TRANSACTIONS)
    {
        printf("cannot add transaction (dumped): waiting for mining...\n");
        return;
    }

    if (!bchain_verify_tx(bchain, tx))
    {
        printf("cannot add transaction (dumped): invalid\n");
        return;
    }

    /* add new pending transaction */
    bchain->pending_txs[bchain->pending_tx_idx] = tx;
    bchain->pending_tx_idx++;
}

void bchain_mine(Blockchain *bchain, char minerAddr[ADDR_SIZE])
{
    if (bchain->pending_tx_idx < NUM_TRANSACTIONS)
    {
        printf("cannot mine transactions: pending transactions is not full\n");
        return;
    }

    bchain_add_block(bchain, bchain->pending_txs);
    bchain->pending_tx_idx = 0;

    /* miner reward */
    bchain_transaction(
        bchain, transaction_new(SYSTEM_ADDR, minerAddr, bchain->reward));
}

/* add a block to the blockchain with the given data */
void bchain_add_block(
    Blockchain *bchain, Transaction txs[NUM_TRANSACTIONS])
{
    if (bchain->size >= NUM_BLOCKS)
    {
        fprintf(stderr, "can't add block: blockchain is full\n");
        exit(EXIT_FAILURE);
    }

    /* create the new block */
    Block *new_block = block_new(
        bchain->size,
        bchain_latest(bchain)->hash,
        txs);

    /* mine the block */
    block_mine(new_block, bchain->difficulty);

    /* set the right block in the array */
    bchain->blocks[bchain->size] = new_block;
    bchain->size++;
}

/* get latest block */
Block *bchain_latest(const Blockchain *bchain)
{
    return bchain->blocks[bchain->size - 1];
}

/* recalculate and test if at least one hash combination differ */
bool bchain_is_valid(const Blockchain *bchain)
{
    /* recalculate and check fist block */
    Block *curr_b = bchain->blocks[0]; /* first block */
    uint8_t recalculated_hash[SHA256_BLOCK_SIZE] = {0};
    block_hash_calc(recalculated_hash, curr_b);
    int cmp = memcmp(recalculated_hash, curr_b->hash, SHA256_BLOCK_SIZE);
    if (cmp != 0)
        return false;

    /* recalculate compare with previous and check blocks */
    for (uint16_t i = 1; i < bchain->size; i++)
    {
        Block *prev_b = bchain->blocks[i - 1]; /* previous block */
        Block *curr_b = bchain->blocks[i];     /* current block */

        /* check combination */
        cmp = memcmp(prev_b->hash, curr_b->prev_hash, SHA256_BLOCK_SIZE);
        if (cmp != 0)
            return false;

        /* check if calculation is correct */
        uint8_t recalculated_hash[SHA256_BLOCK_SIZE] = {0};
        block_hash_calc(recalculated_hash, curr_b);
        cmp = memcmp(recalculated_hash, curr_b->hash, SHA256_BLOCK_SIZE);
        if (cmp != 0)
            return false;
    }

    return true;
}

uint64_t bchain_get_balance(const Blockchain *bchain, char userAddr[ADDR_SIZE])
{
    uint64_t balance = 0;

    char from[ADDR_SIZE];
    char to[ADDR_SIZE];
    /* for each block */
    for (uint16_t i = 0; i < bchain->size; i++)
    {
        /* for each transaction */
        for (uint8_t j = 0; j < NUM_TRANSACTIONS; j++)
        {
            strcpy(from, bchain->blocks[i]->transactions[j].fromAddr);
            strcpy(to, bchain->blocks[i]->transactions[j].toAddr);
            uint32_t amount = bchain->blocks[i]->transactions[j].amount;

            /* check if user is receiver or not and update balance*/
            if (strcmp(userAddr, from) == 0)
                balance -= amount;
            else if (strcmp(userAddr, to) == 0)
                balance += amount;
        }
    }

    /* for each pending transaction */
    for (uint8_t i = 0; i < bchain->pending_tx_idx; i++)
    {
        strcpy(from, bchain->pending_txs[i].fromAddr);
        strcpy(to, bchain->pending_txs[i].toAddr);
        uint32_t amount = bchain->pending_txs[i].amount;

        /* check if user is receiver or not and update balance*/
        if (strcmp(userAddr, from) == 0)
            balance -= amount;
        else if (strcmp(userAddr, to) == 0)
            balance += amount;
    }

    return balance;
}