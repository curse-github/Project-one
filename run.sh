export VULKAN_SDK=../1.4.321.1/x86_64
make ./out/app.out
cp -r Resources out
cd out
./app.out
cd ..
make clean
