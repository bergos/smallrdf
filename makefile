CPP_SRCS += \
  RDF.cpp \
  RDFNTriplesParser.cpp \
  RDFNTriplesSerializer.cpp \
  RDF_test.cpp \
  RDFNTriplesParser_test.cpp \
  RDFNTriplesSerializer_test.cpp \
  test.cpp

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

#./build:
#	@mkdir ./build

./build/%.o: %.cpp
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"

all: test

test: $(OBJS)
	g++ -o test $(OBJS) $(LIBS)

clean:
	rm test $(OBJS) $(CPP_DEPS)
