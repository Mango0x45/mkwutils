mkwutils - A collection of Mario Kart Wii utilities
===================================================

`mkwutils` is a collection of utility programs for Mario Kart Wii players that aid in manipulating
savefiles, ghosts, miis, etc. All of the programs are written in C targetted towards Unix-like
systems. The programs have been tested primarily on Linux, but also on various BSDs and on
big-endian systems aswell.

Dependencies
------------

* `GCC`, `Clang`, or some other C compiler that supports C11.
* `make`
	* Not required, but will make compilation and installation much easier.
* `gperf`

Utilities
---------

| Name                                      | Description                                                                    |
|-------------------------------------------|--------------------------------------------------------------------------------|
| `db2rkg` - **D**ata**b**ase to **RKG**    | Convert a ghost database link to a link directly to the ghost file.            |
| `rrc` - **R**ksys **R**egion **C**changer | Change a savefiles region, useful for using other region savefiles in Dolphin. |
| `gm` - **G**host **M**anager              | View, import, and export ghosts to and from savefiles.                         |
