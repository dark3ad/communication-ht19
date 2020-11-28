import os
import sys

node = 'com'
mode = 'development'


def print_error():
    print("\nError...")
    print('Usage: The available flag and arguments')
    print('python generate.py [-mode <production|test>]')


if len(sys.argv) == 3 or len(sys.argv) == 4:
    if sys.argv[1] != '-mode':
        print_error()
        exit(1)
    elif sys.argv[2] not in ['development', 'production', 'test']:
        print_error()
        exit(2)
    mode = sys.argv[2]
elif len(sys.argv) != 1:
    print_error()
    exit(3)

branch = 'develop'
if mode == 'production':
    branch = 'master'

os.system('git submodule init')
os.system('git submodule update')
os.system('cd canbus && git checkout {0} && git pull'.format(branch))

os.system(
    'cd canbus && python canbus.py -node {0} -mode {1}'.format(node, mode))
print('The CAN interface was generated in the {0} mode'.format(mode.upper()))
