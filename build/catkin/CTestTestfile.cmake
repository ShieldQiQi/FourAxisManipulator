# CMake generated Testfile for 
# Source directory: /home/pi/catkin_qi/src/catkin
# Build directory: /home/pi/catkin_qi/build/catkin
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(_ctest_catkin_nosetests_test.local_tests "/home/pi/catkin_qi/build/catkin_generated/env_cached.sh" "/usr/bin/python" "/home/pi/catkin_qi/src/catkin/cmake/test/run_tests.py" "/home/pi/catkin_qi/build/test_results/catkin/nosetests-test.local_tests.xml" "--return-code" "\"/usr/bin/cmake\" -E make_directory /home/pi/catkin_qi/build/test_results/catkin" "/usr/bin/nosetests-2.7 -P --process-timeout=60 --where=/home/pi/catkin_qi/src/catkin/test/local_tests --with-xunit --xunit-file=/home/pi/catkin_qi/build/test_results/catkin/nosetests-test.local_tests.xml")
add_test(_ctest_catkin_nosetests_test.unit_tests "/home/pi/catkin_qi/build/catkin_generated/env_cached.sh" "/usr/bin/python" "/home/pi/catkin_qi/src/catkin/cmake/test/run_tests.py" "/home/pi/catkin_qi/build/test_results/catkin/nosetests-test.unit_tests.xml" "--return-code" "\"/usr/bin/cmake\" -E make_directory /home/pi/catkin_qi/build/test_results/catkin" "/usr/bin/nosetests-2.7 -P --process-timeout=60 --where=/home/pi/catkin_qi/src/catkin/test/unit_tests --with-xunit --xunit-file=/home/pi/catkin_qi/build/test_results/catkin/nosetests-test.unit_tests.xml")
