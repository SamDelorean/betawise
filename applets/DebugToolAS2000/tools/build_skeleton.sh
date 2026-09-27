#!/bin/sh
set -eu

ROOT=$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)
OUT="${1:-/tmp/debugtool-as2000-skeleton}"
PREFIX="${M68HC11_PREFIX:-$HOME/.local/m68hc1x-binutils/usr/bin}"
AS="$PREFIX/m68hc11-as"
LD="$PREFIX/m68hc11-ld"
SIZE="$PREFIX/m68hc11-size"
NM="$PREFIX/m68hc11-nm"

rm -rf "$OUT"
mkdir -p "$OUT"

INC="-I$ROOT/include"

# Portable core/interface must not contain 16-bit absolute addresses.
if grep -En '0x[0-9A-Fa-f]{4}|\\$[0-9A-Fa-f]{4}' \
  "$ROOT/core/debugtool_core.asm" \
  "$ROOT/include/debugtool_state.inc" \
  "$ROOT/include/debugtool_bindings.inc"; then
  echo "ERROR: absolute 16-bit address leaked into portable core/interface" >&2
  exit 1
fi

"$AS" -m68hc11 $INC -o "$OUT/core.o" "$ROOT/core/debugtool_core.asm"
"$AS" -m68hc11 $INC -o "$OUT/binding.o" "$ROOT/bindings/diagnostic_mame.asm"
"$AS" -m68hc11 $INC -o "$OUT/smoke.o" "$ROOT/tests/binding_smoke.asm"

cat >"$OUT/skeleton.ld" <<'EOF'
SECTIONS
{
  .text 0xA000 : { *(.text) }
  .bindcheck 0x6000 : { *(.bindcheck) }
  .bss 0x7000 (NOLOAD) : { *(.bss) }
}
EOF

"$LD" -T "$OUT/skeleton.ld" -e DEBUGTOOL_ENTRY -o "$OUT/skeleton.elf"   "$OUT/core.o" "$OUT/binding.o" "$OUT/smoke.o"

"$SIZE" -A "$OUT/core.o"
"$SIZE" -A "$OUT/binding.o"
"$SIZE" -A "$OUT/smoke.o"
"$SIZE" -A "$OUT/skeleton.elf"
"$NM" -n "$OUT/skeleton.elf" >"$OUT/symbols.txt"

grep -q ' DEBUGTOOL_ENTRY$' "$OUT/symbols.txt"
grep -q ' DBG_WS_BASE$' "$OUT/symbols.txt"
grep -q ' DBG_BIND_DYNFS_ACTIVE_CTX$' "$OUT/symbols.txt"

echo "SKELETON_BINDING_PASS"
echo "Synthetic link map only: .text=A000 .bindcheck=6000 .bss=7000"
