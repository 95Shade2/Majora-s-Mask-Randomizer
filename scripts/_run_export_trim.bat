@echo off
cd /d C:\REPOSI~1\MAJORA~1
git show HEAD:"Majora's Mask Randomizer GUI/Form1.Designer.cs" > scripts\Form1.Designer.cs.full
python scripts\trim_form1_designer.py
echo TRIM_EXIT=%ERRORLEVEL%
find /c /v "" scripts\Form1.Designer.cs.full
