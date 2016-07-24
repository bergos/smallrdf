#ifndef __RDF_H__
#define __RDF_H__

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "RDFList.h"

#define rdf_malloc malloc
#define rdf_memcpy memcpy
#define rdf_free free

class RDFString {
  public:
	const uint8_t* buf;
	size_t length;

	RDFString ();
	RDFString (const uint8_t* buf, size_t length);
    RDFString (const char* buf);
	~RDFString();

	const char* c_str();

  protected:
	char* _c_str;
};

typedef enum {
  RDF_NAMED_NODE,
  RDF_BLANK_NODE,
  RDF_LITERAL,
  RDF_DEFAULT_GRAPH,
  RDF_VARIABLE
} RDFTermType;

class RDFTerm {
  public:
    RDFTermType termType;
    RDFString value;

	RDFTerm(RDFTermType termType, RDFString value);
};

class RDFNamedNode : public RDFTerm {
  public:
    RDFNamedNode(RDFString value);
};

class RDFLiteral : public RDFTerm {
  public:
    RDFLiteral(RDFString value);
};

class RDFBlankNode : public RDFTerm {
  public:
	RDFBlankNode(RDFString value);
};

class RDFDefaultGraph : public RDFTerm {
  public:
    RDFDefaultGraph();
};

class RDFQuad {
  public:
    RDFTerm* subject;
	RDFTerm* predicate;
	RDFTerm* object;
	RDFTerm* graph;
	
	RDFQuad(RDFTerm* subject, RDFTerm* predicate, RDFTerm* object, RDFTerm* graph);
};

class RDFDocument {
  public:
	RDFList<RDFQuad*> quads;

	~RDFDocument();

    RDFNamedNode* namedNode(RDFString value);
    RDFLiteral* literal(RDFString value);
    RDFBlankNode* blankNode(RDFString value);
    RDFDefaultGraph* defaultGraph();
    RDFQuad* triple(RDFTerm* subject, RDFTerm* predicate, RDFTerm* object);
    RDFQuad* quad(RDFTerm* subject, RDFTerm* predicate, RDFTerm* object, RDFTerm* graph);

  protected:
    RDFDefaultGraph _defaultGraph;
    RDFList<RDFTerm*> _terms;
};

#endif //__RDF_H__
