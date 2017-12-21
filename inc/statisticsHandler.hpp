/*
 * Copyright © 2017 Vincent Lachenal
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#ifndef STATISTICS_HANDLER_HPP
#define STATISTICS_HANDLER_HPP

#include "StatsService.h"
#include "statisticsDAO.hpp"


/*!
 * Statistics service implementation
 *
 * \author Vincent Lachenal
 */
class StatisticsHandler: public StatsServiceIf {

  // Attributes +
private:
  /** Customer DAO */
  StatisticsDAO& _dao;
  // Attributes -


  // Constructors +
public:
  StatisticsHandler(StatisticsDAO& dao);
  // Constructors -

  // Destructor +
public:
  virtual ~StatisticsHandler();
  // Destructor -

  // Methods +
  /*!
   * Consolidate statistics
   *
   * \param test the test suite
   *
   * \throw StatsException any error
   * \throw TException unexpected error
   */
  virtual void consolidate(const TestSuite& test);

  /*!
   * Purge server side statistics
   *
   * \throw StatsException any error
   * \throw TException unexpected error
   */
  virtual void purge();
  // Methods -

};

#endif // STATISTICS_HANDLER_HPP
