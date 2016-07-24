#include <stdio.h>

#include "RDF.h"
#include "RDFNTriples.h"

const char* ntriples =
  "<http://example.org/subject1> <http://example.org/predicate> <http://example.org/object> .\n"
  "<http://example.org/subject2> <http://example.org/predicate> <http://example.org/object>.\n"
  "<http://example.org/subject2> <http://example.org/predicate> \"literal\".\n"
  "<http://example.org/subject2> <http://example.org/predicate> _:blank1.\n";

void debugTerm(RDFTerm* term) {
  switch(term->termType) {
    case RDF_NAMED_NODE:
      printf("NamedNode: %s\n", term->value.c_str());
      break;
    case RDF_LITERAL:
	  printf("Literal: %s\n", term->value.c_str());
	  break;
    case RDF_BLANK_NODE:
      printf("BlankNode: %s\n", term->value.c_str());
      break;
    case RDF_DEFAULT_GRAPH:
      printf("DefaultGraph\n");
      break;
    case RDF_VARIABLE:
      printf("Variable: %s\n", term->value.c_str());
      break;
    default:
      printf("unknown term type: ");
      break;
  }
}

void debugQuad(RDFQuad* quad) {
  printf("Quad:\n");
  debugTerm(quad->subject);
  debugTerm(quad->predicate);
  debugTerm(quad->object);
  debugTerm(quad->graph);
  printf("\n");
}

void debugDocument(RDFDocument* document) {
  for (int i=0; i<document->quads.length; i++) {
    debugQuad(document->quads.get(i));
  }
}

int main (int argc, char** argv) {
  NTriplesParser parser;

  RDFDocument* document = parser.parse((const uint8_t*)ntriples, strlen(ntriples));

  //printf("%d\n", document->quads.length);
  //debugDocument(document);

  NTriplesSerializer serializer;

  const uint8_t* ser = serializer.serialize(document);

  printf("%s\n", ser);

  delete document;
}

