CPP_SRCS += \
  src/RDF.cpp \
  src/RDFNTriplesParser.cpp \
  src/RDFNTriplesSerializer.cpp \
  extra/test/RDF_test.cpp \
  extra/test/RDFNTriplesParser_test.cpp \
  extra/test/RDFNTriplesSerializer_test.cpp \
  extra/test/test.cpp

OBJS += \
  ./build/RDF.o \
  ./build/RDFNTriplesParser.o \
  ./build/RDFNTriplesSerializer.o \
  ./build/RDF_test.o \
  ./build/RDFNTriplesParser_test.o \
  ./build/RDFNTriplesSerializer_test.o \
  ./build/test.o 

CPP_DEPS += \
  ./build/RDF.d \
  ./build/RDFNTriplesParser.d \
  ./build/RDFNTriplesSerializer.d \
  ./build/RDF_test.d \
  ./build/RDFNTriplesParser_test.d \
  ./build/RDFNTriplesSerializer_test.d \
  ./build/test.d

LIBS += \
  -lgtest \
  -lpthread

./build/%.o: ./src/%.cpp
	@mkdir -p ./build
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"

./build/%.o: ./extra/test/%.cpp
	@mkdir -p ./build
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<" -I src

all: test

test: $(OBJS)
	g++ -o test $(OBJS) $(LIBS)

clean:
	@rm -rf build
	@rm -f test
