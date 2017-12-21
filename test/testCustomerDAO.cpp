/*
 * Copyright © 2017 Vincent Lachenal
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file for more details.
 */
#include "customerDAO.hpp"
#include "date/dateFormatter.hpp"

#include <iostream>
#include <vector>

int
main(void) {
  std::cout << "Enter in testCustomerDAO" << std::endl;

  // Register PostgreSQL layer library because it is done in main function
  CustomerDAO dao;

  // Create customer +
  std::cout << "Create a new customer" << std::endl;
  CustomerDTO customer;
  customer.firstName = "Chuck";
  customer.lastName = "Norris";
  anch::date::DateFormatter formatter("%Y-%m-%d");
  formatter.parse("1940-03-10", customer.birthDate);
  customer.email = "chuck.norris@yopmail.com";
  customer.address.zipCode = "33500";
  customer.address.city = "Libourne";
  customer.address.country = "France";
  PhoneDTO phone;
  phone.type = PhoneTypeDTO::LANDLINE;
  phone.number = "0836656565";
  customer.phones.push_back(phone);
  std::string uuid = dao.create(customer);
  std::cout << "New customer ID is " << uuid << std::endl;
  // Create customer -

  // List customers +
  std::cout << "List all customers in database" << std::endl;
  std::vector<CustomerDTO> customers = dao.listCustomers();
  bool found = false;
  for(auto iter = customers.cbegin() ; iter != customers.cend() ; ++iter) {
    if(uuid == iter->id.toString()) {
      found = true;
      break;
    }
  }
  if(!found) {
    std::cerr << uuid << " customer has not been found." << std::endl;
    return 1;
  }
  // List customers -

  // Get customer details +
  CustomerDTO cust = dao.getDetails(uuid);
  std::cout << "Found customer " << cust.id << '\n'
	    << "First name: " << cust.firstName << '\n'
	    << "Last name: " << cust.lastName << '\n'
	    << "Birth date: " << cust.birthDate << '\n'
	    << "Email: " << cust.email << '\n'
	    << "Address:" << '\n'
	    << cust.address.zipCode << '\n'
	    << cust.address.city << '\n'
	    << cust.address.country << '\n'
	    << "Phone: " << cust.phones[0].number << std::endl;
  if(uuid != static_cast<std::string>(cust.id)) {
    std::cerr << "Found ID " << cust.id << " instead of " << uuid << std::endl;
    return 1;
  }
  // Get customer details -

  // Delete customers +
  dao.deleteAll();
  customers = dao.listCustomers();
  if(!customers.empty()) {
    std::cerr << "Customers should have been deleted." << std::endl;
    return 1;
  }
  // Delete customers -

  std::cout << "Exit testCustomerDAO" << std::endl;
  return 0;
}
