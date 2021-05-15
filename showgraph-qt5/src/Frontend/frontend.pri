# Frontend

HEADERS += $$SRC_DIR/Frontend/fe_gcc.h \
	$$SRC_DIR/Frontend/fe_iface.h \
	$$SRC_DIR/Frontend/fe_test.h \
	$$SRC_DIR/Frontend/parser.h \
	$$SRC_DIR/Frontend/symtab.h \

SOURCES += $$SRC_DIR/Frontend/fe_gcc.cpp \
	$$SRC_DIR/Frontend/fe_icc.cpp \
	$$SRC_DIR/Frontend/fe_llvm.cpp \
	$$SRC_DIR/Frontend/fe_test.cpp \
	$$SRC_DIR/Frontend/parser.cpp

