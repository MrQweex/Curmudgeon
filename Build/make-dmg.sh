hdiutil create /tmp/tmp.dmg -ov -volname "Curmudgeon-1.0" -fs HFS+ -srcfolder ../../build-Curmudgeon-Desktop-Debug
hdiutil convert /tmp/tmp.dmg -format UDZO -o Curmudgeon.dmg
