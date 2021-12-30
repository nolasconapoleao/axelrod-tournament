# Before running this script you should run: 
# chmod +x build.sh

# Create and enter build folder
mkdir build
cd build

# Build project and run executable
cmake ..
make all
./axelrod
