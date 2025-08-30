O_BUILD = g++ -I./Include -I./lib/Include -I$(VULKAN_SDK)/Include -o
EXE_BUILD = g++ -o ./out/app.exe -L./Lib -L$(VULKAN_SDK)/Lib -o

Files = app Window Engine Helpers

./out/%.o: ./Include/%.h | ./src/%.cpp
	@$(O_BUILD) $@ -c $|

makefolders:
	@-mkdir out
./out/app.exe: makefolders | $(Files:%=./out/%.o)
	@$(EXE_BUILD) $@ $| -lglfw3 -lvulkan -lgdi32
clean:
	rmdir /s /q out