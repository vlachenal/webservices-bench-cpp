/*
 * Copyright © 2017 Vincent Lachenal
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#include <thrift/concurrency/ThreadManager.h>
#include <thrift/concurrency/PlatformThreadFactory.h>
#include <thrift/protocol/TCompactProtocol.h>
#include <thrift/processor/TMultiplexedProcessor.h>
#include <thrift/server/TSimpleServer.h>
#include <thrift/server/TThreadPoolServer.h>
#include <thrift/server/TThreadedServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TSocket.h>
#include <thrift/transport/TTransportUtils.h>
#include <thrift/TToString.h>

#include <iostream>
#include <stdexcept>
#include <sstream>

#include "CustomerService.h"
#include "customerDAO.hpp"
#include "customerHandler.hpp"

#include "StatsService.h"
#include "statisticsDAO.hpp"
#include "statisticsHandler.hpp"


using apache::thrift::concurrency::ThreadManager;
using apache::thrift::concurrency::PlatformThreadFactory;
using apache::thrift::server::TThreadPoolServer;
using apache::thrift::transport::TServerSocket;
using apache::thrift::transport::TSocket;
using apache::thrift::transport::TBufferedTransportFactory;
using apache::thrift::protocol::TCompactProtocolFactory;
using apache::thrift::TMultiplexedProcessor;

/*
  CalculatorIfFactory is code generated.
  CalculatorCloneFactory is useful for getting access to the server side of the
  transport.  It is also useful for making per-connection state.  Without this
  CloneFactory, all connections will end up sharing the same handler instance.
*/
/*class CustomerCloneFactory : virtual public CustomerServiceIfFactory {
private:
  CustomerDAO& _dao;
 public:
  CustomerCloneFactory(CustomerDAO& dao): _dao(dao) {}
  virtual ~CustomerCloneFactory() {}
  virtual CustomerServiceIf* getHandler(const ::apache::thrift::TConnectionInfo& connInfo) {
    std::shared_ptr<TSocket> sock = std::dynamic_pointer_cast<TSocket>(connInfo.transport);
    std::cout << "Incoming connection\n";
    std::cout << "\tSocketInfo: "  << sock->getSocketInfo() << "\n";
    std::cout << "\tPeerHost: "    << sock->getPeerHost() << "\n";
    std::cout << "\tPeerAddress: " << sock->getPeerAddress() << "\n";
    std::cout << "\tPeerPort: "    << sock->getPeerPort() << "\n";
    return new CustomerHandler(_dao);
  }
  virtual void releaseHandler(CustomerServiceIf* handler) {
    delete handler;
  }
  };*/

/*!
 * Application entry point
 *
 * \param argc the number of parameters
 * \param argv the parameters
 *
 * \return the application return code
 */
int
main(int argc, char* argv[]) {

  CustomerDAO custDAO;
  StatisticsDAO statDAO;

  const int workerCount = 10;

  std::shared_ptr<ThreadManager> threadManager = ThreadManager::newSimpleThreadManager(workerCount);
  threadManager->threadFactory(std::make_shared<PlatformThreadFactory>());
  threadManager->start();

  // This server allows "workerCount" connection at a time, and reuses threads
  std::shared_ptr<TMultiplexedProcessor> processor = std::make_shared<TMultiplexedProcessor>();
  processor.get()->registerProcessor("customer", std::make_shared<CustomerServiceProcessor>(std::make_shared<CustomerHandler>(custDAO)));
  processor.get()->registerProcessor("stats", std::make_shared<StatsServiceProcessor>(std::make_shared<StatisticsHandler>(statDAO)));
  TThreadPoolServer server(processor,
			   std::make_shared<TServerSocket>(9090),
			   std::make_shared<TBufferedTransportFactory>(),
			   std::make_shared<TCompactProtocolFactory>(),
			   threadManager);

  std::cout << "Starting the server..." << std::endl;
  server.serve();
  std::cout << "Done." << std::endl;

  return 0;
}
