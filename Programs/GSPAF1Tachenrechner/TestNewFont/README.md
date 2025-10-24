# Beschreibung
Dieses Programm testet STM Fonts, die entweder gem�� dem Standardverfahren im Datenbereich des Programms oder die separat im internen Flash liegen.
- Es dient als Vorlage f�r die Verwendung von Fonts, die separat im internen Flash gespeichert sind.
- Es dient zur �berpr�fung neu erstellter Fonts
- Es dient als Testprogramm f�r das Display.

## Testen von STM Fonts
- Auf dem Display werden immer drei Zeichen angezeigt: Eine Referenz, das Zeichen aus dem Font im Programmspeicher und das Zeichen aus dem Font, der separat ins Flash geschrieben wurde.
- Diese Darstellung der Zeichen auf dem LCD Display kann unteranderem zur �berpr�fung und Anpassung der Positionierung der Zeichen, die vom Programm glyph2STMfont durchgef�hrt wird, genutzt werden.
- Neue Fonts werden wie folgt eingef�gt
	- In main.c werden die neuen Font Paare (Font im Programmspeicher, separat im Flash gespeicherter Font) in die Liste fontList eingetragen.
// EOF

