import os
import sys

node = 'com'
mode = 'development'

if len(sys.argv) == 3 or len(sys.argv) == 4:
    if sys.argv[1] != '-mode':
        print("\nError...")
        print('Usage: The available flag and arguments')
        print('1) python generate.py')
        print('2) python generate.py -mode production')
        print('3) python generate.py -mode development')
        exit(1)
    elif sys.argv[2] != 'development' and sys.argv[2] != 'production':
        print("\nError...")
        print('Usage: The available flag and arguments')
        print('1) python generate.py')
        print('2) python generate.py -mode production')
        print('3) python generate.py -mode development')
        exit(2)
elif len(sys.argv) != 1:
    print("\nError...")
    print('Usage: The available flag and arguments')
    print('1) python generate.py')
    print('2) python generate.py -mode production')
    print('3) python generate.py -mode development')
    exit(3)

os.system('git submodule init')
os.system('git submodule update')
os.system('cd canbus && git checkout master && git pull')

os.system('cd canbus && python canbus.py -node {0} -mode {1}'.format(node, mode))
print('The CAN interface was generated in the {0} mode'.format(mode.upper()))
