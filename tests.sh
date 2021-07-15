#!/bin/bash

echo "--> Starting first open test"
./main < tests/open/input_1 > output
python compare.py output tests/open/output_1
echo "--> Starting second open test"
./main < tests/open/input_2 > output
python compare.py output tests/open/output_2
echo "--> Starting third open test"
./main < tests/open/input_3 > output
python compare.py output tests/open/output_3
echo "--> Starting fourth open test"
./main < tests/open/input_4 > output
python compare.py output tests/open/output_4
echo "--> Starting fifth open test"
./main < tests/open/input_5 > output
python compare.py output tests/open/output_5
echo "--> Starting sixth open test"
./main < tests/open/input_6 > output
python compare.py output tests/open/output_6

echo "--> Starting first private test"
./main < tests/private/input_1 > output
echo "--> Starting second private test"
./main < tests/private/input_2 > output
echo "--> Starting third private test"
./main < tests/private/input_3 > output
echo "--> Starting fourth private test"
./main < tests/private/input_4 > output
echo "--> Starting fifth private test"
./main < tests/private/input_5 > output
echo "--> Starting sixth private test"
./main < tests/private/input_6 > output