#!/bin/bash
# Indent: tab, Tabsize: 4, Encoding: UTF-8, Language: Make, Breaks: linux
################################################################################
#
# This file is part of To-mate-Oh.
# Copyright © 2014 con-f-use.
# Use under GNU General Public License v3.0 or later.
#
################################################################################

doxygen doxygen.cfg
cd latex
sed -i 's/pdflatex/pdflatex \-interaction\=nonstopmode/g' Makefile
make
cd ..
cp latex/refman.pdf docu.pdf
rm -r latex
