#include <stdint.h>
#include "aes.h"

void KeyGen(uint8_t roundkeys[][STATE_ROW_SIZE][STATE_COL_SIZE], uint8_t master_key[STATE_ROW_SIZE][STATE_COL_SIZE])
{
    for (uint8_t i = 0; i < STATE_ROW_SIZE; i++)
    {
        for (uint8_t j = 0; j < STATE_COL_SIZE; j++)
        {
            roundkeys[0][i][j] = master_key[i][j];
        }
    }

    for (uint8_t round = 1; round <= ROUND_COUNT; round++)
    {
        ColumnFill(roundkeys, round);
        OtherColumnsFill(roundkeys, round);
    }
}

void ColumnFill(uint8_t roundkeys[][STATE_ROW_SIZE][STATE_COL_SIZE], int round)
{
    uint8_t temp[STATE_ROW_SIZE];

    for (uint8_t i = 0; i < STATE_ROW_SIZE; i++)
    {
        temp[i] = roundkeys[round - 1][i][STATE_COL_SIZE - 1];
    }

    uint8_t t = temp[0];
    for (uint8_t i = 0; i < STATE_ROW_SIZE - 1; i++)
    {
        temp[i] = temp[i + 1];
    }
    temp[STATE_ROW_SIZE - 1] = t;

    for (uint8_t i = 0; i < STATE_ROW_SIZE; i++)
    {
        temp[i] = sboxtab[temp[i]];
    }

    temp[0] ^= rcon[round - 1];

    for (uint8_t i = 0; i < STATE_ROW_SIZE; i++)
    {
        roundkeys[round][i][0] = roundkeys[round - 1][i][0] ^ temp[i];
    }
}

void OtherColumnsFill(uint8_t roundkeys[][STATE_ROW_SIZE][STATE_COL_SIZE], int round)
{
    for (uint8_t j = 1; j < STATE_COL_SIZE; j++)
    {
        for (uint8_t i = 0; i < STATE_ROW_SIZE; i++)
        {
            roundkeys[round][i][j] = roundkeys[round - 1][i][j] ^ roundkeys[round][i][j - 1];
        }
    }
}

void GetRoundKey(uint8_t roundkey[STATE_ROW_SIZE][STATE_COL_SIZE], uint8_t roundkeys[][STATE_ROW_SIZE][STATE_COL_SIZE], int round)
{
    for (uint8_t i = 0; i < STATE_ROW_SIZE; i++)
    {
        for (uint8_t j = 0; j < STATE_COL_SIZE; j++)
        {
            roundkey[i][j] = roundkeys[round][i][j];
        }
    }
}

void MessageToState(uint8_t state[STATE_ROW_SIZE][STATE_COL_SIZE], uint8_t message[DATA_SIZE])
{
    for (uint8_t i = 0; i < STATE_ROW_SIZE; i++)
    {
        for (uint8_t j = 0; j < STATE_COL_SIZE; j++)
        {
            state[i][j] = message[i + j * STATE_ROW_SIZE];
        }
    }
}

void StateToMessage(uint8_t message[DATA_SIZE], uint8_t state[STATE_ROW_SIZE][STATE_COL_SIZE])
{
    for (uint8_t i = 0; i < STATE_ROW_SIZE; i++)
    {
        for (uint8_t j = 0; j < STATE_COL_SIZE; j++)
        {
            message[i + j * STATE_ROW_SIZE] = state[i][j];
        }
    }
}

void MCMatrixColumnProduct(uint8_t colonne[STATE_COL_SIZE])
{
    uint8_t temp[STATE_COL_SIZE];

    for (uint8_t i = 0; i < STATE_COL_SIZE; i++)
    {
        temp[i] = colonne[i];
    }

    colonne[0] = gmul(temp[0], 2) ^ gmul(temp[1], 3) ^ temp[2] ^ temp[3];
    colonne[1] = temp[0] ^ gmul(temp[1], 2) ^ gmul(temp[2], 3) ^ temp[3];
    colonne[2] = temp[0] ^ temp[1] ^ gmul(temp[2], 2) ^ gmul(temp[3], 3);
    colonne[3] = gmul(temp[0], 3) ^ temp[1] ^ temp[2] ^ gmul(temp[3], 2);
}
