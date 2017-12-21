/*
 * Copyright © 2017 Vincent Lachenal
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#ifndef STATISTICS_DAO_HPP
#define STATISTICS_DAO_HPP

#include "testSuiteDTO.hpp"
#include "sql/sqlConnectionFactory.hpp"


/*!
 * Statistics DAO
 *
 * \author Vincent Lachenal
 */
class StatisticsDAO {

  // Attributes +
private:
  /*! SQL connections pool */
  anch::sql::SqlConnectionPool& pool;

  /*! RAM */
  std::string memory;

  /*! CPU */
  std::string cpu;
  // Attributes -

  // Constructors +
public:
  /*!
   * \ref StatisticsDAO default constructor
   */
  StatisticsDAO();
  // Constructors -

  // Destructor +
public:
  /*!
   * \ref StatisticsDAO default constructor
   */
  virtual ~StatisticsDAO();
  // Destructor -

  // Methods +
public:
  /*!
   * Store test suite result into database
   *
   * \param testSuite the test suite result
   */
  void save(TestSuiteDTO& testSuite);
  // Methods -

};

#endif //STATISTICS_DAO_HPP
