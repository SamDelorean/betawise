# No-Z DictROM takeover integration target

The portable DebugTool-AS2000 v0 core is now selected as the first real
user-facing payload for the AS2000 no-Z DictROM takeover project.

The existing portable core remains authoritative and must not be forked.

Current v0:

- entry: `DEBUGTOOL_ENTRY`
- core: 1175 bytes
- diagnostic binding: 102 bytes
- workspace: 30 bytes
- functions: MEM / GOTO / EDIT / CALL / INFO

The existing diagnostic/current-ROM bindings are **not** suitable unchanged for
the takeover build because they call stock ROM services.

A future takeover binding must replace those dependencies with independently
authored/direct hardware adapters, including an internal-RAM gateway for
external RAM access while code executes from DictROM.

No portable-core rewrite is authorized by this decision.

Integration sequencing is controlled by
`SamDelorean/AS2K-DictROM-Takeover/docs/DEBUGTOOL_TAKEOVER_PAYLOAD.md`.
