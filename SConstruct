env = Environment()

mymode = ARGUMENTS.get('mode','release')
if not (mymode in ['debug','release']):
    print('Error: expected debug or release in mode')
    Exit(1)
mycompiler = ARGUMENTS.get('cxx','clang++')

env.Replace(CXX = mycompiler)

if mymode == 'debug':
    env.Replace(CXXFLAGS = '-std=c++11 -g -DDEBUG')
else:
    env.Replace(CXXFLAGS = '-std=c++11 -O3')

env.Program('network', Glob('./*.cpp'));
