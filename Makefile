H:=wt88047usb
# H:=wt88047wlan0

O:=builddir.$(shell uname -m)
N:=ninja -j4 -v -C $(O)
# DEBUGINFOD_TIMEOUT=0
# G_DEBUG=fatal-warnings
# debuginfo(8)
# GOBJECT_DEBUG=instance-count 

VL:=/tmp/valgrind.log

E:=HTTPS_PROXY=http://127.0.0.1:8080/ 
# E+=DEBUGINFOD_URLS="/dev/null"
V:=/bin/valgrind --keep-debuginfo=yes --suppressions=/usr/share/gtk-4.0/valgrind/gtk.supp --suppressions=/usr/share/glib-2.0/valgrind/glib.supp
V+=-v
# V+=--leak-check=full
V+=--log-file=$(VL)

default: b

# postbuild.transfer
t: builddir.aarch64/adwible
# 	ip -o -4 addr show dev enp0s20f0u2 | grep inet >/dev/null || sudo dhcpcd enp0s20f0u2
	ssh root@$(H) rm -fv /usr/local/bin/$^
	scp $^ root@$(H):/usr/local/bin/

# postbuild.valgrind
v: b
# 	@rm -iv /tmp/adwible.bin || true
	@echo ":; less -SRM +% -p 'HEAP SUMMARY' /tmp/valgrind.log"
	@echo ":; tail -f -c +0 $(VL)"
	$(E) $(V) $(O)/adwible

# postbuild.gdb
g: b
# 	@rm -iv /tmp/adwible.bin || true
	$(E) gdb $(O)/adwible

# postbuild.run
r: b
# 	find $(O)/
# 	https://unix.stackexchange.com/questions/438117
# 	@rm -iv /tmp/adwible.bin || true
	@rm -fv $(VL)
	$(E) $(O)/adwible

# order-only prerequisite
# no rebuild on outdate
b: | $(O)
# 	meson compile -v -C $(O)
	$(N)

setup: | $(O)

$(O):
	@echo "/Makefile --> /meson.build --> /builddir/build.ninja"
	@rm -rf $(O)
	meson setup $(O) .

# https://git.kernel.org/pub/scm/linux/kernel/git/torvalds/linux.git/tree/Makefile?h=v5.19#n1561
clean:
	{ [ -e $(O) ] && $(N) -t clean; } || true
	rm -fv cscope.out
mrproper:
	rm -rfv $(O)/
distclean:
	rm -rfv $(O)/

# gtk4-docs
d:
	/usr/local/bin/doc.sh
