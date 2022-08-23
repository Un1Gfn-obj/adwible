O:=builddir
N:=ninja -j4 -v -C $(O)
# DEBUGINFOD_TIMEOUT=0
# G_DEBUG=fatal-warnings
# debuginfo(8)
# GOBJECT_DEBUG=instance-count 
E:=env HTTPS_PROXY=http://127.0.0.1:8080/ DEBUGINFOD_URLS="/dev/null"
V:=/bin/valgrind -v --keep-debuginfo=yes --leak-check=full --suppressions=/usr/share/gtk-4.0/valgrind/gtk.supp
VL:=/tmp/valgrind.log

default: b

# postbuild.transfer
t: $(O)/adwible
	ssh user@wt88047usb rm -fv /tmp/$^
	scp $^ user@wt88047usb:/tmp/

# postbuild.valgrind
v: b
	@echo ":; less -SRM +% -p 'HEAP SUMMARY' /tmp/valgrind.log"
	@echo ":; tail -f -c +0 $(VL)"
	$(E) $(V) --log-file=$(VL) $(O)/adwible

# postbuild.run
r: b
# 	find $(O)/
# 	https://unix.stackexchange.com/questions/438117
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
	@rm -fv cscope.out
mrproper:
	@rm -rfv $(O)/
distclean:
	@rm -rfv $(O)/
