rm -rf ../../build && clear && mkdir -p ../../build && cd ../../build
gcc ../test/led_driver/test.cpp ../lib/led_driver/led_driver.cpp -I../test/led_driver -I../lib/led_driver -lunity -D TEST -o led_driver && ./led_driver
