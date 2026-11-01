# Weapon 17 native oracles (cloud session, 2026-11-01)

These build on Codex's harness. Copy this folder next to
`validation/native_inventory_check.c`, `native_weapon_init_check.c` and their
includes, so it sits inside `validation/`, then compile from there:

    gcc -O0 -w -I.. -o probe probe_check.c
    gcc -O0 -w -I.. -o coll coll_check.c
    gcc -O0 -w -I.. -o e2e e2e_check.c
    python3 mk.py
    ./e2e <native sections dir> --detail c_detail.bin

The probe takes the reader address, a record size and an input file. Add an
`r` to the address for readers called as (reader, record), for example
`9AAF50r`.

These files contain no game bytes. The native sections stay private.
