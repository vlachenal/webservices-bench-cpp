/*
 * Copyright © 2017 Vincent Lachenal
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#ifndef TEST_SUITE_DTO_HPP
#define TEST_SUITE_DTO_HPP

#include <string>
#include <list>

#include "callDTO.hpp"


/*!
 * Test suite DTO
 *
 * \author Vincent Lachenal
 */
struct TestSuiteDTO {

  // Attributes +
  /** Test suite UUID */
  std::string id;

  /** Client CPU information */
  std::string clientCpu;

  /** Client memory information */
  std::string clientMemory;

  /** Client JVM version */
  std::string clientJvmVersion;

  /** Client JVM vendor */
  std::string clientJvmVendor;

  /** Client OS name */
  std::string clientOsName;

  /** Client OS version */
  std::string clientOsVersion;

  /** Server CPU information */
  std::string serverCpu;

  /** Server memory information */
  std::string serverMemory;

  /** Server JVM version */
  std::string serverJvmVersion;

  /** Server JVM vendor */
  std::string serverJvmVendor;

  /** Server OS name */
  std::string serverOsName;

  /** Server OS version */
  std::string serverOsVersion;

  /** Test suite protocol */
  std::string protocol;

  /** Test suite transport compression */
  std::string compression;

  /** Test suite number of simultaneous calls */
  uint32_t nbThreads;

  /** Comments */
  std::string comment;

  /** Calls */
  std::list<CallDTO> calls;

  /** The mapper which has been used */
  std::string mapper;
  // Attributes -


  // Constructors +
  /*!
   * \ref TestSuiteDTO default constructor
   */
  TestSuiteDTO();

  /*!
   * \ref TestSuiteDTO default constructor
   *
   * \param other the \ref TestSuiteDTO to copy
   */
  TestSuiteDTO(const TestSuiteDTO& other);
  // Constructors -


  // Destructor +
  /*!
   * \ref TestSuiteDTO destructor
   */
  virtual ~TestSuiteDTO();
  // Destructor -

};

#endif //TEST_SUITE_DTO_HPP
