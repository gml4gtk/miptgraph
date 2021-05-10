You can find test dumps in /tests folders.
If you want to create your own compiler dump, you may use:
gcc -c -df file.c for GCC3
or

icc file.c -mP2OPT_il0_print=<num>
num = 6, 2510, -1 for ICC

If you have got GCC4 compiler you may try:
gcc -c -da file.c
and select file.c.r138.jump dump to parsing