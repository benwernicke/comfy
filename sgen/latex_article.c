#include "latex_article.h"

static char article[] = 
    "\\documentclass{article}\n"
    "\n"
    "\\usepackage{enumitem}\n"
    "\\usepackage{blindtext}\n"
    "\\usepackage[document]{ragged2e}\n"
    "\n"
    "\\renewcommand\\thesection{\\Roman{section}} %% Roman numerals for the sections\n"
    "\\renewcommand\\thesubsection{\\roman{subsection}} %% roman numerals for subsections\n"
    "\n"
    "\\title{Article Title}\n"
    "\\author{Ben Wernicke}\n"
    "\\date{\\today}\n"
    "\n"
    "\\begin{document}\n"
    "    \\maketitle\n"
    "\n"
    "    \\center\n"
    "    \\section*{Abstract}\n"
    "    \\justifying\n"
    "    Some very nice abstract text\n"
    "    \\bigskip\n"
    "\n"
    "    \\section{First Section}\n"
    "\\end{document}\n";

void latex_article(char** argv)
{
    puts(article);
}
