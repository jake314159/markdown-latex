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

##Compile and install

The file can be compiled with the following commands

```bash
cd markdownlatex
make
make clean
```

Once compiled it can then be installed (Linux only) with the following command

```bash
make install
```

For more details including OS specific instructions see the [wiki](https://github.com/jake314159/markdown-latex/wiki/Supported-OSes).

##Supported markdown

Not all markdown is supported (yet). Below is a list of the supported markdown syntax

+ Headers H1-H3 done with hashes
+ Lists
+ Ordered lists done with + or - operators
+ Bold
+ Italic
+ Underline
+ Strike through
+ Code blocks and syntax highlighting
+ Page break - not standard markdown but can be done with 4 or more empty lines
+ Horizontal rules (3 or more \* or \- (ignoring spaces))
+ Backslash escapes
+ Tables
+ Line breaks
+ Links
+ Block quotes
+ Images
+ Math

##Markdown syntax TODO
+ Nested lists
+ Symbols
+ Underlined headings with === and --- for H1 and H2
+ Images from a url

##Required tex packages

Below is a list of tex packages which may be required to compile the created .tex files. They will only be included if they are actually required so you may not need all of the packages listed below. Also included is a short explanation as to what they are used for so you can predict which ones you will need.

+ *listings* (For code blocks)
+ *tabularx* (used for tables)
+ *xcolor* (used for shading every other table row and code highlighting)
+ *tableShade* (used for shading every other table row)
+ *geometry* (used for selecting custom margin sizes) (always required)
+ *ulem* (used for strikethroughs)
+ *graphicx* (used for images)
+ *amsmath* (used for equations)

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

###Equations

One thing latex is great at is equations. It was therefore important to allow the use of latex math. This can be done in two ways.

**Block equations**

```
$$$
\text{MyEquation} = \alpha + \beta
$$$
```

Where the dollar symbols have their own line

**Inline equations**

```
Or you can have maths inline like this $$$\alpha$$$.
```

Where the dollar symbols are part of a sentence.

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


