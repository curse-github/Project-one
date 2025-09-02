ifeq ($(OS),Windows_NT)
LIB_DIR = ./Lib
OS_ARGS = -D_WINDOWS=1
else
LIB_DIR = ./Lib/Linux
OS_ARGS = -D_LINUX=1
endif

SPV_BUILD = $(VULKAN_SDK)/Bin/glslc -o
O_BUILD = g++ -O3 -march=native -funroll-loops -flto -I./Include -I./Lib/Include -I$(VULKAN_SDK)/Include $(OS_ARGS) -o
EXE_BUILD = g++ -O3 -march=native -funroll-loops -flto -L$(LIB_DIR) -L$(VULKAN_SDK)/Lib -o

Files = app Window Engine Device Pipeline Swapchain Mesh Renderer RenderSystem GameObject Camera Loaders Helpers
Shaders = simpleVert.vert simpleFrag.frag

./out/%.o: makefolders ./Include/%.h | ./Src/%.cpp
	@$(O_BUILD) $@ -c $|
./out/shaders/%.spv: makefolders | ./Src/shaders/%
	@$(SPV_BUILD) $@ $|

makefolders:
	@-mkdir out
	@-mkdir out/shaders
shaders: $(Shaders:%=./out/shaders/%.spv)
./out/app.exe: makefolders shaders | $(Files:%=./out/%.o)
	@$(EXE_BUILD) $@ $| -lglfw3 -lvulkan -lgdi32
./out/app.out: makefolders shaders | $(Files:%=./out/%.o)
	@$(EXE_BUILD) $@ $| -lglfw3 -lvulkan
clean:
ifeq ($(OS),Windows_NT)
	rmdir /s /q out
else
	rm -Rf ./out
endif
