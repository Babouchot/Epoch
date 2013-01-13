ALL = Run.exe

# Targets



Run.exe : BasicRenderer.o ContactListener.o DeadObject.o LivingObject.o Main.o Object.o Renderer.o World.o WorldLoader.o WorldRenderer.o 
	g++ -lBox2D -o Run.exe BasicRenderer.o ContactListener.o DeadObject.o LivingObject.o Main.o Object.o Renderer.o World.o WorldLoader.o WorldRenderer.o 

BasicRenderer.o: BasicRenderer.cpp BasicRenderer.h Renderer.h  ObjectData.h

	g++ -c  BasicRenderer.cpp 


ContactListener.o: ContactListener.cpp ContactListener.h

	g++ -c  ContactListener.cpp 


DeadObject.o: DeadObject.cpp DeadObject.h Object.h Renderer.h  ObjectData.h LivingObject.h

	g++ -c  DeadObject.cpp 


LivingObject.o: LivingObject.cpp LivingObject.h Object.h Renderer.h  ObjectData.h

	g++ -c  LivingObject.cpp 


Main.o: Main.cpp

	g++ -c  Main.cpp



Object.o: Object.cpp Object.h Renderer.h ObjectData.h

	g++ -c  Object.cpp 


Renderer.o: Renderer.cpp Renderer.h ObjectData.h

	g++ -c  Renderer.cpp 


World.o: World.cpp World.h Object.h Renderer.h ObjectData.h  LivingObject.h DeadObject.h WorldRenderer.h

	g++ -c  World.cpp 


WorldLoader.o: WorldLoader.cpp WorldLoader.h

	g++ -c  WorldLoader.cpp 


WorldRenderer.o: WorldRenderer.cpp WorldRenderer.h Renderer.h  ObjectData.h

	g++ -c  WorldRenderer.cpp 


clean : 
	rm *.o *.exe