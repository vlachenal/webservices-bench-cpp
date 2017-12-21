/*
 * Copyright © 2017 Vincent Lachenal
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#include "statisticsDAO.hpp"

#include <sys/utsname.h>
#include "uuid.hpp"
#include "resource/resource.hpp"


// SQL requests +
const static std::string INS_TEST_SUITE("INSERT INTO TestSuite "
					"(id, client_cpu, client_memory, client_jvm_version, client_jvm_vendor, client_os_name, "
					"client_os_version, server_cpu, server_memory, server_jvm_version, server_jvm_vendor, "
					"server_os_name, server_os_version, protocol, compression, nb_threads, comment, mapper) "
					"VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)");
const static std::string INS_TEST_CALL("INSERT INTO TestCall "
				       "(request_seq, test_suite_id, method, client_start, server_start, server_end, client_end, ok, error_message) "
				       "VALUES (?,?,?,?,?,?,?,?,?)");
// SQL requests -


// Constructors +
StatisticsDAO::StatisticsDAO(): pool(anch::sql::SqlConnectionFactory::getInstance().getPool("apibenchmark")), memory(), cpu() {
  const anch::resource::Resource& res = anch::resource::Resource::getResource("hardware.conf");
  bool found = res.getParameter(memory, "memory");
  if(!found) {
    memory = "?";
  }
  found = res.getParameter(cpu, "cpu");
  if(!found) {
    cpu = "?";
  }
}
// Constructors -


// Destructor +
StatisticsDAO::~StatisticsDAO() {
  // Nothing to do
}
// Destructor -


// Methods +
void
StatisticsDAO::save(TestSuiteDTO& testSuite) {
  if(testSuite.calls.empty()) {
    return;
  }

  // Gather system informations +
  std::ostringstream gccVersion;
  gccVersion << __GNUC__ << '.' << __GNUC_MINOR__ << '.' << __GNUC_PATCHLEVEL__;
  testSuite.serverJvmVersion = gccVersion.str();
  testSuite.serverJvmVendor = "GCC";
  utsname osInfo;
  int ret = uname(&osInfo);
  if(ret != 0) {
    testSuite.serverOsName = "UNKNOW";
    testSuite.serverOsVersion = "X.Y.Z";
  } else {
    std::ostringstream osName;
    osName << osInfo.sysname << ' ' << osInfo.release;
    testSuite.serverOsName = osName.str();
    testSuite.serverOsVersion = osInfo.version;
  }
  testSuite.serverCpu = cpu;
  testSuite.serverMemory = memory;
  // Gather system informations -

  anch::Uuid uuid;
  anch::Uuid::generateUuid(uuid);
  std::string strUuid = uuid.toString();

  auto res = pool.borrowResource();
  res.get().begin();
  try {
    // Insert new test suite +
    res.get().update(INS_TEST_SUITE,
		     strUuid,
		     testSuite.clientCpu,
		     testSuite.clientMemory,
		     testSuite.clientJvmVersion,
		     testSuite.clientJvmVendor,
		     testSuite.clientOsName,
		     testSuite.clientOsVersion,
		     testSuite.serverCpu,
		     testSuite.serverMemory,
		     testSuite.serverJvmVersion,
		     testSuite.serverJvmVendor,
		     testSuite.serverOsName,
		     testSuite.serverOsVersion,
		     testSuite.protocol,
		     testSuite.compression,
		     testSuite.nbThreads,
		     testSuite.comment,
		     testSuite.mapper
    );
    // Insert new test suite -

    // Insert calls +
    res.get().batchUpdate<CallDTO>(INS_TEST_CALL, [strUuid](anch::sql::PreparedStatement& stmt, const CallDTO& call) {
        stmt.set(1, call.requestSeq);
        stmt.set(2, strUuid);
        stmt.set(3, *call.method);
        stmt.set(4, call.clientStart);
        stmt.set(5, call.serverStart);
        stmt.set(6, call.serverEnd);
        stmt.set(7, call.clientEnd);
        stmt.set(8, call.ok);
        stmt.set(9, call.errMsg);
      }, testSuite.calls);
    // Insert calls -

  } catch(...) {
    res.get().rollback();
    throw;
  }
  res.get().commit();
}
// Methods -
