ALL = Run.exe

# Targets



Run.exe : ArousalReader.o fft.o main.o 
	-g++ -g -o Run.exe ArousalReader.o fft.o main.o lib/*.so*
	-[ -e "Build" ] || mkdir "Build"
	-mv *.o *.exe Build

ArousalReader.o: Sources/ArousalReader.cpp Sources/ArousalReader.h  Sources/EmoStateDLL.h Sources/edk.h Sources/edkErrorCode.h Sources/fft.h

	-g++ -g -c  Sources/ArousalReader.cpp 


fft.o: Sources/fft.cpp Sources/fft.h

	-g++ -g -c  Sources/fft.cpp 


main.o: Sources/main.cpp Sources/ArousalReader.h Sources/EmoStateDLL.h  Sources/edk.h Sources/edkErrorCode.h Sources/fft.h

	-g++ -g -c  Sources/main.cpp 


clean : 
	-rm *.o *.exe */*.o */*.exe
