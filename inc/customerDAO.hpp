/*
 * Copyright © 2017 Vincent Lachenal
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#ifndef CUSTOMER_DAO_HPP
#define CUSTOMER_DAO_HPP

#include "customerDTO.hpp"
#include "sql/sqlConnectionFactory.hpp"


/*!
 * Customer DAO
 *
 * \author Vincent Lachenal
 */
class CustomerDAO {

  // Attributes +
private:
  /*! Vacuum requests */
  std::vector<std::string> vacuumReqs;

  /*! SQL connections pool */
  anch::sql::SqlConnectionPool& pool;
  // Attributes -

  // Constructors +
public:
  /*!
   * \ref CustomerDAO default constructor
   */
  CustomerDAO();
  // Constructors -

  // Destructor +
public:
  /*!
   * \ref CustomerDAO default constructor
   */
  virtual ~CustomerDAO();
  // Destructor -

  // Methods +
public:
  /*!
   * List all customers
   *
   * \return the customers
   */
  std::vector<CustomerDTO> listCustomers();

  /*!
   * Get customer details
   *
   * \param uuid the customer UUID
   *
   * \return the customer details
   */
  CustomerDTO getDetails(const std::string& uuid);

  /*!
   * Create customer
   *
   * \param customer the customer to create
   *
   * \return the new customer UUID
   */
  std::string create(const CustomerDTO& customer);

  /*!
   * Delete all customers
   */
  void deleteAll();
  // Methods -

};

#endif // CUSTOMER_DAO_HPP
