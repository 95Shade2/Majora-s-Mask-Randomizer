@echo off
title ndec
set /P infile=ROM File: 
set /P outfile=New ROM File: 
ndec %infile% %outfile% /clean