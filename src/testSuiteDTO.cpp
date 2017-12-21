/*
 * Copyright © 2017 Vincent Lachenal
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#include "testSuiteDTO.hpp"


// Constructors +
TestSuiteDTO::TestSuiteDTO():
  id(),
  clientCpu(),
  clientMemory(),
  clientJvmVersion(),
  clientJvmVendor(),
  clientOsName(),
  clientOsVersion(),
  serverCpu(),
  serverMemory(),
  serverJvmVersion(),
  serverJvmVendor(),
  serverOsName(),
  serverOsVersion(),
  protocol(),
  compression(),
  nbThreads(),
  comment(),
  calls(),
  mapper() {
  // Nothing to do
}

TestSuiteDTO::TestSuiteDTO(const TestSuiteDTO& other):
  id(other.id),
  clientCpu(other.clientCpu),
  clientMemory(other.clientMemory),
  clientJvmVersion(other.clientJvmVersion),
  clientJvmVendor(other.clientJvmVendor),
  clientOsName(other.clientOsName),
  clientOsVersion(other.clientOsVersion),
  serverCpu(other.serverCpu),
  serverMemory(other.serverMemory),
  serverJvmVersion(other.serverJvmVersion),
  serverJvmVendor(other.serverJvmVendor),
  serverOsName(other.serverOsName),
  serverOsVersion(other.serverOsVersion),
  protocol(other.protocol),
  compression(other.compression),
  nbThreads(other.nbThreads),
  comment(other.comment),
  calls(other.calls),
  mapper(other.mapper) {
  // Nothing to do
}
// Constructors -


// Destructor +
TestSuiteDTO::~TestSuiteDTO() {
  // Nothing to do
}
// Destructor -
