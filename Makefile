PLUGIN_NAME = libptyxis-nautilus
CFLAGS = `pkg-config --cflags libnautilus-extension-4 gtk+-3.0`
LDFLAGS = `pkg-config --libs libnautilus-extension-4 gtk+-3.0`

all:
	gcc -shared -fPIC $(CFLAGS) -g -o $(PLUGIN_NAME).so src/$(PLUGIN_NAME).c $(LDFLAGS)

install:
	install $(PLUGIN_NAME).so $(DESTDIR)/usr/lib/x86_64-linux-gnu/nautilus/extensions-4/

clean:
	rm -f $(PLUGIN_NAME).so
