# A338 — abort-like control transfer; exact C prototype blocked

Status: **mechanically closed (A) / exact C prototype blocked**.

A338 is A-line index 206. The three canonical firmware generations implement the same 0x10-byte body after relocation of the current-applet global and one internal control-transfer target.

Mechanically, the service sets bit 7 in the current-applet runtime byte and calls an internal trampoline. That trampoline replaces A7 with the platform stack-top value, clears A6, and jumps to an internal system entry point; it has no RTS. Consequently the ordinary return address created by the A338 call is abandoned. A338 therefore has no normal call/return result path even though an RTS instruction follows the trampoline call syntactically.

The same bit-7 + trampoline sequence occurs in the independently reconstructed KEY_APPLETS path of A330, strongly corroborating an applet-termination/system-transfer role. Historical BetaWise maps index 206 to `abort` and declares `void abort(void)`, so an abort-like identity is a strong inference; no independent vendor symbol evidence is promoted here.

## Caller discrepancy

The official SmartApplet corpus was exhausted: 30 applets with the relevant A-line table plus 11 structural negatives. Exactly two executable A338 callers were found, one in the AS3000 KeyWords applet and one in the NEO Keywords applet.

Both callers explicitly push one zero longword immediately before A338 and clean four bytes immediately afterward. The A338 handler itself never reads caller stack data. Thus the corpus proves an observed caller pattern containing a zero slot, but it does not prove that the slot is semantically consumed or required. This conflicts with the historical BetaWise `abort(void)` prototype.

For that reason this forward-sweep closure does **not** publish a new callable header or normalize the discrepancy away. The mechanically secure facts are: no consumed stack data in the handler, an abort-like non-returning control transfer through the internal trampoline, and two official callers that nevertheless supply an ignored zero longword.

The relocation-normalized handler SHA-256 is `4de38f1f1ecc29b60db43ffe1cb76393423d42f0e2724a2c9344762aa4a3ffa3`.

Static regression: **OVERALL PASS for mechanics**. Exact C prototype: **blocked**. Dynamic regression: **not executed**.

Private workpapers retain canonical ROM hashes, exact addresses, per-ROM disassembly, trampoline correlation, ROM-wide xrefs, the 41/41 caller survey, and the adversarial prototype analysis.
