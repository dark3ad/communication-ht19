rm -rf ../../build && clear && mkdir -p ../../build && cd ../../build
gcc ../test/esp32/esp32_test.cpp ../src/esp32/esp32.cpp -I../test/esp32 -I../include -I../src/esp32 -lunity -o test_esp32_app && ./test_esp32_app

