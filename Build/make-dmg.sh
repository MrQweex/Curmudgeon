#Run make clean in ../../build-Curmudgeon-* && rm Makefile
hdiutil create /tmp/tmp.dmg -ov -volname "Curmudgeon_1.0" -fs HFS+ -srcfolder ../../build-Curmudgeon-Desktop-Debug
hdiutil convert /tmp/tmp.dmg -format UDZO -o Curmudgeon_1.0.dmg
