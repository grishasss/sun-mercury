#!/bin/sh



pdflatex --shell-escape --enable-write18 1.tex

pdftoppm 1.pdf outputname -png
