O_BUILD = g++ -O3 -march=native -funroll-loops -flto -I./Include -I./lib/Include -I$(VULKAN_SDK)/Include -o
SPV_BUILD = $(VULKAN_SDK)/Bin/glslc.exe -o
EXE_BUILD = g++ -O3 -march=native -funroll-loops -flto -L./Lib -L$(VULKAN_SDK)/Lib -o

Files = app Window Engine Device Pipeline Swapchain Model Renderer RenderSystem GameObject Helpers
Shaders = simpleVert.vert simpleFrag.frag

./out/%.o: ./Include/%.h | ./src/%.cpp
	@$(O_BUILD) $@ -c $|
./out/shaders/%.spv: ./src/shaders/%
	@$(SPV_BUILD) $@ $^

makefolders:
	@-mkdir out
	@-mkdir out\shaders
shaders: $(Shaders:%=./out/shaders/%.spv)
./out/app.exe: makefolders shaders | $(Files:%=./out/%.o)
	@$(EXE_BUILD) $@ $| -lglfw3 -lvulkan -lgdi32
clean:
	rmdir /s /q out