# BlockBlockCoin

Barebone coin blockchain system for educational purposes

## Known limitations

- transactions are not signed: usernames are given instead of public keys
- mining is not multi threaded
- a new block can only be mined if the pending transactions array of the
  blockchain is full
- it isn't multi threaded or built for a network of clients trying to mine and
  send transactions simultaneously

## What I learned

- learning more about encryption (still, asymetric encryption is to implement
  for signing transactions as I couldn't find a way to do it without writing and
  reading keys and signatures from disk's files)
- how the blockchain works broadly speaking
- going into binary as the mining difficulty is the number of bits set to 0 in
  the beggining of a hash
- manipulating hex numbers to make a string phrase out of a struct's data
  (the phrase is passed into sha256 as a digest)
- some basic economics
- potential and limitations of blockchains (like it eats up a whole lot of
  memory even if the system is optimised)

## Running example

Here is an output example (of `main.c`) where we only add transactions and the system takes care of the rest

```
mining block 0...
found a hash starting with 12 0-bits !
00000000 00000100 10011001 00100000 00001000 01011000 01000101 10110001
11010011 00001111 00011100 10010100 11000110 01011110 01110111 01110000
11011000 01011011 00010110 00111100 11000011 10111011 00001101 11011010
00111100 01001100 00101100 10011100 01110010 11111001 10101011 11111111 

DEBUG leopnt has 50 and tries to send 5	-> diff=45
DEBUG leopnt has 45 and tries to send 5	-> diff=40
DEBUG leopnt has 40 and tries to send 5	-> diff=35
DEBUG leopnt has 35 and tries to send 5	-> diff=30
DEBUG leopnt has 30 and tries to send 5	-> diff=25
DEBUG leopnt has 25 and tries to send 5	-> diff=20
cannot add transaction (dumped): waiting for mining...
cannot add transaction (dumped): waiting for mining...
cannot add transaction (dumped): waiting for mining...

mining block 1...
found a hash starting with 14 0-bits !
00000000 00000000 11101000 11110010 11001110 10111110 00111010 10010101
10101111 11000100 01111111 00010111 10101001 11101101 11011111 00101010
00001100 11001011 01001111 00010110 11110110 11010011 00001001 10001111
11010110 01101100 01101100 11110001 10010100 11010010 10100100 10101110 

DEBUG injection without mining is prohibited
cannot add transaction (dumped): invalid
DEBUG user1 has 10 and tries to send 5	-> diff=5
DEBUG user1 has 5 and tries to send 2	-> diff=3
DEBUG user3 has 15 and tries to send 1	-> diff=14
cannot mine transactions: pending transactions is not full
DEBUG user2 has 11 and tries to send 3	-> diff=8
DEBUG user1 has 6 and tries to send 92384	-> diff=-92378
cannot add transaction (dumped): invalid
DEBUG user1 has 6 and tries to send 3	-> diff=3
DEBUG miner1 has 52 and tries to send 10	-> diff=42

mining block 2...
found a hash starting with 15 0-bits !
00000000 00000000 10011000 11100000 00010100 10111000 11000100 01101100
11010111 01110111 11011001 11000011 10001110 11100001 11100111 01111101
00010110 10010000 10000000 01010000 00001100 10001110 00010010 00000000
01110010 11000000 00011101 11100011 01010111 01101101 11110001 01101000 


The BLOCKCHAIN:
Block {
	id: 0	nonce: 1851	tstamp: 2022-01-03T22:01:37Z
	0x00000000000000000000000000000000
	0x04992085845b1d3f1c94c65e7770d85b163cc3bbdda3c4c2c9c72f9abff
	data: "Transaction(SYSTEM, SYSTEM, 0)Transaction(SYSTEM, SYSTEM, 0)Transaction(SYSTEM, SYSTEM, 0)Transaction(SYSTEM, SYSTEM, 0)Transaction(SYSTEM, SYSTEM, 0)Transaction(SYSTEM, SYSTEM, 0)Transaction(SYSTEM, SYSTEM, 0)"
}
Block {
	id: 1	nonce: 15073	tstamp: 2022-01-03T22:01:37Z
	0x04992085845b1d3f1c94c65e7770d85b163cc3bbdda3c4c2c9c72f9abff
	0x00e8f2cebe3a95afc47f17a9eddf2accb4f16f6d398fd66c6cf194d2a4ae
	data: "Transaction(SYSTEM, leopnt, 50)Transaction(leopnt, user1, 5)Transaction(leopnt, user2, 5)Transaction(leopnt, user3, 5)Transaction(leopnt, user1, 5)Transaction(leopnt, user2, 5)Transaction(leopnt, user3, 5)"
}
Block {
	id: 2	nonce: 29995	tstamp: 2022-01-03T22:01:41Z
	0x00e8f2cebe3a95afc47f17a9eddf2accb4f16f6d398fd66c6cf194d2a4ae
	0x0098e014b8c46cd777d9c38ee1e77d16908050c8e12072c01de3576df168
	data: "Transaction(SYSTEM, miner1, 50)Transaction(user1, user3, 5)Transaction(user1, miner1, 2)Transaction(user3, user2, 1)Transaction(user2, user1, 3)Transaction(user1, user3, 3)Transaction(miner1, user4, 10)"
}

balance of leopnt 	20
balance of user1 	3
balance of user2 	8
balance of user3 	67
balance of user4 	10
balance of miner1 	42

chain is valid: 1
modifying block 2, transaction 5 with new amount: 999999
chain is valid: 0
