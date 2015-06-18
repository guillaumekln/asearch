# Introduction

This project provides a fast approximative string search based on the Damerau-Levenshtein distance. It consists of two parts:

* the *compiler* that serializes a dictionnary of words into a Patricia trie;
* the *approximator* that loads the trie and reads search queries from its standard input.

This project focuses on simplicity and search performance. It is written in C++11.

# Compile

The projet uses CMake.

    $ mkdir build
    $ cd build
    $ cmake ..
    $ make


# Usage

## Compiler

    $ ./compiler words.txt trie.bin

`words.txt` is a text file that contains one word per line in the format:

    <word>\t<frequency>

For example:

    n936    705
    n937    695
    n938    2014
    ...

## Approximator

    $ cat query.txt
    approx 1 google
    approx 2 facebook
    $ ./approx trie.bin < query.txt

The query must follow the format:

    approx <maximal distance> <word>

The *approximator* outputs the results in a JSON array.
For example :

    $ echo "approx 0 google" | ./approx trie.bin
    [{"word":"google","freq":54816211,"distance":0}]

The results within the JSON array are ordered in:

* increasing distance;
* decreasing frequency;
* increasing lexicographical order.
