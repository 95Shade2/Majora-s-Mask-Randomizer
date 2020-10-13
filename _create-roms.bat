@echo off
title Randomizer ROM/WAD Creation
cd _create-roms
set ini=injector.ini
echo > "%ini%" region=3
echo >> "%ini%" randomticket=0
echo >> "%ini%" ticketname=NMRE
echo >> "%ini%" delaytime=0
compress "..\mm2.z64" bmim.z64
romc0 bmim.z64 bmim_vc.z64
injectuwad bmim_vc.z64 mmr.wad "Randomizer" 2 n
ren "bmim.z64" "Legend of Zelda, The - Majora's Mask - Randomizer (U).z64"
ren "VC-Randomizer in mmr.wad" "Legend of Zelda, The - Majora's Mask - Randomizer (U).wad"
move "Legend of Zelda, The - Majora's Mask - Randomizer (U).z64" "..\"
move "Legend of Zelda, The - Majora's Mask - Randomizer (U).wad" "..\"
del ARCHIVE.bin
del bmim_vc.z64
del 00000000.app
del 00000001.app
del 00000002.app
del 00000003.app
del 00000004.app
del 00000005.app
del 00000006.app
del 00000007.app
del title.cert
del title.tik
del title.tmd
del sha1out.txt
del InjectorLog.txt