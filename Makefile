include ./transferconf
N:=ninja -j4 -v -C $(O)

default: run-ui

run-ui: build
	env G_DEBUG=fatal-warnings $(O)/$(BIN)

# order-only prerequisite
# no rebuild on outdate
build: | $(O)
# 	$(N)
	meson compile -v -C $(O)

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
