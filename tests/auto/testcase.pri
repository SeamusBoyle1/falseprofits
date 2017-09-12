CONFIG += testcase

CONFIG += c++11

# gcov
with_gcov {
QMAKE_CXXFLAGS += -fprofile-arcs -ftest-coverage
LIBS += -lgcov
}
