#!/bin/bash

# Check if input file is provided
if [ -z "$1" ]; then
  echo "Usage: $0 <input_file>"
  exit 1
fi

# Read the file and replace Latin characters with Cyrillic equivalents
sed -i \
-e 's/a/а/g' -e 's/b/б/g' -e 's/c/к/g' -e 's/d/д/g' -e 's/e/е/g' \
-e 's/f/ф/g' -e 's/g/г/g' -e 's/h/х/g' -e 's/i/и/g' -e 's/j/й/g' \
-e 's/k/к/g' -e 's/l/л/g' -e 's/m/м/g' -e 's/n/н/g' -e 's/o/о/g' \
-e 's/p/п/g' -e 's/q/к/g' -e 's/r/р/g' -e 's/s/с/g' -e 's/t/т/g' \
-e 's/u/у/g' -e 's/v/в/g' -e 's/w/в/g' -e 's/x/кс/g' -e 's/y/й/g' \
-e 's/z/з/g' "$1"

echo "Transliteration complete: $1"
