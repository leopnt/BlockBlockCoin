#include <stdio.h>
#include "src/blockchain.h"

int main(int argc, char *argv[])
{
    Blockchain bchain = bchain_new(12);
    bchain.difficulty += 2; /* increase difficulty by 2 bits */

    bchain_transaction(&bchain, transaction_new(AUTHOR_ADDR, "user1", 5));
    bchain_transaction(&bchain, transaction_new(AUTHOR_ADDR, "user2", 5));
    bchain_transaction(&bchain, transaction_new(AUTHOR_ADDR, "user3", 5));
    bchain_transaction(&bchain, transaction_new(AUTHOR_ADDR, "user1", 5));
    bchain_transaction(&bchain, transaction_new(AUTHOR_ADDR, "user2", 5));
    bchain_transaction(&bchain, transaction_new(AUTHOR_ADDR, "user3", 5));
    /* see NUM_TRANSACTIONS in block.h */
    /* pending transactions is now full. We wait for a miner to mine */

    /* we cannot add transactions until a new mining has occured here */
    /* this one will be dumped */
    bchain_transaction(&bchain, transaction_new(AUTHOR_ADDR, "user4", 5));
    bchain_transaction(&bchain, transaction_new(AUTHOR_ADDR, "user4", 5));
    bchain_transaction(&bchain, transaction_new(AUTHOR_ADDR, "user4", 5));

    bchain_mine(&bchain, "miner1");

    /* trying to inject money */
    /* this one will be dumped */
    bchain_transaction(&bchain, transaction_new(SYSTEM_ADDR, "user3", 92384));

    bchain_transaction(&bchain, transaction_new("user1", "user3", 5));
    bchain_transaction(&bchain, transaction_new("user1", "miner1", 2));
    bchain_transaction(&bchain, transaction_new("user3", "user2", 1));

    /* we cannot mine until pending transactions is full */
    /* this mining will not occur */
    bchain_mine(&bchain, "miner1");
    bchain.difficulty += 1; /* increase difficulty by 1 bits */

    bchain_transaction(&bchain, transaction_new("user2", "user1", 3));

    /* a user cannot send more than he has */
    /* this one will be dumped */
    bchain_transaction(&bchain, transaction_new("user1", "user3", 92384));

    bchain_transaction(&bchain, transaction_new("user1", "user3", 3));
    bchain_transaction(&bchain, transaction_new("miner1", "user4", 10));

    bchain_mine(&bchain, "user3");
    /* user3 gets a reward: the first transaction of the next block */

    /* print the blockchain */
    printf("\nThe BLOCKCHAIN:\n");
    for (uint16_t i = 0; i < bchain.size; i++)
        printf("%s\n", block_str(bchain.blocks[i]));

    printf("\nbalance of leopnt \t%lld\n", bchain_get_balance(
                                               &bchain, AUTHOR_ADDR));
    printf("balance of user1 \t%lld\n", bchain_get_balance(&bchain, "user1"));
    printf("balance of user2 \t%lld\n", bchain_get_balance(&bchain, "user2"));
    printf("balance of user3 \t%lld\n", bchain_get_balance(&bchain, "user3"));
    printf("balance of user4 \t%lld\n", bchain_get_balance(&bchain, "user4"));
    printf("balance of miner1 \t%lld\n", bchain_get_balance(&bchain, "miner1"));

    printf("\nchain is valid: %d\n", bchain_is_valid(&bchain));
    printf("modifying block 2, transaction 5 with new amount: 999999\n");
    bchain.blocks[2]->transactions[5].amount = 999999;
    printf("chain is valid: %d\n", bchain_is_valid(&bchain));

    /* free memory */
    bchain_free(&bchain);

    return 0;
}