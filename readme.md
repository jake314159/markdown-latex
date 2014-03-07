#Markdown Latex

Converts a markdown (.md) file into a LaTeX (.tex) file which can then be used with a LaTeX compiler to create pretty documents

##Example running

Once compiled (see compile section) the code can be run as followed

```bash
markdownlatex -o output_tex.tex input_markdown.md
```

It can also be run with this command

```bash
markdownlatex example.md > output.tex
```

It can also be run with the command

```bash
markdownlatex < example.md > output.tex
```

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
+ Lists done with + or - operators
+ Ordered lists done with + or - operators
+ Bold
+ Italic
+ Underline
+ Code blocks
+ Page break - not standard markdown but can be done with 4 or more empty lines
+ Horizontal rules (3 or more \* or \- (ignoring spaces))
+ Backslash escapes
+ Tables (Markdown within tables however is not currently supported)
+ Line breaks

##Markdown syntax TODO
+ Lists with \*
+ Lists with multiple paragraphs
+ Nested lists
+ Strike through
+ Block quotes
+ Nested block quotes
+ Images
+ Links
+ Symbols
+ Math
+ Code highlighting
+ Underlined headings with === and --- for H1 and H2

##Non standard markdown

In order to give more control over how the markdown is converted into tex there are some extra comments you can include. These comments will be ignored by a standard markdown parser but is used by markdownlatex to do something not supported with standard markdown syntax.

###Cover page

To add a cover page to your document you should include a comment right at the top of the document with information on what to display. For example:

```
<!--
title:A quick example
date:Today
author:Jake
-->
```

Please note that date and author are optional and that the comment block MUST be the first thing in the document otherwise it will be ignored.

##TODO
+ Handle buffer overflows

