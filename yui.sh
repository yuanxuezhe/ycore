CURRENT_DIR=$(pwd)
BUILD_DIR="2_build"
EXE_DIR="0_bin/Lnx64"
#FILE_NAME="ygate"
FILE_NAME="AutoTable2Code"
#rm -rf $BUILD_DIR

#exec "scl enable devtoolset-11 bash"

lsof -ti:8989 | xargs -r kill -9

if [ ! -d "$BUILD_DIR" ]; then
    mkdir -p "$BUILD_DIR"
    echo "Folder created: $BUILD_DIR"
#else
#    echo "Folder already exists: $BUILD_DIR"
fi

#mkdir $BUILD_DIR

cd $CURRENT_DIR
cd $BUILD_DIR
cmake ..
make -j8

cd $CURRENT_DIR
cd $EXE_DIR

./$FILE_NAME
