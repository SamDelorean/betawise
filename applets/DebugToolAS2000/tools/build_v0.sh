#!/bin/sh
set -eu

ROOT=$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)
OUT="${1:-/tmp/debugtool-as2000-v0}"
PREFIX="${M68HC11_PREFIX:-$HOME/.local/m68hc1x-binutils/usr/bin}"
AS="$PREFIX/m68hc11-as"
LD="$PREFIX/m68hc11-ld"
OBJCOPY="$PREFIX/m68hc11-objcopy"
SIZE="$PREFIX/m68hc11-size"
NM="$PREFIX/m68hc11-nm"

rm -rf "$OUT"
mkdir -p "$OUT"

if grep -En '0x[0-9A-Fa-f]{4}|\$[0-9A-Fa-f]{4}' \
  "$ROOT/core/debugtool_core.asm" \
  "$ROOT/include/debugtool_state.inc" \
  "$ROOT/include/debugtool_bindings.inc"; then
  echo "ERROR: absolute 16-bit address leaked into portable core/interface" >&2
  exit 1
fi

INC="-I$ROOT/include"
"$AS" -m68hc11 $INC -o "$OUT/core.o" "$ROOT/core/debugtool_core.asm"
"$AS" -m68hc11 $INC -o "$OUT/binding.o" "$ROOT/bindings/diagnostic_mame.asm"

cat >"$OUT/v0.ld" <<'EOF'
SECTIONS
{
  .text 0xA000 : { *(.text) }
  .bss 0x7000 (NOLOAD) : { *(.bss) }
}
EOF

"$LD" -T "$OUT/v0.ld" -e DEBUGTOOL_ENTRY -o "$OUT/v0.elf"   "$OUT/core.o" "$OUT/binding.o"
"$OBJCOPY" -O binary -j .text "$OUT/v0.elf" "$OUT/v0.bin"

"$SIZE" -A "$OUT/core.o"
"$SIZE" -A "$OUT/binding.o"
"$SIZE" -A "$OUT/v0.elf"
"$NM" -n "$OUT/v0.elf" >"$OUT/symbols.txt"

sha256sum "$OUT/core.o" "$OUT/binding.o" "$OUT/v0.elf" "$OUT/v0.bin"   >"$OUT/SHA256SUMS"

cat "$OUT/SHA256SUMS"
echo "V0_BUILD_PASS"
echo "Synthetic link map only: .text=A000 .bss=7000"
