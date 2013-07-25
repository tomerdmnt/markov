Markov chain text generator. Reads from stdin and writes to stdout

```bash
  $ curl http://www.gutenberg.org/cache/epub/2701/pg2701.txt \
  	| markov --max-words=500
```

## Install

```bash
   $ git clone ...
   $ cd markov
   $ make
```

## Usage

```
  Usage: markov [options]

    reads from stdin and writes a markov chain text to stdout

  Options:

    -p, --prefix-words		Number of words in prefix [2]
    -w, --max-words		Maximum number of words to output [50]
    -d, --delimiter		Delimiter between words in output [' ']
    -s, --hash-size		Hash tables sizes [10000]
    -h, --help			Prints this help message
```
