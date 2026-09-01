# SYS_A074 — inclusive byte-range search

Status: mechanically closed from primary firmware evidence.

## ABI

```c
const unsigned char *SYS_A074(
    const unsigned char *pStart,
    const unsigned char *pEnd,
    int chr);
```

`const` documents observed behavior: the routine only reads the searched bytes. The trap ABI itself does not encode C constness.

## Confirmed behavior

`SYS_A074` searches for the first byte equal to `(unsigned char)chr` in the **inclusive** pointer interval `[pStart, pEnd]`.

- On a match, it returns the address of that byte.
- On a miss, it returns `NULL`.
- If `pStart > pEnd` under the routine's unsigned address comparison, it returns `NULL` without reading the interval.
- `pEnd` is inclusive, not one-past-the-end.
- Only the low byte of `chr` is compared.
- A zero byte has no special terminating meaning.
- `chr == 0` can therefore match a zero byte normally.
- A zero byte before a later match does not terminate the search.
- The routine performs no pointer-validity checks.

Equivalent model:

```c
const unsigned char *SYS_A074(
    const unsigned char *pStart,
    const unsigned char *pEnd,
    int chr)
{
    unsigned char target = (unsigned char)chr;

    if (pStart > pEnd)
        return NULL;

    for (const unsigned char *p = pStart; p <= pEnd; ++p) {
        if (*p == target)
            return p;
    }

    return NULL;
}
```

The model expresses the firmware contract; ordinary C pointer ordering is only formally defined within related objects, whereas the firmware compares machine addresses directly.

## Source-first reconciliation

Historical BetaWise keeps the conservative symbol `SYS_A074` and comments it as:

`strchr equivalent (pStart, pEnd, chr)`

The three canonical firmware implementations show that **`strchr` is not an exact semantic description**. Unlike `strchr`, this routine does not treat NUL as a string terminator. It is better described as an inclusive byte-range search whose extent is supplied by two pointers.

No stronger vendor function name or public historical prototype has been recovered, so the neutral `SYS_A074` name is retained.

## Cross-generation verification

The handler is byte-identical in all three canonical images:

| Platform | Handler address | Handler size |
| --- | ---: | ---: |
| AlphaSmart 3000, 2005 | `0x004CBF5A` | `0x26` |
| NEO, 2005 | `0x005CBAB2` | `0x26` |
| NEO, 2013 | `0x0041C4D2` | `0x26` |

Handler SHA-256 in all three images:

`543897bd0663996ce770390813771b9cababe2cecff6af4b7c243022e488f5f9`

Firmware bytes and ROM images remain private and are intentionally not reproduced here.

## Calling convention details

The handler consumes three stack arguments in this order:

1. `pStart`
2. `pEnd`
3. `chr`

It preserves `A3`, uses `A1` as the current pointer, `A0` as the inclusive ending pointer, and returns the result in `D0`. The third argument is compared as a byte.

## Caller evidence

Official AlphaWordPlus 2005 and AlphaWordPlus NEO 2012 both contain the `A074` import/veneer in their A-line veneer tables. No executable caller is attributed here without a structural reference decoder; raw byte occurrence searching is not treated as caller evidence.

## Refutation checks

The primary implementation and executed static regression refute these plausible alternatives:

- **bounded `strchr` that stops at NUL:** false;
- **exclusive end pointer:** false;
- **generation-dependent behavior:** false;
- **full-width integer comparison for `chr`:** false.

## Regression status

Static regression was **executed and passed** against all three canonical images. It verifies canonical ROM hashes, exact handler extraction, byte identity, the handler hash, and synthetic model cases for first-byte match, match at `pEnd`, miss, zero-byte match, search across an intermediate zero byte, low-byte truncation of `chr`, and `pStart > pEnd`.

A dynamic hardware/emulator probe has not been executed; it is not required for the mechanical closure documented here.
