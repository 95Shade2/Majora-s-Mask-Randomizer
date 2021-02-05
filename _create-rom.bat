@echo off
title Randomizer ROM Creation
cd _create-roms
set ini=injector.ini
echo > "%ini%" region=3
echo >> "%ini%" randomticket=0
echo >> "%ini%" ticketname=NMRE
echo >> "%ini%" delaytime=0
compress "..\mm2.z64" bmim.z64
romc0 bmim.z64 bmim_vc.z64
ren "bmim.z64" "Legend of Zelda, The - Majora's Mask - Randomizer (U).z64"
move "Legend of Zelda, The - Majora's Mask - Randomizer (U).z64" "..\"
del ARCHIVE.bin
del bmim_vc.z64