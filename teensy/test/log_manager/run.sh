rm -rf build && clear && mkdir -p build && cd ./build
g++ --coverage ../test/log_manager/test.cpp ../src/log_manager/log_manager.cpp ../lib/canbus/canbus.cpp ../lib/canbus/candata.cpp ../lib/canbus/can_service.cpp -I../lib/canbus -I../src/log_manager/ -I../test/log_manager -I../include -lunity -o log_manager && ./log_manager
lcov --directory . --capture --output-file app.info
genhtml --output-directory cov_html app.info
