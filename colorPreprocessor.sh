
cat colorData.txt | sed 's/\\/\\\\/g' | tr '\n' ' ' | tr '\r' ' ' | sed 's/^/"/g' | sed 's/$/"/g' > src/colorData.txt
