/* Copyright 2016 Thomas Bergwinkl. All Rights Reserved.

 */

#include "RDFNTriplesParser.h"

#include <gtest/gtest.h>

TEST(NTriplesParser, NamedNode) {
  RDFString input(
      "<http://example.org/subject> <http://example.org/predicate> <http://example.org/object> .");
  RDFDocument document;
  const RDFQuad* quad = NTriplesParser::parseQuad_static(&input, &document);

  ASSERT_TRUE(
      quad->subject->value->equals(RDFString("http://example.org/subject")));
  ASSERT_TRUE(
      quad->predicate->value->equals(
          RDFString("http://example.org/predicate")));
  ASSERT_TRUE(
      quad->object->value->equals(RDFString("http://example.org/object")));
}

TEST(NTriplesParser, Literal) {
  RDFString input(
      "<http://example.org/subject> <http://example.org/predicate> \"object\" .");
  RDFDocument document;
  const RDFQuad* quad = NTriplesParser::parseQuad_static(&input, &document);

  ASSERT_TRUE(
      quad->subject->value->equals(RDFString("http://example.org/subject")));
  ASSERT_TRUE(
      quad->predicate->value->equals(
          RDFString("http://example.org/predicate")));
  ASSERT_TRUE(quad->object->value->equals(RDFString("object")));
}
