@echo off
cd /d C:\REPOSI~1\MAJORA~1
git show HEAD:"Majora's Mask Randomizer GUI/Form1.Designer.cs" > MMR_GUI\Form1.Designer.cs.full
python scripts\trim_form1_designer.py
cmake --build build --config debug --target mm_rando_gui
exit /b %ERRORLEVEL%
