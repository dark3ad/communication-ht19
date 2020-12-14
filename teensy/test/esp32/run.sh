rm -rf build && clear && mkdir -p build && cd build
gcc ../test/esp32/test.cpp ../src/esp32/esp32.cpp ../lib/canbus/canbus.cpp ../lib/canbus/candata.cpp ../lib/canbus/can_service.cpp -I../lib/canbus -I../test/esp32 -I../include -I../src/esp32 -lunity -o test_esp32_app && ./test_esp32_app

