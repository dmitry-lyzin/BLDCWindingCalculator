APPNAME	= BLDCWindingCalculator
VERSION	= 0.1.0-alpha
WINREL	= $(APPNAME)-$(VERSION)-win32.zip
BIN	= /usr/local/bin
LOCALE	= /usr/share/locale
CXXFLAGS= -O3 -DNDEBUG -DAPPNAME=\"$(APPNAME)\" -DVERSION=\"$(VERSION)\" -DLOCALE=\"$(LOCALE)\" -s -std=c++20 -Wno-literal-suffix
TARGETS	= $(APPNAME) locale/ru/LC_MESSAGES/$(APPNAME).po locale/ru/LC_MESSAGES/$(APPNAME).mo
CP	= "C:\Program Files\Git\usr\bin\cp.exe"


.PHONY: all clean install uninstall windows

all: $(TARGETS)

clean:
	$(RM) *.o $(TARGETS)
install: $(APPNAME) locale/ru/LC_MESSAGES/$(APPNAME).mo
	install $(APPNAME) $(BIN)
	install -m644 locale/ru/LC_MESSAGES/$(APPNAME).mo $(LOCALE)/ru/LC_MESSAGES
uninstall:
	$(RM) $(BIN)/$(APPNAME) $(LOCALE)/ru/LC_MESSAGES/$(APPNAME).mo


locale/ru/LC_MESSAGES/$(APPNAME).pot: $(APPNAME).cpp
	xgettext --keyword=_ --from-code=UTF-8 --package-name $(APPNAME) --package-version $(VERSION) \
		 --default-domain $(APPNAME) --omit-header --no-location --sort-output --join-existing --output $@ $<
	xgettext             --from-code=UTF-8 --package-name $(APPNAME) --package-version $(VERSION) \
		 --default-domain $(APPNAME) --omit-header --no-location --sort-output --join-existing --output $@ $<
%.po: %.pot
	msginit --no-translator --locale ru.utf8 --output-file $@ --input $<
%.mo: %.po
	msgfmt --check --verbose --output-file $@ $<

windows: Release/$(WINREL)

Release/$(WINREL): Release/$(APPNAME).exe Release/$(APPNAME)ForNoob.cmd Release/locale/ru/LC_MESSAGES/$(APPNAME).mo
	$(RM) $@
	cmd /C cd Release && 7z a -r $(WINREL) \
					$(APPNAME).exe \
					$(APPNAME)ForNoob.cmd \
					locale/ru/LC_MESSAGES/$(APPNAME).mo

Release/locale/ru/LC_MESSAGES/%: locale/ru/LC_MESSAGES/%
	$(CP) --parents $< Release
