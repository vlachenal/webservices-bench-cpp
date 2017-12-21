/*
 * Copyright © 2017 Vincent Lachenal
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#ifndef CUSTOMER_HANDLER_HPP
#define CUSTOMER_HANDLER_HPP

#include "CustomerService.h"
#include "customerDAO.hpp"


/*!
 * Customer service implementation
 *
 * \author Vincent Lachenal
 */
class CustomerHandler: public CustomerServiceIf {

  // Attributes +
private:
  /** Customer DAO */
  CustomerDAO& _dao;
  // Attributes -


  // Constructors +
public:
  CustomerHandler(CustomerDAO& dao);
  // Constructors -

  // Destructor +
public:
  virtual ~CustomerHandler();
  // Destructor -

  // Methods +
public:
  /*!
   * List all customers in database
   *
   * \param request the request
   * \param response customers
   *
   * \throw CustomerException any error
   * \throw TException unexpected error
   */
  virtual void listCustomers(std::vector<Customer>& response, const ListAllRequest& request) override;

  /*!
   * Retrieve customer details
   *
   * \param request the get details request
   * \param response the customer details
   *
   * \throw CustomerException any error
   * \throw TException unexpected error
   */
  virtual void get(Customer& response, const GetRequest& request) override;

  /*!
   * Create customer
   *
   * \param request the create request
   * \param response the new customer's identifier
   *
   * \throw CustomerException any error
   * \throw TException unexpected error
   */
  virtual void create(std::string& response, const CreateRequest& request) override;

  /*!
   * Delete all customers
   *
   * \throw CustomerException any error
   * \throw TException unexpected error
   */
  virtual void deleteAll() override;
  // Methods -

};

#endif // CUSTOMER_HANDLER_HPP
