# PIRI Writing Counter

A command-line tool for counting paragraphs, sentences, words and characters.

Use : count filename [-s] [-p] [-w]

Execute without any argument to show help.

## Flags
- -s : count spaces as characters.
- -p : count punctuation marks that end a sentence(., ?, !) as characters.
- -w : show the first and last words of each paragraph.

## Build
When building with Visual Studio 2017, follow the instructions [here](https://blogs.msdn.microsoft.com/vcblog/2017/05/05/cpp-modules-in-visual-studio-2017) first. Use command-line argument /module:interface when building count.cpp.

When building with other compilers, make sure you enabled its module system, and refactor to conform to it or not use the module system altogether if need be.