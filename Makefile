APPNAME	= BLDCWindingCalculator
PREFIX	= /usr/local/bin
CXXFLAGS= -O3 -DNDEBUG -DAPPNAME=\"$(APPNAME)\" -s -std=c++20
#TARGETS	= $(APPNAME) $(APPNAME).pot ru/LC_MESSAGES/$(APPNAME).pot ru/LC_MESSAGES/$(APPNAME).mo
TARGETS	= $(APPNAME) ru/LC_MESSAGES/$(APPNAME).po ru/LC_MESSAGES/$(APPNAME).mo

.PHONY: all clean install uninstall

all: $(TARGETS)

clean:
	rm -rf *.o $(TARGETS)
install:
	install $(APPNAME) $(PREFIX)
uninstall:
	rm -rf $(PREFIX)/$(APPNAME)

ru/LC_MESSAGES/$(APPNAME).pot: $(APPNAME).cpp
	@mkdir --parents ru/LC_MESSAGES
	xgettext --from-code=UTF-8 --keyword=_ --package-name $(APPNAME) --package-version 0.1 --default-domain $(APPNAME) \
		 --no-location --no-wrap --sort-output --join-existing --output $@ $<
%.po: %.pot
	msginit --no-translator --locale ru.utf8 --output-file $@ --input $<
%.mo: %.po
	msgfmt --check --verbose --output-file $@ $<
