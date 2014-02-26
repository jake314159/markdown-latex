Markdown Latex
==============

Converts a markdown (.md) file into a LaTeX (.tex) file which can then be used with a LaTeX compiler to create pretty documents from markdown

Example running
------------

Once compiled (see compile section) the code can be run as followed

```bash
markdownlatex < example.md > output.tex
```

Note: Currently markdownlatex uses the standard input and standard output to do the conversion. In the future it will be possible to supply the files as arguments

Compile
----------

The file can be compiled with the following command

```bash
gcc src/markdownlatex.c -std=c99 -o bin/markdownlatex
```

Supported markdown
--------------

Not all markdown is supported (yet). Below is a list of the supported markdown syntax

+ Headers H1-H2 done with hashes (#)
+ Lists done with + operators
+ Bold
+ Underline

Markdown syntax TODO
--------------
+ Lists with - and *
+ Ordered lists
+ Lists with multiple paragraphs
+ underline
+ Strike through
+ Line/Page breaks
+ Block quotes
+ Nested block quotes
+ Tables
+ Images
+ Links
+ Symbols
+ Math
+ Code blocks
+ Horizontal rules (3 or more * or - (ignoring spaces))
+ Backslash escapes


TODO
--------
Take file names as paramiters
Include flags to change document formating (eg. Font side or type of document)

