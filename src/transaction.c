#include "transaction.h"

Transaction transaction_new(char fromAddr[], char toAddr[], uint32_t amount)
{
    Transaction t;
    strcpy(t.fromAddr, fromAddr);
    strcpy(t.toAddr, toAddr);
    t.amount = amount;

    return t;
}

char *transaction_to_str(const Transaction *transaction)
{
    memset(_transaction_str_buf_out, 0, sizeof(_transaction_str_buf_out));

    strcat(_transaction_str_buf_out, "Transaction(");
    strcat(_transaction_str_buf_out, transaction->fromAddr);
    strcat(_transaction_str_buf_out, ", ");
    strcat(_transaction_str_buf_out, transaction->toAddr);
    strcat(_transaction_str_buf_out, ", ");
    char uint32_str[16] = {0};
    sprintf(uint32_str, "%u", transaction->amount);
    strcat(_transaction_str_buf_out, uint32_str);
    strcat(_transaction_str_buf_out, ")");

    return _transaction_str_buf_out;
}