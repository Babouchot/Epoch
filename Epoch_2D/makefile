ALL = Run.exe

# Targets



Run.exe : BasicRenderer.o CharacterObject.o ContactListener.o DeadObject.o LivingObject.o Main.o Object.o Renderer.o World.o WorldLoader.o WorldRenderer.o 
	-g++ -lBox2D -o Run.exe BasicRenderer.o CharacterObject.o ContactListener.o DeadObject.o LivingObject.o Main.o Object.o Renderer.o World.o WorldLoader.o WorldRenderer.o 
	-[ -e "Build" ] || mkdir "Build"
	-mv *.o *.exe Build

BasicRenderer.o: Sources/BasicRenderer.cpp Sources/BasicRenderer.h  Sources/Renderer.h Sources/ObjectData.h

	-g++ -c  Sources/BasicRenderer.cpp 


CharacterObject.o: Sources/CharacterObject.cpp Sources/CharacterObject.h  Sources/LivingObject.h Sources/Object.h Sources/Renderer.h  Sources/ObjectData.h

	-g++ -c  Sources/CharacterObject.cpp 


ContactListener.o: Sources/ContactListener.cpp Sources/ContactListener.h

	-g++ -c  Sources/ContactListener.cpp 


DeadObject.o: Sources/DeadObject.cpp Sources/DeadObject.h  Sources/Object.h Sources/Renderer.h Sources/ObjectData.h  Sources/LivingObject.h

	-g++ -c  Sources/DeadObject.cpp 


LivingObject.o: Sources/LivingObject.cpp Sources/LivingObject.h  Sources/Object.h Sources/Renderer.h Sources/ObjectData.h

	-g++ -c  Sources/LivingObject.cpp 


Main.o: Sources/Main.cpp

	-g++ -c  Sources/Main.cpp



Object.o: Sources/Object.cpp Sources/Object.h Sources/Renderer.h  Sources/ObjectData.h

	-g++ -c  Sources/Object.cpp 


Renderer.o: Sources/Renderer.cpp Sources/Renderer.h Sources/ObjectData.h  Sources/BasicRenderer.h

	-g++ -c  Sources/Renderer.cpp 


World.o: Sources/World.cpp Sources/World.h Sources/Object.h  Sources/Renderer.h Sources/ObjectData.h Sources/LivingObject.h  Sources/DeadObject.h Sources/WorldRenderer.h

	-g++ -c  Sources/World.cpp 


WorldLoader.o: Sources/WorldLoader.cpp Sources/WorldLoader.h

	-g++ -c  Sources/WorldLoader.cpp 


WorldRenderer.o: Sources/WorldRenderer.cpp Sources/WorldRenderer.h

	-g++ -c  Sources/WorldRenderer.cpp 


clean : 
	-rm *.o *.exe */*.o */*.exe