
@echo off

IF NOT EXIST musicformats.sln (
	echo "Warning, this script must be called from the build folder (e.g. win64)"
	GOTO END
)


cmake ..  -DPACK=on -DLILY=on -DBRL=on
cmake --build . --config Release --  /maxcpucount:4
cpack -G NSIS64
move musicformats-*.exe ..

:END
