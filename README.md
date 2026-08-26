BetaWise
========

BetaWise is a development environment and evolving SDK for AlphaSmart System 3 / OS3K SmartApplets. It targets AlphaSmart 3000, NEO, and NEO2 while preserving platform-specific differences where they matter.

The project combines:

* an `m68k-elf` toolchain;
* the BetaWise OS3K support library and linker setup;
* example/custom SmartApplets;
* documented A-line/System 3 interfaces;
* ABI research probes intended for emulator and hardware validation.

SDK / ABI Documentation
-----------------------

See:

* `docs/ABI.md` — documented OS3K calls and confidence levels;
* `docs/COMPATIBILITY.md` — AS3000 vs NEO/NEO2 compatibility model;
* `docs/RESEARCH_STATUS.md` — confirmed, probable, and unresolved interfaces.

Research probes live under `tests/applets/`. They are intentionally kept separate from normal end-user applets.

Compiling
---------

Any `m68k-elf` GCC cross compiler should work. On Windows, MinGW GCC for M68K has been used successfully.

Normal build:

```
make
```

For ABI/reverse-engineering work, compiler intermediate files can be retained with:

```
make SAVE_TEMPS=1
```

This asks GCC to preserve files such as preprocessed `.i` and assembly `.s` output alongside `.o` and the final `.OS3KApp`. These generated artifacts are ignored by Git by default.

ABI probe applets are not part of the default top-level build. Build them explicitly from their directory, for example:

```
make -C tests/applets/DialogProbe SAVE_TEMPS=1
```

Installing Custom Applets
-------------------------

Using Neo Manager:

* File → Add to Applet List... → select `[AppletFile].OS3KApp`
* SmartApplets tab → select `[AppletName]` → Add
  * Optional: checking *Delete SmartApplets that are not in the Install List…* saves a keypress or two during debug (**will delete existing applets**)
* Send List tab → Send

On Windows 7 x64, Neo Manager stores custom applets in:

`%LocalAppData%\VirtualStore\Program Files (x86)\AlphaSmart\AlphaSmart Manager 2\SmartApplets`

Tips
----

* To start in the applet chooser, hold `Left Shift` + `Tab` while turning on.
* To reset to defaults, hold `Right Shift` + `Backspace` while turning on. The reset password is `tommy`.

Research Method
---------------

A-line interfaces are promoted into the documented SDK only when their contract is supported by evidence. Sources include historical AlphaSmart code, firmware/Ghidra analysis, existing BetaWise use, emulator traces, minimal probe applets, and physical AS3000/NEO validation.

Emulator and SDK development deliberately feed each other: emulator traces can clarify ABI behavior, while BetaWise probes become regression cases for the emulator.

Work in Progress
----------------

Major ongoing areas include:

* improve stdio compatibility across character and graphical display hardware;
* investigate `ProcessMessage` statuses and additional messages;
* finish timer API reconstruction;
* finish non-message keyboard and special-key processing;
* reconstruct file and clipboard APIs already partially identified in the syscall table;
* continue replacing raw `SYS_Axxx` entries with evidence-backed interfaces;
* reduce or eliminate the need to place all applet global data into one structure.

Contributions and reproducible observations are welcome.
