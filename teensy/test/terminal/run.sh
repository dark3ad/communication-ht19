rm -rf ../../build && clear && mkdir -p ../../build && cd ../../build
gcc ../test/terminal/test.cpp ../src/terminal/terminal.cpp -I../test/terminal -I../include -I../lib/canbus -I../src/terminal -lunity -o terminal && ./terminal
