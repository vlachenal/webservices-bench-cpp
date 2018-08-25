/*
 * Copyright © 2017 Vincent Lachenal
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#include "statisticsHandler.hpp"


// Functions +
/*!
 * Convert \ref ClientCall to \ref CallDTO
 *
 * \param call the \ref CallDTO
 * \param thrift the \ref ClientCall
 */
void
fromThrift(CallDTO& call, const ClientCall& thrift) {
  call.clientStart = thrift.clientStart;
  call.clientEnd = thrift.clientEnd;
  call.method = &thrift.method;
  call.protocol = &thrift.protocol;
  call.requestSeq = thrift.requestSeq;
  call.ok = thrift.ok;
  call.errMsg = thrift.errMsg;
}

/*!
 * Convert \ref TestSuite to \ref TestSuiteDTO
 *
 * \param test the \ref TestSuiteDTO
 * \param thrift the \ref TestSuite
 */
void
fromThrift(TestSuiteDTO& test, const TestSuite& thrift) {
  test.clientCpu = thrift.cpu;
  test.clientMemory = thrift.memory;
  test.clientJvmVersion = thrift.jvm;
  test.clientJvmVendor = thrift.vendor;
  test.clientOsName = thrift.osFamily;
  test.clientOsVersion = thrift.osVersion;
  test.nbThreads = thrift.nbThread;
  test.compression = thrift.compression;
  test.protocol = thrift.protocol;
  test.comment = thrift.comment;
  test.mapper = "manual";
}
// Functions -


// Constructors +
StatisticsHandler::StatisticsHandler(StatisticsDAO& dao): _dao(dao) {
  // Nothing to do
}
// Constructors -


// Destructor +
StatisticsHandler::~StatisticsHandler() {
  // Nothing to do
}
// Destructor -


// Methods +
void
StatisticsHandler::consolidate(const TestSuite& test) {
  if(test.calls.empty()) {
    StatsException error;
    error.message = "No calls to consolidate";
    throw error;
  }
  TestSuiteDTO suite;
  fromThrift(suite, test);
  std::list<CallDTO> calls;
  for(auto ccall : test.calls) {
    CallDTO call;
    fromThrift(call, ccall);
    call = mergeCall(call);
    if(call.initialized) {
      calls.push_back(call);
    }
  }
  suite.calls = calls;
  _dao.save(suite);
}

void
StatisticsHandler::purge() {
  cleanCalls();
}
// Methods -
