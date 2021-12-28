# blockchain_test

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
