# SDK audit: Clipboard header repair rollback

## Material finding

The attempted A20C/A210/A214 public-header integration commit `398b94545845363bbc651569feebc21ef83483ed` contained an unrelated textual regression in `APPLET_HEADER_BEGIN`: the continuation backslashes after `.fileUsage = 0` and `.entryPoint = &BwProcessMessage` were removed.

That changes the preprocessor structure of the applet header macro and is not an acceptable ABI/header-only Clipboard delta.

## Action

The branch `sdk/abi-automation` was restored to its immediately preceding safe commit `85fd6df2b5a8efef1c6e99f17390afbeaefa71b3`. Therefore the malformed header commit is no longer part of branch history.

The Clipboard audit conclusion itself is unchanged: A20C/A210/A214 are the narrow historical public-declaration candidate, and the architecture-correct repair remains direct declarations in `os3k.h` matching `file_clipboard_buffer.h`, without including the reconstructed focused header.

A future application must preserve the rest of `os3k.h` byte-for-byte except for the intended declaration insertion and should be checked by a zero-unrelated-diff gate before publication.

## Build status

The failed integration was not built and must never be treated as `BUILD_VALIDATED`. No new source/header delta remains on the branch after rollback. The eventual corrected header edit remains `PENDING_M68K_BUILD` until `make -C os3k clean all` and a representative applet clean build are executed with the m68k toolchain.

No `CERRADA_CON_LÍMITE_DE_EVIDENCIA` contract was promoted and no closed syscall was reopened.