rm -rf ../../build && clear && mkdir -p ../../build && cd ../../build
gcc ../lib/canbus/canbus.cpp ../lib/canbus/can_service.cpp ../test/terminal/test.cpp ../src/terminal/terminal.cpp -I../test/terminal -I../include -I../lib/canbus -I../lib/sdcard -I../src/terminal -lunity -o terminal && ./terminal
