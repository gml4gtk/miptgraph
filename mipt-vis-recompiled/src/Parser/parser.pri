# Parser

HEADERS += $$SRC_DIR/Parser/graph_parser.h \
	$$SRC_DIR/Parser/parser.h \
	$$SRC_DIR/Parser/parser_iface.h

SOURCES += $$SRC_DIR/Parser/gcc_parser.cpp \
	$$SRC_DIR/Parser/graph_parser.cpp \
	$$SRC_DIR/Parser/icc_parser.cpp \
	$$SRC_DIR/Parser/parser_impl.cpp