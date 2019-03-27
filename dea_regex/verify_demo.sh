#!/bin/sh
./builddir/process "`cat verifier.txt | tr -d " \t\n\r"`" "`./builddir/contains "Hal\.lo"`"
