example1
========

 Example1 is a simple plugin example w/o extra dependencies(other than for grip2).If a user builds the example1 project, then the resulting plugin library(libmyplugin.so) will be generated in lib folder.For a test purpose, lib folder already has libmyplugin.so in it.The plugin library(libmyplugin.so) can be loaded from grip2.


Directory Overview
==================

 1. src: myplugin.cpp
 
 2. include: myplugin.h
 
 3. ui: myplugin.ui
 
 4. lib: libmyplugin.so (lib folder is supposed to be empty, and will be loaded with libmyplugin.so when the project is built)


