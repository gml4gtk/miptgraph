
SRC	= \
	Deriv.cpp \
	main.cpp \
	Graph/Edge.cpp \
	Graph/Graph.cpp \
	Graph/Node.cpp \
	Layout/Layout.cpp \
	Layout/LEdge.cpp \
	Layout/LGraph.cpp \
	Layout/LNode.cpp \
	Layout/Ordering.cpp

# qmake -query reported this include path
# QT_INSTALL_HEADERS:/usr/include/x86_64-linux-gnu/qt5
all:
	g++ -g -o main $(SRC) -I/usr/include/x86_64-linux-gnu/qt5/ -I/usr/include/x86_64-linux-gnu/qt5/include/  -I/usr/include/x86_64-linux-gnu/qt5/include/QtGui/ -I/usr/include/x86_64-linux-gnu/qt5/include/QtCore/ 

