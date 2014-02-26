#Markdown Latex


Converts a markdown (.md) file into a LaTeX (.tex) file which can then be used with a LaTeX compiler to create pretty documents

##Example running

Once compiled (see compile section) the code can be run as followed

```bash
markdownlatex example.md > output.tex
```

It can also be run with the command

```bash
markdownlatex < example.md > output.tex
```

Note: Currently markdown-latex uses the standard output to set the output file. In the future it will be possible to supply these files as arguments

##Compile

The file can be compiled with the following commands

```bash
cd markdownlatex
make
make clean
```

##Supported markdown

Not all markdown is supported (yet). Below is a list of the supported markdown syntax

+ Headers H1-H2 done with hashes
+ Lists done with + operators
+ Bold
+ Italic
+ Underline
+ Code blocks
+ Page break - not standard markdown but can be done with "\\newpage" (without the quotes)
+ Horizontal rules (3 or more \* or \- (ignoring spaces))
+ Backslash escapes

##Markdown syntax TODO
+ Lists with \*
+ Ordered lists
+ Lists with multiple paragraphs
+ Strike through
+ Line breaks
+ Block quotes
+ Nested block quotes
+ Tables
+ Images
+ Links
+ Symbols
+ Math
+ Code highlighting
+ Underlined headings with === and --- for H1 and H2

##TODO
+ Take file names as parameters
+ Include flags to change document formatting (eg. Font side or type of document)
+ Handle buffer overflows

