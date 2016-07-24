#include "RDF.h"

RDFString::RDFString () : buf(NULL), length(0), _c_str(NULL) {}

RDFString::RDFString (const uint8_t* buf, size_t length) : buf(buf), length(length), _c_str(NULL) {}

RDFString::RDFString (const char* buf) : buf((uint8_t*)buf), length(strlen(buf)), _c_str(NULL) {}

RDFString::~RDFString () {
  if (_c_str) {
    rdf_free(_c_str);
  }
}

const char* RDFString::c_str() {
  if (_c_str == NULL) {
	_c_str = (char*)rdf_malloc(length + 1);
	rdf_memcpy(_c_str, buf, length);
	*(_c_str + length) = 0;
  }

  return _c_str;
}

RDFTerm::RDFTerm (RDFTermType termType, RDFString value) : termType(termType), value(value) {}

RDFNamedNode::RDFNamedNode(RDFString value) : RDFTerm(RDF_NAMED_NODE, value) {}

RDFLiteral::RDFLiteral(RDFString value) : RDFTerm(RDF_LITERAL, value) {}

RDFBlankNode::RDFBlankNode(RDFString value) : RDFTerm(RDF_BLANK_NODE, value) {}

RDFDefaultGraph::RDFDefaultGraph () : RDFTerm(RDF_DEFAULT_GRAPH, RDFString("")) {}

RDFQuad::RDFQuad (RDFTerm* subject, RDFTerm* predicate, RDFTerm* object, RDFTerm* graph) : subject(subject), predicate(predicate), object(object), graph(graph) {}

RDFDocument::~RDFDocument () {
  for (int i=0; i<quads.length; i++) {
    delete quads.get(i);
  }

  for (int i=0; i<_terms.length; i++) {
    delete _terms.get(i);
  }
}

RDFNamedNode* RDFDocument::namedNode(RDFString value) {
  return (RDFNamedNode*)_terms.add(new RDFNamedNode(value));
}

RDFLiteral* RDFDocument::literal(RDFString value) {
  return (RDFLiteral*)_terms.add(new RDFLiteral(value));
}

RDFBlankNode* RDFDocument::blankNode(RDFString value) {
  return (RDFBlankNode*)_terms.add(new RDFBlankNode(value));
}

RDFDefaultGraph* RDFDocument::defaultGraph() {
  return &_defaultGraph;
}

RDFQuad* RDFDocument::triple(RDFTerm* subject, RDFTerm* predicate, RDFTerm* object) {
  return quads.add(new RDFQuad(subject, predicate, object, &_defaultGraph));
}

RDFQuad* RDFDocument::quad(RDFTerm* subject, RDFTerm* predicate, RDFTerm* object, RDFTerm* graph) {
  return quads.add(new RDFQuad(subject, predicate, object, graph));
}
