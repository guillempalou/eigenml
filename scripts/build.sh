PROJECT_ROOT = `dirname $0`
PROJECT_BINARY = ``

mkdir build
cmake ..
make
./$PROJECT_BINARY