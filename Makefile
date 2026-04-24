APPNAME	= BLDCWindingCalculator
BIN	= /usr/local/bin
LOCALE	= /usr/share/locale
CXXFLAGS= -O3 -DNDEBUG -DAPPNAME=\"$(APPNAME)\" -DLOCALE=\"$(LOCALE)\" -s -std=c++20
TARGETS	= $(APPNAME) locale/ru/LC_MESSAGES/$(APPNAME).po locale/ru/LC_MESSAGES/$(APPNAME).mo

.PHONY: all clean install uninstall

all: $(TARGETS)

clean:
	rm -rf *.o $(TARGETS)
install: $(APPNAME) locale/ru/LC_MESSAGES/$(APPNAME).mo
	install $(APPNAME) $(BIN)
	install -m644 locale/ru/LC_MESSAGES/$(APPNAME).mo $(LOCALE)/ru/LC_MESSAGES
uninstall:
	rm -rf $(BIN)/$(APPNAME) $(LOCALE)/ru/LC_MESSAGES/$(APPNAME).mo


locale/ru/LC_MESSAGES/$(APPNAME).pot: $(APPNAME).cpp
	xgettext --keyword=_ --from-code=UTF-8 --package-name $(APPNAME) --package-version 0.1 --default-domain $(APPNAME) \
		 --omit-header --no-location --sort-output --join-existing --output $@ $<
	xgettext             --from-code=UTF-8 --package-name $(APPNAME) --package-version 0.1 --default-domain $(APPNAME) \
		 --omit-header --no-location --sort-output --join-existing --output $@ $<
%.po: %.pot
	msginit --no-translator --locale ru.utf8 --output-file $@ --input $<
%.mo: %.po
	msgfmt --check --verbose --output-file $@ $<
