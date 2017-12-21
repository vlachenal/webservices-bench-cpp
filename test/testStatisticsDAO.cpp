/*
 * Copyright © 2017 Vincent Lachenal
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#include "statisticsDAO.hpp"

#include "testSuiteDTO.hpp"

const std::string TEST_THRIFT_PROTO("thrift");
const std::string TEST_METHOD("plop");

int
main(void) {
  std::cout << "Enter in testStatisticsDAO" << std::endl;

  StatisticsDAO dao;
  TestSuiteDTO suite;
  suite.clientCpu = "Intel Xeon X123456@400GHz";
  suite.clientMemory = "123456To";
  suite.clientJvmVersion = "7.2.1";
  suite.clientJvmVendor = "GCC";
  suite.clientOsName = "Linux";
  suite.clientOsVersion = "4.14.7-1-ARCH";
  suite.serverCpu = "Intel Xeon X123456@400GHz";
  suite.serverMemory = "123456To";
  suite.serverJvmVersion = "7.2.1";
  suite.serverJvmVendor = "GCC";
  suite.serverOsName = "Linux";
  suite.serverOsVersion = "4.14.7-1-ARCH";
  suite.protocol = "Thrift";
  suite.compression = "none";
  suite.nbThreads = 4;
  suite.comment = "Unit test";
  {
    CallDTO call;
    call.initialized = true;
    call.requestSeq = 1;
    call.protocol = &TEST_THRIFT_PROTO;
    call.method = &TEST_METHOD;
    call.clientStart = 0;
    call.serverStart = 1;
    call.serverEnd = 2;
    call.clientEnd = 3;
    call.ok = true;
    suite.calls.push_back(call);
  }
  {
    CallDTO call;
    call.initialized = true;
    call.requestSeq = 2;
    call.protocol = &TEST_THRIFT_PROTO;
    call.method = &TEST_METHOD;
    call.clientStart = 4;
    call.serverStart = 5;
    call.serverEnd = 6;
    call.clientEnd = 7;
    call.ok = false;
    call.errMsg = "plop";
    suite.calls.push_back(call);
  }
  suite.mapper = "manual";
  dao.save(suite);

  std::cout << "Exit testStatisticsDAO" << std::endl;
  return 0;
}
