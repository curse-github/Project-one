ifeq ($(OS),Windows_NT)
LIB_DIR = ./Lib
CONST_ARGS = -D_WINDOWS=1
else
LIB_DIR = ./Lib/Linux
CONST_ARGS = -D_LINUX=1
endif
CONST_ARGS += -D_DEBUG=0

SPV_BUILD = $(VULKAN_SDK)/Bin/glslc -o
O_BUILD = g++ -O3 -march=native -funroll-loops -flto -I./Include -I./Include/Eng -I./Lib/Include -I$(VULKAN_SDK)/Include $(CONST_ARGS) -o
EXE_BUILD = g++ -O3 -march=native -funroll-loops -flto -L$(LIB_DIR) -L$(VULKAN_SDK)/Lib -o

Files = app Engine GameObject Camera Loaders Helpers
EngineFiles = Eng/Window Eng/Pipeline Eng/Swapchain Eng/Renderer Eng/RenderSystem Eng/Mesh Eng/Buffer Eng/Descriptors Eng/Device
allFiles = $(Files) $(EngineFiles)
Shaders = simpleVert.vert simpleFrag.frag

./out/%.o: makefolders ./Include/%.h | ./Src/%.cpp
	@$(O_BUILD) $@ -c $|
./out/shaders/%.spv: makefolders | ./Src/shaders/%
	@$(SPV_BUILD) $@ $|

makefolders:
	@-mkdir out
ifeq ($(OS),Windows_NT)
	@-mkdir out\Eng
	@-mkdir out\shaders
else
	@-mkdir out/Eng
	@-mkdir out/shaders
endif
shaders: $(Shaders:%=./out/shaders/%.spv)
./out/app.exe: makefolders shaders | $(allFiles:%=./out/%.o)
	@$(EXE_BUILD) $@ $| -lglfw3 -lvulkan -lgdi32
./out/app.out: makefolders shaders | $(allFiles:%=./out/%.o)
	@$(EXE_BUILD) $@ $| -lglfw3 -lvulkan
clean:
ifeq ($(OS),Windows_NT)
	rmdir /s /q out
else
	rm -Rf ./out
endif
