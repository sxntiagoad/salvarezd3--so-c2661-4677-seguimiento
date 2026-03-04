#!/bin/bash

if [ $# -ne 2 ]; then
    echo "Uso: $0 <archivo> <columna>"
    exit 1
fi

archivo=$1
columna=$2

cut -d'|' -f$columna "$archivo" | grep -v '^$' | sort | uniq
