#Markdown Latex

Converts a markdown (.md) file into a LaTeX (.tex) file which can then be used with a LaTeX compiler to create pretty documents.  

Currently being developed on *Ubuntu 12.04 LTS* but wider support will be brought with the first offical release. For the time being feel free to try it on your machine and post any problems on the issues tab and even if it works tell me so I can add your setup as being offically supported.  

##Example running

Once compiled (see compile section) the code can be run as followed

```bash
markdownlatex -o output_tex.tex input_markdown.md
```

It can also be run using standard input and output with the command

```bash
markdownlatex < example.md > output.tex
```

##Compile & install

The file can be compiled with the following commands

```bash
cd markdownlatex
make
make clean
```

Once compiled it can then be installed (only tested on Linux) with the following command

```bash
make install
```

##Supported markdown

Not all markdown is supported (yet). Below is a list of the supported markdown syntax

+ Headers H1-H3 done with hashes
+ Lists done with + or - operators
+ Ordered lists done with + or - operators
+ Bold
+ Italic
+ Underline
+ Strike through
+ Code blocks & syntax highlighting
+ Page break - not standard markdown but can be done with 4 or more empty lines
+ Horizontal rules (3 or more \* or \- (ignoring spaces))
+ Backslash escapes
+ Tables (Markdown within tables however is not currently supported)
+ Line breaks
+ Links
+ Block quotes
+ Images

##Markdown syntax TODO
+ Lists with \*
+ Lists with multiple paragraphs
+ Nested lists
+ Symbols
+ Math
+ Underlined headings with === and --- for H1 and H2
+ Images from a url

##Required tex packages

Currently markdownlatex imports all the required tex packages even if they aren't used in the document. You can manually remove the unused packages from the tex file as a temporary solution until markdownlatex is designed to only include the packages needed. Below is a list of packages which will be included (and what they are used for).

+ *listings* (For code blocks)
+ *tabularx* (used for tables)
+ *xcolor* (used for shading every other table row and code highlighting)
+ *tableShade* (used for shading every other table row)
+ *geometry* (used for selecting custom margin sizes (can't be removed))
+ *ulem* (used for strikethroughs)
+ *graphicx* (used for images)

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

##Licence

   Copyright 2014 Jacob Causon

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.


